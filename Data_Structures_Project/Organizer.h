#ifndef ORGANIZER_H
#define ORGANIZER_H

#include "UI.h"
#include "Hospital.h"
#include "Patient.h"
#include "Car.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ModifiedPriQ.h"
#include "ModifiedQ.h"

#include <string>
using namespace std;

struct CancellationReq
{
	int PID;
	int hospitalID;
	int CancellationTimestep;
};

class Organizer
{
private:
	// Lists used in orgranizer class
	Hospital** HospitalList;						// An array of pointers to hospitals
	ModifiedQ patientsList;	                        // Patients list of type Linked Queue (list of pointers to patients)
	LinkedQueue<CancellationReq> CancellationList;	// Cancellation requests' list of type Linked Queue
	LinkedQueue<Patient*> FinishedList;				// Finished patients' list of type Linked Queue
	priQueue<Car*> BackCars;						// Back cars' list (cars on their way back) of type Priority Queue
	ModifiedPriQ OutCars;							// Out cars' list (cars out on their way to pick up patients) of type Priority Queue (modified)
	priQueue<Car*> checkupList;						// Checkup cars list
	Hospital** failedHospitalsList;                 // Failed Hospitals list

	// General data members
	int timeStep;
	UI GUI;
	int numHospitals;
	int** distanceMatrix;
	double outCarsFailureProbability;
	double backCarsFailureProbability;
	double hospitalFailureProbability;
	int checkupTime;
	int NCFailuresOut;
	int NCFailuresBack;
	int SCFailuresOut;
	int SCFailuresBack;
	int numOfFailedHospitals;
	int numOfOutOfServiceSC;
	int numOfOutOfServiceNC;
	int numOfOutOfServiceCars;

	//File Loading data members (can be declared in file processing and freed at the end)
	string filename;//Keep
	string outfile; //Keep used when outputting results file
	int speedScars, speedNcars;
	int* scarsPerHospital;
	int* ncarsPerHospital;
	int numRequests;
	int numCancellations;
	int unAssignedEPCount;

public:
	//Member Functions
  
	//Constructor
	Organizer();
  
	/***** Input file member functions *****/
	void setInputFileName(UI gui) { filename = gui.getInputFileName(); }
	void setOutputFileName(UI gui) { outfile = gui.getOutputFileName(); }
	void processInputFile();					//Processes input file
	void generateOutputFile();                  //Generates output file
	void readHospitalData();					//Reads hospital distance data
	void AddHospital(const int Hospital_ID);	//Adding a Hospital to the hospital list

	/**** Functions for handling Cars **/
	void updateOutCars();
	void updateBackCars();
	void updateCheckupCars();
	void handleCarMovements(); //move from out to back and from back to hospitals
	void moveCarFromFreeToOut(Patient* patient); //move from free to out
	void outCarFailure();
	void outCarFailureAction(Car* car);
	void backCarFailure();
	void backCarFailureAction(Car* car);
	void addCarToCheckup(Car* car) { checkupList.enqueue(car, checkupTime); }
	void hospitalFaliure();
	void hospitalFailureAction(Hospital* failedHospital);

	void handleCancellations();
	void addToFinishedList(Car* car);
	void transferPatientsRequests(Patient* patient, int nearestHospitalID);
	bool handleEP(Patient* patient,Hospital* hospital);

	~Organizer();
};

Organizer::Organizer() :
	timeStep(0),
	HospitalList(nullptr),
	numHospitals(0),
	speedScars(0),
	speedNcars(0),
	distanceMatrix(nullptr),
	scarsPerHospital(nullptr),
	ncarsPerHospital(nullptr),
	numRequests(0),
	numCancellations(0),
	outCarsFailureProbability(0),
	backCarsFailureProbability(0),
	hospitalFailureProbability(0),
	checkupTime(0),
	unAssignedEPCount(0),
	numOfFailedHospitals(0),
	numOfOutOfServiceCars(0),
	numOfOutOfServiceSC(0),
	numOfOutOfServiceNC(0)
	checkupTime(0),
	NCFailuresBack(0),
	NCFailuresOut(0),
	SCFailuresBack(0),
	SCFailuresOut(0)
{
}

