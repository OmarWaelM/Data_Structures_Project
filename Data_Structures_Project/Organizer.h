#ifndef ORGANIZER_H
#define ORGANIZER_H
#include "Car.h"
#include "UI.h"
#include "Hospital.h"
using namespace std;

#include<iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>

class UI;

struct CancellationReq
{
	int PID;
	int hospitalID;
	int CancellationTimestep;
};

class Organizer
{
private:
	//Lists used in orgranizer class
	LinkedQueue<Patient*> patientsList;  // Patients list queue (list of pointers to patients)
	LinkedQueue<CancellationReq> CancellationList;
	LinkedQueue<Patient*> FinishedList;
	priQueue<Car*> BackCars;
	ModifiedPriQ OutCars;

	//General data members
	int timeStep;
	UI GUI;
	string filename;

	Hospital** HospitalList; //An array of pointers to hospitals
	int numHospitals;
	int speedScars, speedNcars;
	int** distanceMatrix;
	int* scarsPerHospital;
	int* ncarsPerHospital;
	int numRequests;
	int numCancellations;


public:
	//Member Functions
  
	//Constructor
	Organizer();
	void processInputFile();
	void Simulator();
  
  
  Hospital* getNearestHospital(Patient* EP);
  bool addEPtoHospital(Patient* EP);
  
	// Functions for managing Back Cars:
	//Adding a Back Car based on its priority
	//void AddBackCar(const string& Car, int Priority);

  //Removing the highest priority from the Back_Cars queue
	//bool RemoveBackCar(string& Car);

	LinkedQueue<Patient*>* getFinishedList() { return &FinishedList; }

	//Functions for managing hospital list:

	// Getter for hospital list (if needed)
	Hospital** getHospitalList() { return HospitalList; }

	// Getter for number of hospitals
	int getNumHospitals() const { return numHospitals; }

	Hospital* getHospital(int ID)
	{
		// Ensure ID is within bounds
		if (ID < 0 || ID > numHospitals) {
			return nullptr;  // Return nullptr if ID is invalid
		}
		return HospitalList[ID];  // Return the pointer to the hospital object at index ID
	}

	//Adding a Hospital to the hospital list
	void AddHospital(const int Hospital_ID);

	//Printing out the hospitals' information as shown in the sample output file
	void printHospitals()const;

	// functions for managing finished lists
	//Adding an entry to the finished list
	//void AddFinished(const string& Finished_Entry);

	//Removing an entry from the finished list
	//bool RemoveFinished(string& Finished_Entry);

	//Print items in Finished List
	//void DisplayFinishedList()const;

	/***** Input file member functions *****/

	//Reads hospital distance data
	void readHospitalData();
	//Printing out the hospitals in the hospital list
	void PrintHospitalsList()const;
	void printPatientsList()const;
	void printCancellationList()const;

	// Functions for handling Out Cars
	//void handleCancellations();
	void handleCarMovements();

	~Organizer();
};

Organizer::Organizer() :
	GUI(),
	timeStep(0),
	HospitalList(nullptr),
	numHospitals(0),
	speedScars(0),
	speedNcars(0),
	distanceMatrix(nullptr),
	scarsPerHospital(nullptr),
	ncarsPerHospital(nullptr),
	numRequests(0),
	numCancellations(0)

{
}

/***** FILE LOADING FUNCTION *****/

/* The processInputFile function loads, reads and processes the input file 
containing data related to the hospitals, patient requests, and cancellations. It then either calls
the respective functions to store the data in appropriate data structures or stores the latter itself */

