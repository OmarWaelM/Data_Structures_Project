#pragma once
enum patientType{
	SP,
	NP,
	EP
};
class Patient
{
private:
	//Patient data members
	patientType ptype;
	int reqTime;
	int pickupTime;
	int nearestHospital;
	int distToHospital;
public:
	patientType getPatientType();

};
