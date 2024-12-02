#ifndef CAR_H
#define CAR_H

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
	int distToPatient; //decrememnted by speed every timestep
	int distToHospital; //incrememnted by speed every timestep

public:
	Car(int id, int hosp, carType type, int spd);

	int getcarID();
	int getHospital();
	int getAssignedPatientID();
	int getDistToPatient();
	int getDistToHospital();
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

int Car::getDistToPatient()
{
	return distToPatient;
}

int Car::getDistToHospital()
{
	return distToHospital;
}

carType Car::getCarType()
{
	return cType;
}

bool Car::AssignPatient(Patient* p)
{	
	if (assignedPatient == nullptr)
	{
		assignedPatient = p;
		distToPatient = p->getDistance();
		distToHospital = 0;
		return true;
	}
	else
	{
		return false;
	}
}

ostream& operator <<(ostream& os, Car& car)
{
	if (car.cType == SC)
		os << 'S';
	else
		os << 'N';
	if (car.assignedPatient)
	{
		os << car.carID << "_H" << car.hospital << "_P" << car.assignedPatient->getPatientID();
	}
	else //only used during this phase as later on patient will always be assigned when printing
	{
		os << car.carID << "_H" << car.hospital;
	}
	return os;
}

#endif