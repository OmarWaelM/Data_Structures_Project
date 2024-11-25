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
	int arrivalTime;
	int returnTime;      // Time at which the car is expected to return to the hospital
	int priority;
public:
	Car(int id, int hosp, carType type, int spd);

	int getcarID();
	int getHospital();
	int getAssignedPatientID();
	int getArrivalTime();
	carType getCarType();
	int getReturnTime();
	void setReturnTime(int time) { returnTime = time; }
	bool AssignPatient(Patient* p);
	int getPriority() const { return priority; }
	void pickupPatient()
	{
		cout << "Car " << carID << " is picking up a patient." << endl;
		
	}
	friend ostream& operator <<(ostream& os, Car& car);
};

Car::Car(int id, int hosp, carType type, int spd)
{
	carID = id;
	hospital = hosp;
	cType = type;
	speed = spd;
	assignedPatient = nullptr;
	distToHospital = 0;
	distToPatient = 0;
	returnTime = 0;
}

int Car::getcarID()
{
	return carID;
}

int Car::getHospital()
{
	return hospital;
}

int Car::getAssignedPatientID()
{
	return assignedPatient->getPatientID();
}

inline int Car::getArrivalTime()
{
	return arrivalTime;
}
int Car::getReturnTime()
{
	return returnTime;
}

carType Car::getCarType()
{
	return cType;
}


bool  Car::AssignPatient(Patient* p)
{	
	if (assignedPatient == nullptr)
	{
		assignedPatient = p;
		distToPatient = p->getDistance();
		distToHospital = 0;
		return true;
	}
	else
		return false;

}


ostream& operator <<(ostream& os, Car& car)
{
	if (car.cType == SC)
		os << 'S';
	else
		os << 'N';
	os << car.carID << "_H" << car.hospital << "_P" << car.assignedPatient->getPatientID();
	return os;
}
