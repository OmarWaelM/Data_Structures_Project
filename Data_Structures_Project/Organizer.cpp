#include "Organizer.h"
#include "UI.h"
#include "Patient.h"
#include "Car.h"
#include "Hospital.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ModifiedQ.h"
#include "ModifiedPriQ.h"
#include <iostream>

using namespace std;

Organizer::Organizer()
{
	GUI.Start();

	HospitalList = new Hospital[5];
	//test
	for (int i = 0; i < 5; i++) {

	}
}
