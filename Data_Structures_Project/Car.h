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
	carType cType;
	int carID;
	int speed;
	int hospital;
	int distToPatient;	//decrememnted by speed every timestep
	int distToHospital; //incrememnted by speed every timestep
	int timeToCompleteCheckup;
	Patient* assignedPatient;
	bool failure;

public:
	//Constructor
	Car(int id, int hosp, carType type, int spd);

	//Assignment and Deassignment
	bool AssignPatient(Patient* p);
	Patient* deassignPatient();

	void Failure();

	//Update functions decrement distances by speed every timestep, if distance is less that speed decrements to 0
	void updateBack();
	void updateOut();	
	void updateCheckup();

	//Getters
	int getcarID() { return carID; }
	int getHospital() { return hospital; }
	int getAssignedPatientID() { return assignedPatient->getPatientID(); }
	int getDistToPatient() { return distToPatient; }
	int getDistToHospital() { return distToHospital; }
	int getTimeToCompleteCheckup() { return timeToCompleteCheckup; }
	carType getCarType() { return cType; }
	bool getFailureState() { return failure; }
	
	//Outstream operator overloading
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
	distToPatient = -1;
	timeToCompleteCheckup = -1;
	failure = false;
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
		return false;
}

Patient* Car::deassignPatient()
{
	if (assignedPatient != nullptr)
	{
		Patient* p = assignedPatient;
		assignedPatient = nullptr;
		distToPatient = -1;
		distToHospital = 0;
		return p;
	}
	else
		return nullptr;
}

void Car::Failure()
{

}

void Car::updateBack()
{
	if (distToHospital < speed)
	{
		distToPatient += distToHospital;
		distToHospital = 0;
	}
	else
	{
		distToPatient == speed;
		distToHospital -= speed;
	}
}

void Car::updateOut()
{
	if (distToPatient < speed)
	{
		distToHospital += distToPatient;
		distToPatient = 0;
	}
	else
	{
		distToPatient -= speed;
		distToHospital += speed;
	}
}

void Car::updateCheckup()
{
	timeToCompleteCheckup--;
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
	else
	{
		os << car.carID << "_H" << car.hospital;
	}
	return os;
}

#endif