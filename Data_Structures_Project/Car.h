#pragma once

enum carType {
	NC,
	SC
};

class Car
{
private:
	//Car data members
	int carID;
	carType cType;
	int speed;
	int hospital;
	Patient* assignedPatient;
	int distToPatient;
	int distToHospital;
  
public:
	Car(int id, int hosp, carType type, int spd);

	int getcarID();
	int getHospital();
	int getAssignedPatientID();
	carType getCarType();

	bool AssignPatient(Patient* p);

	friend ostream& operator <<(ostream& os, Car& car);
};

Car::Car(int id, int hosp, carType type, int spd)
{
	carID = id;
	hospital = hosp;
	cType = type;
	speed = spd;
	assignedPatient = nullptr;
	distToHospital = 0;
	distToPatient = 0;
}

int Car::getcarID()
{
	return carID;
}

int Car::getHospital()
{
	return hospital;
}

int Car::getAssignedPatientID()
{
	return assignedPatient->getPatientID();
}

carType Car::getCarType()
{
	return cType;
}


bool  Car::AssignPatient(Patient* p)
{
	if (assignedPatient == nullptr)
	{
		assignedPatient = p;
		distToPatient = p->getDistance();
		distToHospital = 0;
		return true;
	}
	else
		return false;

}


ostream& operator <<(ostream& os, Car& car)
{
	if (car.cType == SC)
		os << 'S';
	else
		os << 'N';
	os << car.carID << "_H" << car.hospital << "_P" << car.assignedPatient->getPatientID();
	return os;
}