/***** FILE LOADING FUNCTIONS *****/

/* The processInputFile function loads, reads and processes the input file 
containing data related to the hospitals, patient requests, and cancellations. It then either calls
the respective functions to store the data in appropriate data structures or stores the latter itself */

void Organizer::processInputFile()
{
	setInputFileName(GUI);

	ifstream inputFile;
	inputFile.open(filename + ".txt", ios::in);
	if (!inputFile.is_open())
	{
		return;
	}

	//Read the number of hospitals (the first line)
	inputFile >> numHospitals;

	//Read the speeds of SCars and Ncars (the second line)
	inputFile >> speedScars >> speedNcars;

	//Read SC & NC cars checkup time
	inputFile >> checkupTime;

	// Read failure probabilities
	inputFile >> outCarsFailureProbability >> backCarsFailureProbability >> hospitalFailureProbability; 

	// Read the hospital matrix (numHospitals x numHospitals)
	distanceMatrix = new int* [numHospitals];
	for (int i = 0; i < numHospitals; ++i)
	{
		distanceMatrix[i] = new int[numHospitals];
	}
	for (int i = 0; i < numHospitals; i++)
	{
		for (int j = 0; j < numHospitals; j++)
		{
			inputFile >> distanceMatrix[i][j];
		}
	}
	// Read the number of SCars and NCars available for each Hospital
	scarsPerHospital = new int[numHospitals];  // SCars
	ncarsPerHospital = new int[numHospitals];  // NCars
	for (int i = 0; i < numHospitals; i++)
	{
		inputFile >> scarsPerHospital[i] >> ncarsPerHospital[i];
	}

	// Read number of patient requests
	inputFile >> numRequests;

	//Read all patients' requests list
	for (int i = 0; i < numRequests; i++)
	{
		string type;
		int requestTime, patientID, nearestHospitalID, distanceToHospital, caseSeverity;
		inputFile >> type;
		Patient* patient = nullptr; // Pointer to a Patient object

		if (type == "NP")
		{
			inputFile >> requestTime >> patientID >> nearestHospitalID >> distanceToHospital;

			// Dynamically create a Normal Patient (NP)
			patient = new Patient(patientID, requestTime, nearestHospitalID, distanceToHospital, NP);
		}
		else if (type == "SP")
		{
			inputFile >> requestTime >> patientID >> nearestHospitalID >> distanceToHospital;

			// Dynamically create a Special Patient (SP)
			patient = new Patient(patientID, requestTime, nearestHospitalID, distanceToHospital, SP);
		}
		else if (type == "EP")
		{
			inputFile >> requestTime >> patientID >> nearestHospitalID >> distanceToHospital >> caseSeverity;

			// Dynamically create an Emergency Patient (EP) with case severity
			patient = new Patient(patientID, requestTime, nearestHospitalID, distanceToHospital, EP, caseSeverity);
		}

		// Enqueue the patient pointer into the queue
		patientsList.enqueue(patient);
	}

	// Read number of cancellations
	inputFile >> numCancellations;

	//Reads request cancellation list
	for (int i = 0; i < numCancellations; i++)
	{
		int PID, hospitalID, cancellationTimestep;
		// Parse the cancellation request
		inputFile >> PID >> hospitalID >> cancellationTimestep;

		// Create a CancellationReq struct
		CancellationReq cancellation = { PID, hospitalID, cancellationTimestep };

		// Enqueue the cancellation request into the CancellationList
		CancellationList.enqueue(cancellation);
	}

	inputFile.close();

	// Call the readHospitalData function to process the loaded data and create the appropriate data structures
	readHospitalData();
}

