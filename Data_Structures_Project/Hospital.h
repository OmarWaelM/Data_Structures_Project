#pragma once
#include "UI.h"
#include "Patient.h"
#include "Car.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ModifiedQ.h"
#include "ModifiedPriQ.h"

class Hospital
{
private:
	//Lists used in hospital class
	LinkedQueue<Patient*> SPList;
	priQueue<Patient*> EPList;
	ModifiedQ<Patient*> NPList;

	LinkedQueue<Car*> SCList;
	LinkedQueue<Car*> NCList;

public:
	//Member Function


};

