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

public:
	//Constructor
	Car(int id, int hosp, carType type, int spd);

	//Assignment and Deassignment
	bool AssignPatient(Patient* p);
	void setInCheckup(bool state) {this->inCheckUp = state;}
	bool isInCheckup() const {return this->inCheckUp;}

	//----------------------------------------------------------------------------------------------------
	//TODO: Implement functions to return patient and reset values to retrun to hospital
	//Patient* deassignPatient();
	//		Update functions decrement distances by speed every timestep, if distance is less that speed decrements to 0
	//void updateBack();
	//void updateOut();	
	//----------------------------------------------------------------------------------------------------

	//Getters
	int getcarID() { return carID; }
	int getHospital() { return hospital; }
	int getAssignedPatientID() { return assignedPatient->getPatientID(); }
	int getDistToPatient() { return distToPatient; }
	int getDistToHospital() { return distToHospital; }
	carType getCarType() { return cType; }
	Patient* getAssignedPatient() const { return assignedPatient; } // Return the pointer to the assigned patient

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
	distToPatient = 0;
	inCheckUp = false;
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