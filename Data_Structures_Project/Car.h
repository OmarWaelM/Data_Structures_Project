#pragma once
#include "Patient.h"

class Car
{
private:
	//Car data members
	char type;
	int speed;
	char status;
	Patient* assignedPatient;
	int distToPatient;
	//can we add a busy time counter?
	//we can add a assigned time and return time so we dont need to access outcarlist 
	//every timestep

};

