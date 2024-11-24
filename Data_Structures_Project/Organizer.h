#pragma once
#include "UI.h"
#include "Patient.h"
#include "Car.h"
#include "Hospital.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ModifiedQ.h"
#include "ModifiedPriQ.h"

using namespace std;
#include<iostream>
#include <vector>
#include <string>
#include <utility>
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
	Hospital HospitalList;
	int* DistancesMatrix;
	//General data members
	int timeStep;
	UI GUI;

public:
	//Member Functions
    
    //Constructor
    Organizer();

    void processInputFile(const string& filename);

    Hospital* getHospital(int ID) { return &HospitalList[ID]; }
    LinkedQueue<Patient*>* getFinishedList() { return &FinishedList; }

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
	int numHospitals;
	inputFile >> numHospitals;

	//Read the speeds of SCars and Ncars (the second line)
	int speedScars, speedNcars;
	inputFile >> speedScars >> speedNcars;

	//Read the hospital matrix (numHospitals x numHospitals)
	vector<vector<int>> distanceMatrix(numHospitals, vector<int>(numHospitals));
	for (int i = 0; i < numHospitals; i++)
	{
		for (int j = 0; j < numHospitals; j++)
		{
			inputFile >> distanceMatrix[i][j];
		}
	}

	//Read the number of Scars and Ncars available for each Hospital
	vector<pair<int, int>> carsPerHospital(numHospitals); // Pair {SCars, NCars}
	for (int i = 0; i < numHospitals; i++)
	{
		inputFile >> carsPerHospital[i].first >> carsPerHospital[i].second;
	}

	// Read number of patient requests
	int numRequests;
	inputFile >> numRequests;

	// Read each request and store it in a vector
	vector<string> patientRequests(numRequests);
	inputFile.ignore();

	for (int i = 0; i < numRequests; i++)
	{
		getline(inputFile, patientRequests[i]);
	}

	// Read number of cancellations
	int numCancellations;
	inputFile >> numCancellations;

	vector<string> cancellations(numCancellations);
	inputFile.ignore();
	for (int i = 0; i < numCancellations; i++)
	{
		getline(inputFile, cancellations[i]);
	}

	inputFile.close();

	// Writing Hospital Data
	ofstream hospitalFile("HospitalData.txt");
	if (hospitalFile.is_open())
	{
		hospitalFile << numHospitals << endl;

		// Writing distanceMatrix
		for (int i = 0; i < numHospitals; ++i) {
			for (int j = 0; j < numHospitals; ++j) {
				hospitalFile << distanceMatrix[i][j] << " ";
			}
			hospitalFile << endl;
		}
		// Writing carsPerHospital
		for (int i = 0; i < numHospitals; ++i) {
			hospitalFile << carsPerHospital[i].first << " " << carsPerHospital[i].second << endl;
		}
		readHospitalData(hospitalFile);
		hospitalFile.close();
	}

	// Writing Car Data
	ofstream carFile("CarData.txt");
	if (carFile.is_open())
	{
		carFile << speedScars << " " << speedNcars << endl;
		readCarData(carFile);
		carFile.close();
	}

	// Writing Patient Requests
	ofstream patientFile("PatientRequests.txt");
	if (patientFile.is_open())
	{
		patientFile << patientRequests.size() << endl; //Number of requests

		// Writing patient requests
		for (int i = 0; i < patientRequests.size(); ++i) {
			patientFile << patientRequests[i] << endl;
		}
		readPatientRequests(patientFile);
		patientFile.close();
	}

	// Writing Cancellation Requests
	ofstream cancellationFile("CancellationRequests.txt");
	if (cancellationFile.is_open())
	{
		cancellationFile << cancellations.size() << endl;

		// Writing cancellation requests
		for (int i = 0; i < cancellations.size(); ++i) {
			cancellationFile << cancellations[i] << endl;
		}
		readCancellationRequests(cancellationFile);
		cancellationFile.close();
	}
}

void Organizer::readHospitalData(ifstream& hFile)
{

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