void Organizer::AddHospital(const int Hospital_ID)
{
	int index = Hospital_ID - 1;

	// Ensure the index is within bounds
	if (index < 0 || index >= numHospitals)
	{
		return;
	}

	// Create a new Hospital object
	Hospital* newHospital = new Hospital();

	// Set the Hospital ID
	newHospital->setID(Hospital_ID);

	// Add the new Hospital to the HospitalList at the specified index
	HospitalList[index] = newHospital;
}

void Organizer::readHospitalData()
{
	// Dynamically allocate an array of pointers to Hospital objects
	HospitalList = new Hospital * [numHospitals];

	static int carID = 1; //For the Car ID's

	for (int i = 0; i < numHospitals; ++i)
	{
		AddHospital(i + 1);  // Add hospital with ID = i + 1 at index i

		// Reads the number of SCars and NCars for this hospital
		int scars, ncars;
		scars = scarsPerHospital[i];
		ncars = ncarsPerHospital[i];

		// Adds SCars to the hospital's SCList and NCars to the NCList
		// For each SCar, add it to the SCList
		for (int j = 0; j < scars; ++j)
		{
			Car* car = new Car(carID++, i + 1, SC, speedScars);
			HospitalList[i]->addCarToList(car);  // Adds to SCList or NCList based on car type
		}

		// For each NCar, add it to the NCList
		for (int j = 0; j < ncars; ++j) {
			Car* car = new Car(carID++, i + 1, NC, speedNcars);
			HospitalList[i]->addCarToList(car);  // Adds to SCList or NCList based on car type
		}
	}
}

/****** CAR HANDLING FUNCTIONS ******/

void Organizer::updateOutCars()
{
	priQueue<Car*> temp;
	Car* c;
	int pri;
	while (!OutCars.isEmpty())
	{
		OutCars.dequeue(c, pri);
		c->updateOut();
		if (c->getDistToPatient() == 0)
		temp.enqueue(c, -c->getDistToPatient());
	}
	while (!temp.isEmpty())
	{
		temp.dequeue(c, pri);
		OutCars.enqueue(c, pri);
	}
}

void Organizer::updateBackCars()
{
	priQueue<Car*> temp;
	Car* c;
	int pri;
	while (!BackCars.isEmpty())
	{
		BackCars.dequeue(c, pri);
		c->updateBack();
		temp.enqueue(c, -c->getDistToHospital());
	}
	while (!temp.isEmpty())
	{
		temp.dequeue(c, pri);
		BackCars.enqueue(c, pri);
	}
}

void Organizer::updateCheckupCars()
 {
	priQueue<Car*> temp;
	Car* c;
	int p;
	while (!checkupList.isEmpty())
	{
		checkupList.dequeue(c, p);
		p--;
		temp.enqueue(c, p);
	}
	while (!temp.isEmpty())
	{
		temp.dequeue(c, p);
		checkupList.enqueue(c, p);
	}
 }

void Organizer::moveCarFromFreeToOut(Patient* patient)
{
	if (!patient) { return; }

	int nearestHospitalID = patient->getNearestHospital();
	// Validate hospital ID to ensure the hospital exists
	if (nearestHospitalID < 1 || nearestHospitalID > numHospitals) { return; }

	Hospital* nearestHospital = HospitalList[nearestHospitalID - 1];
	Car* assignedCar = nullptr;
	bool assigned;
	// Attempt to assign a car to the patient using Hospital's patient & car assignment logic
	assigned = nearestHospital->assignPatientToCar(patient, assignedCar);

	// If a car was successfully assigned
	if (assigned)
	{
		OutCars.enqueue(assignedCar, -assignedCar->getDistToPatient()); // Add car to OUT cars queue
		return;
	}

	// Handle unassigned EP patients
	if (patient->getPatientType() == patientType::EP)
	{
		unAssignedEPCount++;
		handleEP(patient, nearestHospital); // Handle EP patient as no car was available
	}
}

