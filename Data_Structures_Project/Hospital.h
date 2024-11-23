#pragma once

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

public:
	//Member Function
	Hospital(int id);

	void addCarToList(Car* car);
	void addPatientToList(Patient* patient);
	void assignPatientToCar(Patient* patient, Car* car);

	friend ostream& operator <<(ostream& os, Hospital& h);

};

