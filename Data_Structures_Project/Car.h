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
	carType ctype;
	int speed;
	char status;
	Patient* assignedPatient;
	int distToPatient;
	//can we add a busy time counter?
	//we can add a assigned time and return time so we dont need to access outcarlist 
	//every timestep
public:
	carType getCarType();
};