void Organizer::handleCarMovements()
{
	Car* car;
	int priority;
	// Move cars from the OutCars queue to BackCars queue when they arrive at the patient's location (distance to the patient becomes 0)
	while (!OutCars.isEmpty() && OutCars.peek(car, priority) && car->getDistToPatient() == 0)
	{
		OutCars.dequeue(car, priority);
		car->getAssignedPatient()->setPickup(timeStep);
		car->getAssignedPatient()->setStopped(false);
		BackCars.enqueue(car, -car->getDistToHospital());//car added to BackCars,with a priority based on its distance to the hospital
         //Negative distance used to ensure cars closer to the hospital are prioritized (higher priority for shorter distances)	
	}

	// Process BackCars: return cars to hospitals if they have completed their task
	while (!BackCars.isEmpty() && BackCars.peek(car, priority) && car->getDistToHospital() == 0)
	{
		BackCars.dequeue(car, priority);
		if (car->getFailureBack() || car->getFailureOut())
		{
			car->setInCheckup(true);
			addCarToCheckup(car);
		}
		else
		{
			// Deassigns the patient from the car and adds the patient to the FinishedList
			addToFinishedList(car);
		}
	}

	// Process checkup list
	while (!checkupList.isEmpty() && checkupList.peek(car, priority) && priority == 0)
	{
		checkupList.dequeue(car, priority);
		car->setFailureBack(false);
		car->setFailureOut(false);
		car->setInCheckup(false);
		HospitalList[car->getHospital() - 1]->addCarToList(car);
	}
}

/****** FAILURE PROBABILITIES AND ACTIONS FUNCTIONS ******/

void Organizer::outCarFailure()
{
	if (OutCars.isEmpty()) return;

	// Generate a random number to determine failure
	double randomValue = (rand() % 100) / 100.0; // Random value between 0 and 1

	if (randomValue <= outCarsFailureProbability) 
	{
		Car* car = nullptr;
		int priority;

		randomValue = (rand() % OutCars.getCount());

		priQueue<Car*> temp;
		for (int i = 0; i < randomValue; i++)
		{
			OutCars.dequeue(car, priority);
			temp.enqueue(car, priority);
		}

		OutCars.dequeue(car, priority);
		outCarFailureAction(car);

		while (!temp.isEmpty())
		{
			temp.dequeue(car, priority);
			OutCars.enqueue(car, priority);
		}
	}
}

void Organizer::outCarFailureAction(Car* car)
{
	car->setFailureOut(true);
	HospitalList[car->getHospital()-1]->addFailurePatient(car->getAssignedPatient());
	BackCars.enqueue(car, -car->getDistToHospital());
	if (car->getCarType() == NC)
		NCFailuresOut++;
	else
		SCFailuresOut++;
}

void Organizer::backCarFailure()
{
	if (BackCars.isEmpty()) return;

	// Generate a random number to determine failure
	double randomValue = (rand() % 100) / 100.0; // Random value between 0 and 1

	if (randomValue <= backCarsFailureProbability)
	{
		Car* car = nullptr;
		int priority;

		randomValue = (rand() % BackCars.getCount());

		priQueue<Car*> temp;
		for (int i = 0; i < randomValue; i++)
		{
			BackCars.dequeue(car, priority);
			temp.enqueue(car, priority);
		}

		BackCars.dequeue(car, priority);
		backCarFailureAction(car);

		while (!temp.isEmpty())
		{
			temp.dequeue(car, priority);
			BackCars.enqueue(car, priority);
		}
	}
}

void Organizer::backCarFailureAction(Car* car)
{
	car->setFailureBack(true);
	car->getAssignedPatient()->setDistanceToPickup(car->getDistToHospital());
	car->getAssignedPatient()->setStopped(true);
	HospitalList[car->getHospital() - 1]->addFailurePatient(car->getAssignedPatient());
	BackCars.enqueue(car, -car->getDistToHospital());
	if (car->getCarType() == NC)
		NCFailuresBack++;
	else
		SCFailuresBack++;
}

