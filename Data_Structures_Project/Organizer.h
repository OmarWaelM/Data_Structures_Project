#ifndef ORGANIZER_H
#define ORGANIZER_H
using namespace std;
#include "UI.h"
#include "Hospital.h"
#include "Patient.h"
#include "Car.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ModifiedPriQ.h"

using namespace std;
#include<iostream>
#include <string>
#include <fstream>
#include <iomanip>

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
	LinkedQueue<Patient*> AllPatientsList;
	LinkedQueue<CancellationReq> CancellationList;
	LinkedQueue<Patient*> FinishedList;
	priQueue<Car*> BackCars;
	ModifiedPriQ<Car*> OutCars;

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
	string* patientRequests;
	int numCancellations;
	string* cancellations;


public:
	//Member Functions

	//Constructor
	Organizer();
	void processInputFile();
	void Simulator();

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

	//Printing out the hospitals in the hospital list
	void PrintHospitals()const;

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

	// Reads the available car data
	void readCarData();

	// Reads patient request list
	void readPatientRequests();

	//// Reads request cancellation list
	void readCancellationRequests();

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
	patientRequests(nullptr),
	numCancellations(0),
	cancellations(nullptr)
{
	GUI.Start();
	filename = GUI.getInputFileName();
}

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

	// Read each request and store it in a dynamic array
	patientRequests = new string[numRequests];
	for (int i = 0; i < numRequests; i++)
	{
		getline(inputFile, patientRequests[i]);
	}

	// Read number of cancellations
	inputFile >> numCancellations;

	cancellations = new string[numCancellations];
	for (int i = 0; i < numCancellations; i++)
	{
		getline(inputFile, cancellations[i]);
	}
	inputFile.close();

}

void Organizer::Simulator()
{
	timeStep = 0;
	GUI.Start();
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

		HospitalList[i]->setDistanceMatrix(distanceMatrix, numHospitals);
	}
}

void Organizer::PrintHospitals() const
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

void Organizer::readCarData()
{



} 

void Organizer::readPatientRequests()
{

}

void Organizer::readCancellationRequests()
{

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
	delete[] patientRequests;
	delete[] cancellations;

	// Cleanup for the hospitalArray
	for (int i = 0; i < numHospitals; ++i)
	{
		delete HospitalList[i];  // Delete each individual Hospital object
	}
	delete[] HospitalList;  // Delete the array of Hospital pointers
}


#endif

