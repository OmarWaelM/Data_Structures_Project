#pragma once
#include "Patient.h"


enum carType {
	NC,
	SC
};

class Car
{
private:
	//Car data members
	int carID;
	carType cType;
	int speed;
	int hospital;
	Patient* assignedPatient;
	int distToPatient;
	int distToHospital;
  
public:
	Car(int id, int hosp, carType type, int spd);

	int getcarID();
	int getHospital();
	int getAssignedPatientID();
	carType getCarType();

	bool AssignPatient(Patient* p);

	friend ostream& operator <<(ostream& os, Car car);
};