void Organizer::hospitalFaliure()
{
	// Generate a random number to determine failure
	double randomValue = (rand() % 100) / 100.0; // Random value between 0 and 1

	if (randomValue <= hospitalFailureProbability)
	{
		// Randomly select a hospital to fail
		int failedHospitalID = rand() % numHospitals + 1;
		Hospital* failedHospital = HospitalList[failedHospitalID - 1];
		failedHospitalsList[numOfFailedHospitals] = failedHospital;
		numOfFailedHospitals++;
		hospitalFailureAction(failedHospital);
	}
}

void Organizer::hospitalFailureAction(Hospital* failedHospital)
{
	if (!failedHospital || failedHospital->isFailed()) { return; }

	// Mark the hospital as failed
	failedHospital->setFailed(true);

	// Find the nearest hospital that is not failed
	int secondNearestHospitalID = -1;
	int minDistance = -1;

	// Loop through the distance matrix to find the first valid distance
	for (int i = 0; i < numHospitals; i++)
	{
		if (i != failedHospitalID - 1 && !HospitalList[i]->isFailed())
		{
			secondNearestHospitalID = i + 1;
			minDistance = DistanceMatrix[failedHospitalID - 1][i];
		}
	}

	// Transfer SP, EP, and NP patients to the nearest hospitals
	Patient* patient;
	LinkedQueue<Patient*> SPList = failedHospital->transferSPList();
	priQueue<Patient*> EPList = failedHospital->transferEPList();
	ModifiedQ NPList = failedHospital->transferNPList();

	// Reassign SP patients
	while (!SPList.isEmpty())
	{
		SPList.dequeue(patient);
		transferPatientsRequests(patient, secondNearestHospitalID);
	}

	// Reassign EP patients
	while (!EPList.isEmpty())
	{
		int priority; 
		EPList.dequeue(patient, priority); 
		transferPatientsRequests(patient, secondNearestHospitalID);
	}

	// Reassign NP patients
	while (!NPList.isEmpty())
	{
		NPList.dequeue(patient); 
		transferPatientsRequests(patient, secondNearestHospitalID);
	}

	// Remove all free cars (both SC and NC) from the system
	Car* car;
	LinkedQueue<Car*> SCList = failedHospital->transferSCList();
	LinkedQueue<Car*> NCList = failedHospital->transferNCList();

	while (!SCList.isEmpty())
	{
		SCList.dequeue(car);
		numOfOutOfServiceSC++;
		delete car;
	}

	while (!NCList.isEmpty())
	{
		NCList.dequeue(car);
		numOfOutOfServiceNC++;
		delete car;
	}
	numOfOutOfServiceCars = numOfOutOfServiceSC + numOfOutOfServiceNC;

	// Handle OUT cars of the failed hospital
	priQueue<Car*> tempOutCars;
	Car* outCar = nullptr;
	int priorityOut;

	while (!OutCars.isEmpty())
	{
		OutCars.dequeue(outCar, priorityOut);
		if (outCar->getHospital() == failedHospital->getHospitalID())
		{
			// Handle the failure of the assigned car
			outCarFailureAction(outCar);
			delete outCar; // Remove the car from the system
		}
		else
		{
			tempOutCars.enqueue(outCar, priorityOut);
		}
	}

	// Restore remaining OUT cars
	while (!tempOutCars.isEmpty())
	{
		tempOutCars.dequeue(outCar, priorityOut);
		OutCars.enqueue(outCar, priorityOut);
	}

	// Handle BACK cars of the failed hospital
	priQueue<Car*> tempBackCars;
	Car* backCar = nullptr;
	int priorityBack;
	while (!BackCars.isEmpty())
	{
		BackCars.dequeue(backCar, priorityBack);
		if (backCar->getHospital() == failedHospital->getHospitalID())
		{
			// Handle the failure of the assigned car
			backCarFailureAction(backCar);
			delete backCar; // Remove the car from the system
		}
		else
		{
			tempBackCars.enqueue(backCar, priorityBack);
		}
	}

	// Restore remaining BACK cars
	while (!tempBackCars.isEmpty())
	{
		tempBackCars.dequeue(backCar, priorityBack);
		BackCars.enqueue(backCar, priorityBack);
	}

	// Remove the hospital from the HospitalList and shift the remaining hospitals
	for (int i = 0; i < numHospitals; i++)
	{
		if (HospitalList[i] == failedHospital)
		{
			delete HospitalList[i]; // Free memory
			for (int j = i; j < numHospitals - 1; j++)
			{
				HospitalList[j] = HospitalList[j + 1]; // Shift hospitals to the left
			}
			HospitalList[numHospitals - 1] = nullptr; // Nullify the last pointer
			numHospitals--; // Decrease the total count of hospitals
			break;
		}
	}
}

