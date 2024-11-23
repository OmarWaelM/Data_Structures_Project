#pragma once
#include "Patient.h"

class Car
{
private:
	//Car data members
	enum carType {
		NC,
		SC
	};
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

