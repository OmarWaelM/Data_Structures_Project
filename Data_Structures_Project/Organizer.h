#ifndef ORGANIZER_H
#define ORGANIZER_H
using namespace std;
#include "UI.h"

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
	Hospital* HospitalList;
    int numberOfHospitals;
	int* DistancesMatrix;
	//General data members
	int timeStep;
	UI GUI;

public:
    //Member Functions
    
    //Constructor
    Organizer();
    void Simulator();
    //void readHospitalData(ofstream& of);
    //void readCarData(ofstream& of);
    //void readPatientList(ofstream& of);
    //void readCancellationList(ofstream& of);

    
   
    // Functions for managing Back Cars:
    //Adding a Back Car based on its priority
    void AddBackCar(const string& Car, int Priority);
    //Removing the highest priority from the Back_Cars queue
    bool RemoveBackCar(string& Car);

    //Functions for managing hospital list:
    //Adding a Hospital to the hospital list
    void AddHospital(const string& Hospital_Name);
    //Printing out the hospitals in the hospital list
    void PrintHospitals()const;

    // functions for managing finished lists
    //Adding an entry to the finished list
    void AddFinished(const string& Finished_Entry);

    //Removing an entry from the finished list
    bool RemoveFinished(string& Finished_Entry);

    //Print items in Finished List
    void DisplayFinishedList()const;

    // Functions for handling Out Cars
      void handleCancellations();
      void handleCarMovements();
    

    ~Organizer();

};



Organizer::Organizer()
{
}

Organizer::Simulator()
{
    timeStep = 0;
    GUI.Start();

}
void Organizer::handleCarMovements()
{
    Car* car;

    // Process OutCars: move cars to BackCars if they have arrived
    while (!OutCars.isEmpty() && OutCars.peek(car) && car->getArrivalTime() == timeStep) {
        int priority;
        OutCars.dequeue(car, priority);
        car->pickupPatient(); // Perform patient pickup
        BackCars.enqueue(car, car->getPriority());
    }

    // Process BackCars: return cars to hospitals if they have completed their task
    int priority;
    while (!BackCars.isEmpty() && BackCars.peek(car, priority) && car->getReturnTime() == timeStep) {
        BackCars.dequeue(car, priority);
        // Handle returning the car to its hospital
        // HospitalList[car->getHospitalID()].handleReturningCar(car);

    }


Organizer::~Organizer()
{
    delete[] HospitalList;
}


#endif#ifndef ORGANIZER_H
#define ORGANIZER_H
using namespace std;
#include "UI.h"

class UI;

struct CancellationReq
{
	int PID;
    int hospitalID;
	int CancellationTimestep;
};