void Organizer::transferPatientsRequests(Patient* patient, int nearestHospitalID)
{
	if (!patient) { return; }

	// Assign the patient to the new nearest hospital
	Hospital* nearestHospital = HospitalList[nearestHospitalID - 1];
	nearestHospital->addPatientToList(patient);
}

/****** CANCELLATION REQUESTS HANDLING FUNCTION ******/

void Organizer::handleCancellations()
{
	CancellationReq cr;

	while (CancellationList.peek(cr) && cr.CancellationTimestep == timeStep)
	{
		CancellationList.dequeue(cr);
		Hospital* hospital = HospitalList[cr.hospitalID - 1];

		// Verifies that the Patient exists in its corresponding hospital's NP List
		if (!hospital->isPatientInNPList(cr.PID)) { return; }

		// Remove the patient from the system
		bool removed = patientsList.cancelRequest(cr.PID);
		numRequests = patientsList.getCount();

		// Search for the car in the OutCars list then dequeue it if found
		Car* assignedCar = nullptr; 
		bool carFound = OutCars.cancelRequest(cr.PID, assignedCar);

		if (carFound)
		{
			hospital->cancelRequest(cr.PID);
			BackCars.enqueue(assignedCar, -assignedCar->getDistToHospital());
		}
	}
}

/****** ADD FINISHED PATIENTS TO FINISHED LIST FUNCTION *****/

void Organizer::addToFinishedList(Car* car)
{
	Patient* p = car->deassignPatient();
	p->setFinished(timeStep);
	FinishedList.enqueue(p);
	HospitalList[car->getHospital() - 1]->addCarToList(car);
}

/****** GENERATE & UPDATE OUTPUT FILE FUNCTION *****/

