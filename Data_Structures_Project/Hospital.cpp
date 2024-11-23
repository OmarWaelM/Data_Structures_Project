#include "Hospital.h"

void Hospital::assignPatientToList(Patient* patient)
{
	if (patient->getPatientType() == SP)
		SPList.enqueue(patient);
	else if (patient->getPatientType() == NP)
		NPList.enqueue(patient);
	else
		EPList.enqueue(patient, patient->getPatientPriority());
}

void Hospital::assignPatientToCar(Patient* patient, Car* car)
{
}

