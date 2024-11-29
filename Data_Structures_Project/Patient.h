#ifndef PATIENT_H
#define PATIENT_H

using namespace std;

enum patientType{
	SP,
	NP,
	EP
};

class Patient
{
private:
	//Patient data members
	int patientID;
	patientType pType;
	int patientPri; // Corresponds to the case severity (for EP only)
	int reqTime; // Time of the request
	int pickupTime;
	int nearestHospital;
	int distToHospital;
public:
	Patient(int id, int requestTime, int hospital, int dist, patientType type, int priority = -1);
	patientType getPatientType();
	int getPatientPriority();
	int getPatientID();
	int getRequestTime();
	int getDistance();
	int getNearestHospital();
	void setPriority(int priority) { patientPri = priority; }
	friend ostream& operator <<(ostream& os, Patient& patient);
};


Patient::Patient(int id, int requestTime, int hospital, int dist, patientType type, int priority)
	: patientID(id), reqTime(requestTime), nearestHospital(hospital), distToHospital(dist),
	pType(type), patientPri(priority), pickupTime(-1) 
{
	// If the patient type is not EP, the priority (case severity) should remain -1
	if (pType != EP)
	{
		patientPri = -1;
	}
}

patientType Patient::getPatientType()
{
	return pType;
}

int Patient::getPatientPriority()
{
	return patientPri;
}

int Patient::getPatientID()
{
	return patientID;
}

int Patient::getRequestTime()
{
	return reqTime;
}

int Patient::getDistance()
{
	return distToHospital;
}

int Patient::getNearestHospital()
{
	return nearestHospital;
}


ostream& operator <<(ostream& os, Patient& patient)
{
	os << patient.patientID;
	return os;
}

#endif