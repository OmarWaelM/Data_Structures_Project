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
	int reqTime;	// Time of the request
	int pickupTime;
	int finishTime;

	int nearestHospital;
	int distToHospital;

	bool stopped; // During back car failure

public:
	//Constructor
	Patient(int id, int requestTime, int hospital, int dist, patientType type, int priority = -1);

	//Getters
	patientType getPatientType() { return pType; }
	int getPatientPriority() { return patientPri; }
	int getPatientID() { return patientID; }
	int getRequestTime() { return reqTime; }
	int getDistance() { return distToHospital; }
	int getNearestHospital() { return nearestHospital; }
	int getFinishTime() { return finishTime; }
	int getWaitTime() { return pickupTime - reqTime; }
	int getBusyTime() { return finishTime - pickupTime; }
	bool getStopped() { return stopped; }

	//Setters
	void setPatientID(int id) { patientID = id; }
	void setPickup(int pt) { pickupTime = pt; }
	void setFinished(int ft) { finishTime = ft; }
	void setDistanceToPickup(int dst) { distToHospital = dst; }
	void setStopped(bool stp) { stopped = stp; }
	bool hasArrived(bool state) { if (finishTime != -1) return true; }

	//Output stream operator overloading
	friend ostream& operator <<(ostream& os, Patient& patient);
};

//Constructor
Patient::Patient(int id, int requestTime, int hospital, int dist, patientType type, int priority)
	:patientID(id), 
	 reqTime(requestTime),
	 nearestHospital(hospital), 
	 distToHospital(dist),
	 pType(type), 
	 patientPri(priority), 
	 pickupTime(-1), 
	 finishTime(-1),
	 stopped(false)
{
}

//Outstream operator overloading
ostream& operator <<(ostream& os, Patient& patient)
{
	os << patient.patientID;
	return os;
}

#endif