void Organizer::generateOutputFile()
{
	setOutputFileName(GUI);

	ofstream OutputFile;
	OutputFile.open(outfile + ".txt", ios::out);

	Patient* tempItem;
	LinkedQueue<Patient*> tempList;
	ModifiedQ temp;

	// Writinf the Finished Patients List
	int FT, PID, QT, WT;
	OutputFile >> "FT" >> "\t" >> "PID" >> "\t" >> "QT" >> "\t" >> "WT" >> '\n';
	while (!FinishedList.isEmpty())
	{
		FinishedList.dequeue(tempItem);

		FT = tempItem->getFinishTime();
		PID = tempItem->getPatientID();
		QT = tempItem->getRequestTime();
		WT = tempItem->getWaitTime();
		tempList.enqueue(tempItem);
		OutputFile << FT << "\t" << PID << "\t" << QT << "\t" << WT << '\n\n';
	}
	//Restore the finished patients list
	while (!tempList.isEmpty())
	{
		tempList.dequeue(tempItem);
		FinishedList.enqueue(tempItem);
	}

	OutputFile >> "============== System Statistics ==============" >> '\n';

	// Caculating and writing the statistics
	//Writing the  Total number of patients and number of patients of each type in the system
	int totalSP = 0, totalNP = 0, totalEP = 0;
	for (int i = 0; i < numRequests; i++)
	{
		patientsList.dequeue(tempItem);

		if (tempItem->getPatientType() == SP) totalSP++;
		if (tempItem->getPatientType() == NP) totalNP++;
		if (tempItem->getPatientType() == EP) totalEP++;

		temp.enqueue(tempItem);
	}
	//Restore the patients list
	while (!temp.isEmpty())
	{
		temp.dequeue(tempItem);
		patientsList.enqueue(tempItem);
	}

	OutputFile >> "Patients: " >> numRequests >> "\t" >> "[NP: " >> totalNP >> ", SP: " >> totalSP >> ", EP: " >> totalEP >> "]" >> '\n';

	//Writing the total number of hospitals in the system
	OutputFile >> "Hospitals = " >> numHospitals >> '\n';

	//Writing the total number of cars and number of cars of each type in the system
	int totalCars = 0, totalSC = 0, totalNC = 0;
	for (int i = 0; i < numHospitals; i++)
	{
		totalSC += HospitalList[i]->getSCarsCount();
		totalNC += HospitalList[i]->getNCarsCount();
	}
	totalCars = totalSC + totalNC;
	OutputFile >> "Cars: " >> totalCars >> "\t" >> "[SCars: " >> totalSC >> ", NCars: " >> totalNC >> "]" >> '\n';

	//Calculating and writing the average waiting time for patients
	int totalWaitingTime = 0, avgWaitingTime = 0;
	while (!FinishedList.isEmpty())
	{
		FinishedList.dequeue(tempItem);
		totalWaitingTime += tempItem->getWaitTime();
		tempList.enqueue(tempItem);
	}
	//Restore the finished patients list
	while (!tempList.isEmpty())
	{
		tempList.dequeue(tempItem); 
		FinishedList.enqueue(tempItem);
	}

	if (FinishedList.getCount() != 0)
	{
		avgWaitingTime = totalWaitingTime / FinishedList.getCount();
	}
	else { avgWaitingTime = 0; }
	OutputFile >> "Average waiting time = " >> avgWaitingTime >> '\n';

	//Calculating and writing Percentage of EP (relative to the total number of EP) who couldn't be served by home hospital
	double percentage = ((static_cast<double>(unAssignedEPCount) / totalEP) * 100.0);
	OutputFile >> "Percentage of EP who couldn't be served by home hospital = " >> percentage >> "%" >> '\n';

	//Calculating and writing the average busy time of all cars in the system
	int totalBusyTime = 0, avgBusyTime = 0;
	while (!FinishedList.isEmpty())
	{
		FinishedList.dequeue(tempItem);
		totalBusyTime += tempItem->getBusyTime();
		tempList.enqueue(tempItem);
	}
	// Restore the original finished patients list
	while (!tempList.isEmpty())
	{
		tempList.dequeue(tempItem);
		FinishedList.enqueue(tempItem);
	}

	if (FinishedList.getCount() != 0)
	{
		avgBusyTime = totalBusyTime / FinishedList.getCount();
	}
	else { avgBusyTime = 0; }
	OutputFile >> "Average busy time = " >> avgBusyTime >> '\n';

	//Calculating and writing Average Utilization Percentage
	double avgUtilizationTime = ((static_cast<double>(avgBusyTime) / timeStep) * 100.0);
	OutputFile >> "Average Utilization Percentage = " >> avgUtilizationTime >> "%" >> '\n\n';

	//Writing the number of failed cars and their failure percentages (out and back cars)


	//Writing the number of failed hospitals and their failure percentages
	OutputFile >> "Number of failed hospitals = " >> numOfFailedHospitals >> ", Hospital Failure Percentage = " >> hospitalFailureProbability * 100 >> "%" >> '\n\n';
	
	//Writing each hospital's number of NP, SP, EP patients at the failure timeStep
	OutputFile >> "List of failed hospitals:" >> '\n' >> "HID" >> "\t" >> "NP COUNT" >> "\t" >> "SP COUNT" >> "\t" >> "EP COUNT" >> "\t" >> "FREE SC" >> "\t" >> "FREE NC" >> '\n';
	for (int i = 0; i < numOfFailedHospitals; i++)
	{
		OutputFile >> failedHospitalsList[i]->getHospitalID() >> "\t" >> failedHospitalsList[i]->getNPListCount() >> "\t";
		OutputFile >> failedHospitalsList[i]->getSPListCount() >> "\t" >> failedHospitalsList[i]->getEPListLength() >> "\t";
		OutputFile >> failedHospitalsList[i]->getSCarsCount() >> "\t" >> failedHospitalsList[i]->getNCarsCount() >> '\n';
	}
	OutputFile >> '\n';

	//Writing the number of cars that went out of service due to hospital failure (SC, NC, and total)
	OutputFile >> "Total number of cars that are out of service due to hospital failure = " >> numOfOutOfServiceCars >> '\n';
	OutputFile >> "[SCars: " >> numOfOutOfServiceSC >> ", NCars: " >> numOfOutOfServiceNC >> "]" >> '\n';

	OutputFile << "============== End of the Output File ==============" << '\n';

	// Close the file
	OutputFile.close();
}


