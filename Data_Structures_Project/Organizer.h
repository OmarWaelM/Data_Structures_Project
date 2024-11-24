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

struct CancellationReq
{
	int PID;
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

    void processInputFile(const string& filename);

	Hospital* getHospital(int ID)
	{
		// Ensure ID is within bounds
		if (ID < 0 || ID >= numHospitals) {
			return nullptr;  // Return nullptr if ID is invalid
		}
		return HospitalList[ID];  // Return the pointer to the hospital object at index ID
	}
    LinkedQueue<Patient*>* getFinishedList() { return &FinishedList; }
	// Getter for hospital list (if needed)
	Hospital** getHospitalList() { return HospitalList; }

	// Getter for number of hospitals
	int getNumHospitals() { return numHospitals; }

    //Adding a Back Car based on its priority
    void AddBackCar(const string& Car, int Priority);

    //Removing the highest priority from the Back_Cars queue
    bool RemoveBackCar(string& Car);

    //Adding a Hospital to the hospital list
    void AddHospital(const string& Hospital_Name);

    //Printing out the hospitals in the hospital list
    void PrintHospitals()const;

    //Adding an entry to the finished list
    void AddFinished(const string& Finished_Entry);

    //Removing an entry from the finished list
    bool RemoveFinished(string& Finished_Entry);

    //Print items in Finished List
    void DisplayFinishedList()const;

    /***** Input file member functions *****/

    //Reads hospital distance data
    void readHospitalData(ifstream& inputfile);

    // Reads the available car data
    void readCarData(ifstream& inputfile);

    // Reads patient request list
    void readPatientRequests(ifstream& inputfile);

    //// Reads request cancellation list
    void readCancellationRequests(ifstream& inputfile);

    ~Organizer();

};

Organizer::Organizer() :GUI(this)
{
    GUI.Start();
}

void Organizer::processInputFile(const string& filename)
{
    ifstream inputFile(filename);
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

void Organizer::readHospitalData()
{
	// Dynamically allocate an array of pointers to Hospital objects
	HospitalList = new Hospital * [numHospitals];

	static int carID = 1; //For the Car ID's

	for (int i = 0; i < numHospitals; ++i)
	{
		// Creates a new Hospital object for each hospital
		HospitalList[i] = new Hospital();

		// Sets the hospital ID
		HospitalList[i]->setID(i + 1);

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

		HospitalList[i]->setDistanceMatrix(distanceMatrix, numHospitals);
	}
}

void Organizer::readCarData(ifstream& cFile)
{
    
}

void Organizer::readPatientRequests(ifstream& pFile)
{
   
}

void Organizer::readCancellationRequests(ifstream& cFile)
{
    
}

Organizer::~Organizer()
{
    delete[] HospitalList;
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