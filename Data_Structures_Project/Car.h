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
	Patient* assignedPatient;
	bool inCheckUp;  //true if the car needs a check up
	bool failureOut;
	bool failureBack;

public:
	//Constructor
	Car(int id, int hosp, carType type, int spd);

	//Assignment and Deassignment
	bool AssignPatient(Patient* p);
	Patient* deassignPatient();
	
	void setFailureOut(bool state) { failureOut = state; }
	void setFailureBack(bool state) { failureBack = state; }
	void setInCheckup(bool state) { inCheckUp = state; }
	void setCarType(carType c) { cType = c; }

	//Update functions decrement distances by speed every timestep, if distance is less that speed decrements to 0
	void updateBack();
	void updateOut();	

	//Getters
	int getcarID() const { return carID; }
	int getHospital() const { return hospital; }
	int getAssignedPatientID() const { return assignedPatient->getPatientID(); }
	int getDistToPatient() const { return distToPatient; }
	int getDistToHospital() const { return distToHospital; }
	carType getCarType() const { return cType; }
	Patient* getAssignedPatient() const { return assignedPatient; } // Return the pointer to the assigned patient
	bool getFailureOut() const { return failureOut; }
	bool getFailureBack() const { return failureBack; }
	bool isInCheckup() const { return this->inCheckUp; }
  
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
	inCheckUp = false;
	distToPatient = -1;
	failureOut = false;
	failureBack = false;
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

void Car::updateBack()
{
	if (!assignedPatient->getStopped())
	{
		if (distToHospital < speed)
		{
			distToPatient += distToHospital;
			distToHospital = 0;
		}
		else
		{
			distToPatient += speed;
			distToHospital -= speed;
		}
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