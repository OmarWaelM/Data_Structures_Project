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



};

