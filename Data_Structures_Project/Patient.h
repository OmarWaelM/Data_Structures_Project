#pragma once
class Patient
{
private:
	//Patient data members
	enum patientType {
		SP,
		NP,
		EP
	};
	int reqTime;
	int pickupTime;
	int nearestHospital;
	int distToHospital;
public:
	patientType getPatientType();

};
