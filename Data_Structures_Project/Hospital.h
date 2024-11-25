#ifndef HOSPITAL_H
#define HOSPITAL_H
#include "ModifiedPriQ.h"
#include "ModifiedQ.h"
#include "LinkedQueue.h"
#include "priQueue.h"

class Hospital
{
private:
	//Lists used in hospital class
	LinkedQueue<Patient*> SPList;
	priQueue<Patient*> EPList;
	ModifiedQ<Patient*> NPList;

	LinkedQueue<Car*> SCList;
	LinkedQueue<Car*> NCList;

	//General data memebers
	int hospitalID;
	int scCount;           // Number of SCars
	int ncCount;           // Number of NCars
	int** distanceMatrix;  // 2D array to store the distance matrix for the hospital

public:
	//Member Function
	Hospital();
	void setID(int id) { hospitalID = id; }
	int getHospitalID() const { return hospitalID; }
	void setSCarsCount(int count) { scCount = count; }
	void setNCarsCount(int count) { ncCount = count; }
	int getSCarsCount() const { return scCount; } // Getter for SCars count
	int getNCarsCount() const { return ncCount; } // Getter for NCars count
	void setDistanceMatrix(int** matrix, int size);
	int** getDistanceMatrix() { return distanceMatrix; }
	void addCarToList(Car* car);
	void addPatientToList(Patient* patient);
	void assignPatientToCar(Patient* patient, Car* car);


	// friend ostream& operator <<(ostream& os, Hospital& h);

};

Hospital::Hospital(): distanceMatrix(nullptr), hospitalID(0), scCount(0), ncCount(0) {}

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
/*ostream& operator <<(ostream& os, Hospital& h)
{
	os << "==============	  Hospital #" << h.hospitalID << " data   ==============" << endl;
	os << h.EPList.getCount() << " EP requests: " << h.EPList << endl;
	os << h.SPList.getCount() << " SP requests: " << h.SPList << endl;
	os << h.NPList.getCount() << " NP requests: " << h.NPList << endl;
	os << "Free Cars: " << h.SCList.getCount() << " SCars, " << h.NCList.getCount() << " NCars" << endl;
	os << "==============	Hospital #" << h.hospitalID << " data end  =============" << endl;
	return os;
} */

#endif