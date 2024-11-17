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
	Hospital* HospitalList;

	//General data members
	int timeStep;
	UI GUI;

public:
	//Member Functions
        //Constructor
        Organizer();

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



     ~Organizer();



};

