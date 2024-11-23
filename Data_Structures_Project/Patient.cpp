#include "Patient.h"

Patient::Patient(int id, int requestTime, int hospital, int dist, patientType type, int priority)
{
	patientID = id;
	reqTime = requestTime;
	nearestHospital = hospital;
	distToHospital = dist;
	pType = type;
	patientPri = priority;
	pickupTime = -1;
}


patientType Patient::getPatientType()
{
	return patientType();
}

int Patient::getPatientPriority()
{
	return patientPri;
}

int Patient::getPatientID()
{
	return patientID;
}

int Patient::getDistance()
{
	return distToHospital;
}

int Patient::getNearestHospital()
{
	return nearestHospital;
}

ostream& operator <<(ostream& os,  Patient patient)
{
	os << patient.patientID;
	return os;
}