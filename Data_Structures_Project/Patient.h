#pragma once
#include <iostream>

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
	int patientPri;
	int reqTime;
	int pickupTime;
	int nearestHospital;
	int distToHospital;
public:

	Patient(int id, int requestTime, int hospital, int dist, patientType type, int priority = -1);
	patientType getPatientType();
	int getPatientPriority();
	int getPatientID();
	int getDistance();
	int getNearestHospital();

	friend ostream& operator <<(ostream& os, Patient patient);
};