bool Organizer::handleEP(Patient* patient, Hospital* hospital)
{
	int shortestList = hospital->getEPListLength();
	int nearestHospitalDistance = patient -> getDistance();
	int distanceBetHospitals = patient->getDistance();

	if (!hospital->addPatientToList(patient) && patient->getPatientType() == EP)
	{
		for (int i = 0; i < numHospitals; i++)
		{
			if (shortestList > HospitalList[i]->getEPListLength())
			{
				shortestList = HospitalList[i]->getEPListLength();
				hospital->setID(HospitalList[i]->getHospitalID());
				hospital->addPatientToList(patient);
				return true;
			}
			else if (shortestList == HospitalList[i]->getEPListLength())
			{
				if (nearestHospitalDistance > distanceMatrix[HospitalList[i]->getHospitalID()][i])
				{
					distanceBetHospitals = nearestHospitalDistance - distanceMatrix[HospitalList[i]->getHospitalID()][i];
					nearestHospitalDistance = distanceMatrix[HospitalList[i]->getHospitalID()][i];
					hospital->setID(HospitalList[i]->getHospitalID());
					patient->setDistanceToPickup(distanceBetHospitals);
					hospital->addPatientToList(patient);
					return true;
				}
			}
		}
	}
	return false;
}


Organizer::~Organizer()
{
	// Cleanup dynamically allocated memory
	for (int i = 0; i < numHospitals; ++i)
	{
		delete[] distanceMatrix[i];
	}
	delete[] distanceMatrix;
	delete[] scarsPerHospital;
	delete[] ncarsPerHospital;

	// Cleanup for the hospitalArray
	for (int i = 0; i < numHospitals; ++i)
	{
		delete HospitalList[i];  // Delete each individual Hospital object
	}
	delete[] HospitalList;  // Delete the array of Hospital pointers

	// Dequeue all patients and delete each dynamically allocated Patient object
	Patient* tempPatient;
	while (!patientsList.isEmpty())
	{
		patientsList.dequeue(tempPatient);
		delete tempPatient;  // Free the memory allocated for the Patient object
	}

	// Dequeue all cancellation requests and delete each CancellationReq struct
	CancellationReq tempCancellation;
	while (!CancellationList.isEmpty())
	{
		CancellationList.dequeue(tempCancellation); // Dequeue each cancellation request
		// No need to explicitly delete tempCancellation since it's a struct
	}
}

#endif

