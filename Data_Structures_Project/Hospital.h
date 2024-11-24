#ifndef HOSPITAL_H
#define HOSPITAL_H
#include "ModifiedPriQ.h"

class Hospital
{
private:
	//Lists used in hospital class
	LinkedQueue<Patient*> SPList;
	priQueue<Patient*> EPList;
	ModifiedPriQ<Patient*> NPList;

	LinkedQueue<Car*> SCList;
	LinkedQueue<Car*> NCList;

	//General data memebers
	int hospitalID;
	int** distanceMatrix;  // 2D array to store the distance matrix for the hospital

public:
	//Member Function
	Hospital() {}
	void setID(int id) { hospitalID = id; }
	void setDistanceMatrix(int** matrix, int size);
	int getHospitalID() { return hospitalID; }
	int** getDistanceMatrix() { return distanceMatrix; }
	void addCarToList(Car* car);
	void addPatientToList(Patient* patient);
	void assignPatientToCar(Patient* patient, Car* car);

	friend ostream& operator <<(ostream& os, Hospital& h);

};

void Hospital::addCarToList(Car* car)
{
	if (car->getCarType() == SC)
		SCList.enqueue(car);
	else
		NCList.enqueue(car);
}

void Hospital::addPatientToList(Patient* patient)
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

void Hospital::setDistanceMatrix(int** matrix, int size)
{
	// Allocate memory for the matrix
	distanceMatrix = new int* [size];
	for (int i = 0; i < size; ++i)
	{
		distanceMatrix[i] = new int[size];  // Allocate memory for each row
	}

	// Copy data from the input matrix
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j) {
			distanceMatrix[i][j] = matrix[i][j];  // Assign each value
		}
	}
}

//this can be changed i made it to look like the description
ostream& operator <<(ostream& os, Hospital& h)
{
	os << "==============	  Hospital #" << h.hospitalID << " data   ==============" << endl;
	os << h.EPList.getCount() << " EP requests: " << h.EPList << endl;
	os << h.SPList.getCount() << " SP requests: " << h.SPList << endl;
	os << h.NPList.getCount() << " NP requests: " << h.NPList << endl;
	os << "Free Cars: " << h.SCList.getCount() << " SCars, " << h.NCList.getCount() << " NCars" << endl;
	os << "==============	Hospital #" << h.hospitalID << " data end  =============" << endl;
	return os;
}

#endif