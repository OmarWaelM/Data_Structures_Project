#ifndef ORGANIZER_H
#define ORGANIZER_H

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
	LinkedQueue<Patient*> patientsList;				// Patients list of type Linked Queue (list of pointers to patients)
	LinkedQueue<CancellationReq> CancellationList;	// Cancellation requests' list of type Linked Queue
	LinkedQueue<Patient*> FinishedList;				// Finished patients' list of type Linked Queue
	priQueue<Car*> BackCars;						// Back cars' list (cars on their way back) of type Priority Queue
	ModifiedPriQ OutCars;							// Out cars' list (cars out on their way to pick up patients) of type Priority Queue (modified)
	priQueue<Car*> checkupList;						// Checkup cars list

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

	//File Loading data members (can be declared in file processing and freed at the end)
	string filename;//Keep
	string outfile; //Keep used when outputting results file
	int speedScars, speedNcars;
	int* scarsPerHospital;
	int* ncarsPerHospital;
	int numRequests;
	int numCancellations;

public:
	//Member Functions
  
	//Constructor
	Organizer();

	//Simulator
	void Simulator();
  
	/***** Input file member functions *****/
	void processInputFile();					//Processes input file
	void readHospitalData();					//Reads hospital distance data
	void AddHospital(const int Hospital_ID);	//Adding a Hospital to the hospital list

	/**** Functions for handling Card **/
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

	//create function to Assign all current patients from patientlist to hospital (code is in simulator)
	//create function to Perform all cancellation requests (code is in simulator)
	//create functino to Assign all possible patients from hospitals to out cars
	//Hamdle no EP
	//Hospital Failure
	//Hospital Failure action
	//Processing input file needs to get probabilities and checkup time

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
	NCFailuresBack(0),
	NCFailuresOut(0),
	SCFailuresBack(0),
	SCFailuresOut(0)
{
}

void Organizer::Simulator()
{
	//Initialization
	timeStep = 0;
	GUI.Start();
	filename = GUI.getInputFileName();
	processInputFile();

	Patient* p;
	Car* car;
	CancellationReq cr;
	bool endSimulation = false;
	int randomNum = 0;

	GUI.Output(timeStep, HospitalList, numHospitals, &BackCars, &OutCars, &FinishedList, &checkupList);

	while (!endSimulation)
	{
		//Updating timestep
		timeStep++;

		// 
		// update cars
		// check for returning cars
		// check for patient requests
		// assign patients to cars
		// check for car failure
		// check for hospital failure
		//

		//Checking for new patients
		while (patientsList.peek(p) && p->getRequestTime() == timeStep)
		{
			patientsList.dequeue(p);
			HospitalList[p->getNearestHospital() - 1]->addPatientToList(p);
		}

		//Checking for cancellation requests
		while (CancellationList.peek(cr) && cr.CancellationTimestep == timeStep)
		{
			CancellationList.dequeue(cr);
			HospitalList[cr.hospitalID - 1]->cancelRequest(cr.PID);
			//does not check the outcars list as no patient-car assignment occurs
		}

		updateOutCars();
		updateBackCars();
		updateCheckupCars();
		handleCarMovements();

		for (int i = 0; i < numHospitals; i++)
		{
			//Generating random number between 0 and 100
			randomNum = rand() % 100;

			if (randomNum >= 10 && randomNum < 20)
			{
				//Getting patient from sp list
				Patient* p = nullptr;
				if (HospitalList[i]->getSP(p))
					FinishedList.enqueue(p);
			}
			if (randomNum >= 20 && randomNum < 25)
			{
				//Getting patient from ep list
				Patient* p = nullptr;
				if (HospitalList[i]->getEP(p))
					FinishedList.enqueue(p);
			}
			if (randomNum >= 30 && randomNum < 40)
			{
				//Getting patient from np list
				Patient* p = nullptr;
				if (HospitalList[i]->getNP(p))
					FinishedList.enqueue(p);
			}
			if (randomNum >= 40 && randomNum < 45)
			{
				//Getting car from sc list
				Car* c = nullptr;
				if (HospitalList[i]->getSC(c))
					OutCars.enqueue(c, 1);
			}
			if (randomNum >= 70 && randomNum < 75)
			{
				//Getting car from nc list
				Car* c = nullptr;
				if (HospitalList[i]->getNC(c))
					OutCars.enqueue(c, 1);
			}
			if (randomNum >= 80 && randomNum < 90)
			{
				//Moving car from out to back cars list
				Car* c = nullptr;
				int pri;
				if (OutCars.dequeue(c, pri))
					BackCars.enqueue(c, pri);
			}
			if (randomNum >= 90 && randomNum < 95)
			{
				//Moving car from back cars list to hospital
				Car* c = nullptr;
				int pri;
				if (BackCars.dequeue(c, pri))
				{
					int cid = c->getHospital();
					HospitalList[cid - 1]->addCarToList(c);
				}
			}
		}
    
		//Output hospital data
		GUI.Output(timeStep, HospitalList, numHospitals, &BackCars, &OutCars, &FinishedList, &checkupList);

		//Checking if all lists are empty
		endSimulation = true;
		if (!patientsList.isEmpty())
			endSimulation = false;
		for (int i = 0; i < numHospitals; i++)
		{
			if (!HospitalList[i]->isEmpty())
				endSimulation = false;
		}
	}


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
			Patient* p = car->deassignPatient();
			p->setFinished(timeStep);
			FinishedList.enqueue(p);
			HospitalList[car->getHospital() - 1]->addCarToList(car);
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

//wrong implementation
void Organizer::moveCarFromFreeToOut(Patient* patient)
{
	 Car* car = nullptr;

	 // Assign a car to the patient (priority: SC > NC)
	 if (HospitalList[patient->getNearestHospital() - 1]->getSC(car) ||
		 HospitalList[patient->getNearestHospital() - 1]->getNC(car)) //-1 as the array is 1_based indexed
	 {
		 // Link the car to the patient
		 car->AssignPatient(patient);

		 // Calculate priority for OutCars based on distance to the patient
		 int priority = -car->getDistToPatient();

		 // Move the car to the OutCars queue
		 OutCars.enqueue(car, priority);
		 cout << "Car " << car->getcarID() << " assigned to Patient " << patient->getPatientID() << " and moved to OutCars.\n";
	 }
	 else
	 {
		 cout << "No available car for Patient " << patient->getPatientID() << ".\n";
	 }
 }

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

