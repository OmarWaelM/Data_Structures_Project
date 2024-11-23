#include "Hospital.h"

void Hospital::assignPatientToList(Patient* patient)
{
	if (patient->getPatientType() == 1)
		SPList.enqueue(patient);
	else if (patient->getPatientType() == 2)
		NPList.enqueue(patient);
	else
		EPList.enqueue(patient);
}

void Hospital::assignPatientToCar(Patient* patient, Car* car)
{
}

