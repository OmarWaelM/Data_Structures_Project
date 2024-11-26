#ifndef ORGANIZER_H
#define ORGANIZER_H
using namespace std;
#include "UI.h"

class UI;

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
	int* DistancesMatrix;
	//General data members
	int timeStep;
	UI GUI;

public:
    //Member Functions
    
    //Constructor
    Organizer();

    //void readHospitalData(ofstream& of);
    //void readCarData(ofstream& of);
    //void readPatientList(ofstream& of);
    //void readCancellationList(ofstream& of);

    Hospital* getHospital(int ID) { return &HospitalList[ID]; }
    LinkedQueue<Patient*>* getFinishedList() { return &FinishedList; }

    Hospital* getNearestHospital(Patient* EP);
    bool addEPtoHospital(Patient* EP);
    /*
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

    */

    ~Organizer();

};



Organizer::Organizer():GUI(this)
{
    GUI.Start();
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
    delete[] HospitalList;
}
#endif