void Organizer::processInputFile()
{
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

void Organizer::Simulator()
{
	timeStep = 0;
	GUI.Start();
	filename = GUI.getInputFileName();
	processInputFile();

	Patient* p;
	while (patientsList.dequeue(p))
	{
		int hid = p->getNearestHospital();
		HospitalList[hid - 1]->addPatientToList(p);
	}

	bool endSimulation = false;
	int randomNum = 0;
	while (!endSimulation)
	{
		timeStep++;
		for (int i = 0; i < numHospitals; i++)
		{
			randomNum = rand() % 100;

			if (randomNum >= 10 && randomNum < 20)
			{
				Patient* p = nullptr;
				if (HospitalList[i]->getSP(p))
					FinishedList.enqueue(p);
			}
			if (randomNum >= 20 && randomNum < 25)
			{
				Patient* p = nullptr;
				if (HospitalList[i]->getEP(p))
					FinishedList.enqueue(p);
			}
			if (randomNum >= 30 && randomNum < 40)
			{
				Patient* p = nullptr;
				if (HospitalList[i]->getNP(p))
					FinishedList.enqueue(p);
			}
			if (randomNum >= 40 && randomNum < 45)
			{
				Car* c = nullptr;
				if (HospitalList[i]->getSC(c))
					OutCars.enqueue(c, 1);
			}
			if (randomNum >= 70 && randomNum < 75)
			{
				Car* c = nullptr;
				if (HospitalList[i]->getNC(c))
					OutCars.enqueue(c, 1);
			}
			if (randomNum >= 80 && randomNum < 90)
			{
				Car* c = nullptr;
				int pri;
				if (OutCars.dequeue(c, pri))
					BackCars.enqueue(c, pri);
			}
			if (randomNum >= 90 && randomNum < 95)
			{
				Car* c = nullptr;
				int pri;
				if (BackCars.dequeue(c, pri))
				{
					int cid = c->getHospital();
					HospitalList[cid - 1]->addCarToList(c);
				}
			}
		}
		GUI.Output(timeStep, HospitalList, numHospitals, &BackCars, &OutCars, &FinishedList);

		endSimulation = true;
		for (int i = 0; i < numHospitals; i++)
		{
			if (!HospitalList[i]->empty())
				endSimulation = false;
		}
	}


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

		HospitalList[i]->setSCarsCount(scars);
		HospitalList[i]->setNCarsCount(ncars);

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

void Organizer::printHospitals() const
{
	for (int i = 0; i < numHospitals; ++i)
	{
		cout << *HospitalList[i]; // Use the overloaded << operator for Hospital class
	}
}

void Organizer::handleCarMovements()
{
	Car* car;
	int cp;
	// Process OutCars: move cars to BackCars if they have arrived
	while (!OutCars.isEmpty() && OutCars.peek(car,cp) && car->getArrivalTime() == timeStep)
	{
		int priority;
		OutCars.dequeue(car, priority);
		car->pickupPatient(); // Perform patient pickup
		BackCars.enqueue(car, car->getPriority());
	}

	// Process BackCars: return cars to hospitals if they have completed their task
	int priority;
	while (!BackCars.isEmpty() && BackCars.peek(car, priority) && car->getReturnTime() == timeStep)
	{
		BackCars.dequeue(car, priority);
		// Handle returning the car to its hospital
		// HospitalList[car->getHospitalID()].handleReturningCar(car);

	}
}

void Organizer::PrintHospitalsList() const
{
	// Check if HospitalList is initialized
	if (!HospitalList)
	{
		cout << "No hospitals available to display.\n";
		return;
	}

	// Iterate through the HospitalList and print details of each hospital
	for (int i = 0; i < numHospitals; ++i)
	{
		cout << "Hospital " << i + 1 << ":\n";

		// Access and display hospital details
		cout << "  Hospital ID: " << HospitalList[i]->getHospitalID() << "\n";
		cout << "  SCars: " << HospitalList[i]->getSCarsCount() << "\n";
		cout << "  NCars: " << HospitalList[i]->getNCarsCount() << "\n";
		cout << "----------------------------------------\n";
	}

	cout << "Distance Matrix:\n";
	for (int i = 0; i < numHospitals; ++i)
	{
		for (int j = 0; j < numHospitals; ++j)
		{
			// Adjust width for uniform spacing
			cout << setw(5) << distanceMatrix[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "----------------------------------------\n";
}

void Organizer::printPatientsList() const {
	if (patientsList.isEmpty()) {
		cout << "The patients list is empty." << endl;
		return;
	}

	cout << "Patients List:" << endl;

	// Create a copy of the patientsList to traverse without modifying it
	LinkedQueue<Patient*> tempQueue = patientsList;
	Patient* tempPatient;

	while (!tempQueue.isEmpty())
	{
		tempQueue.peek(tempPatient); // Get the front patient

		// Print patient details using the getter methods
		cout << "Patient ID: " << tempPatient->getPatientID() << endl;
		cout << "Patient Type: " << (tempPatient->getPatientType() == NP ? "Normal Patient" :
			tempPatient->getPatientType() == SP ? "Special Patient" : "Emergency Patient") << endl;
		cout << "Nearest Hospital ID: " << tempPatient->getNearestHospital() << endl;
		cout << "Distance to Hospital: " << tempPatient->getDistance() << endl;
		if (tempPatient->getPatientType() == EP)
		{
			cout << "Case Severity: " << tempPatient->getPatientPriority() << endl;
		}
		cout << "----------------------------------------\n";
		tempQueue.dequeue(tempPatient); // Remove the front patient
	}
}

void Organizer::printCancellationList() const {
	if (CancellationList.isEmpty()) {
		cout << "The cancellation list is empty." << endl;
		return;
	}

	cout << "Cancellation List:" << endl;

	// Create a copy of the CancellationList to traverse without modifying it
	LinkedQueue<CancellationReq> tempQueue = CancellationList;
	CancellationReq tempCancellation;

	while (!tempQueue.isEmpty()) {
		tempQueue.peek(tempCancellation); // Get the front cancellation request

		// Print cancellation request details
		cout << "Patient ID: " << tempCancellation.PID
			<< ", Hospital ID: " << tempCancellation.hospitalID
			<< ", Cancellation Time: " << tempCancellation.CancellationTimestep << endl;

		tempQueue.dequeue(tempCancellation); // Remove the front cancellation request
	}
}


Hospital* Organizer::getNearestHospital(Patient* EP)
{
    return nullptr;
}

bool Organizer::addEPtoHospital(Patient* EP)
{
    Hospital* hospital = EP->getNearestHospital();
    if (!(hospital->assignPatientToCar()))
    {
        //get the shortest hospital list
        //get nearest hospital to the current one
        return true;
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

