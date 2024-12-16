#ifndef HOSPITAL_H
#define HOSPITAL_H
#include "ModifiedQ.h"

class Hospital
{
private:
	//Lists used in hospital class
	LinkedQueue<Patient*> SPList;
	priQueue<Patient*> EPList;
	ModifiedQ NPList;
	LinkedQueue<Car*> SCList;
	LinkedQueue<Car*> NCList;

	//General data memebers
	int hospitalID;

public:
	//Member Function
	
	//Constructor
	Hospital();

	//List Members movemen
	void addCarToList(Car* car);
	bool addPatientToList(Patient* patient);
	void addFailurePatient(Patient* patient); //adds patient to top after car failure

	//Car and Patient Assignment
	bool assignPatients(Car*&  ambulance);
	bool assignPatientToCar(Patient* patient, Car*& ambulance);
	bool cancelRequest(int patientID) { return NPList.cancelRequest(patientID); } //update this to return car&

	//Setter
	void setID(int id) { hospitalID = id; }

	//Getters
	int getHospitalID() const { return hospitalID; }	// Getter for Hospital ID
	int getSCarsCount() { return SCList.getCount(); }	// Getter for SCars count
	int getNCarsCount() { return NCList.getCount(); }	// Getter for NCars count
	int getEPListLength() { return EPList.getCount(); }	// Getter for EPList count
	bool isEmpty() { return (NPList.isEmpty() && SPList.isEmpty() && EPList.isEmpty()); }	// Checks if all patient lists are empty
	bool isPatientInNPList(int patientID);

	//Outstream operator overloading
	friend ostream& operator <<(ostream& os, Hospital& h);

	//Simulation Specific Function
	bool getNP(Patient*& p);
	bool getEP(Patient*& p);
	bool getSP(Patient*& p);
	bool getNC(Car*& c);
	bool getSC(Car*& c);
};

Hospital::Hospital(): hospitalID(0) {}

void Hospital::addCarToList(Car* car)
{
	if (car->getCarType() == SC)
		SCList.enqueue(car);
	else
		NCList.enqueue(car);
}

bool Hospital::addPatientToList(Patient* patient)
{
	if (patient->getPatientType() == SP)
	{
		SPList.enqueue(patient);
		return true;
	}
	else if (patient->getPatientType() == NP)
	{
		NPList.enqueue(patient);
		return true;
	}
	else
	{
		if (!NCList.isEmpty() || !SCList.isEmpty())
		{
			EPList.enqueue(patient, patient->getPatientPriority());
			return true;
		}
		return false;
	}
}

void Hospital::addFailurePatient(Patient* patient)
{
	if (patient->getPatientType() == SP)
	{
		SPList.addToTop(patient);
	}
	else if (patient->getPatientType() == NP)
	{
		NPList.addToTop(patient);
	}
	else
	{
		EPList.addToTop(patient, patient->getPatientPriority());
	}
}

bool Hospital::assignPatients(Car*& ambulance)
{
	Patient* p;
	int pri;
	if (!EPList.isEmpty())
	{
		EPList.peek(p, pri);
		if (assignPatientToCar(p, ambulance))
		{
			EPList.dequeue(p, pri);
			return true;
		}
	}
	if (!SPList.isEmpty())
	{
		SPList.peek(p);
		if (assignPatientToCar(p, ambulance))
		{
			SPList.dequeue(p);
			return true;
		}
	}
	if (!NPList.isEmpty())
	{
		NPList.peek(p);
		if (assignPatientToCar(p, ambulance))
		{
			NPList.dequeue(p);
			return true;
		}
	}
}

bool Hospital::assignPatientToCar(Patient* p, Car*& ambulance)
{
	Patient* patient = nullptr;
	int x;
	if (p->getPatientType() == NP && NCList.getCount() != 0)
	{
		NPList.dequeue(patient);
		NCList.dequeue(ambulance);
		ambulance->AssignPatient(patient);
		return true;
	}
	else if (p->getPatientType() == SP && SCList.getCount() != 0)
	{
		SPList.dequeue(patient);
		SCList.dequeue(ambulance);
		ambulance->AssignPatient(patient);
		return true;
	}
	else if (p->getPatientType() == EP)
	{
		if (NCList.getCount() != 0)
		{
			NCList.dequeue(ambulance);
			EPList.dequeue(patient,x);
			ambulance->AssignPatient(patient);
			return true;
		}
		else if (SCList.getCount() != 0)
		{
			SCList.dequeue(ambulance);
			EPList.dequeue(patient, x);
			ambulance->AssignPatient(patient);
			return true;
		}
	}
	return false;
}

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

bool Hospital::isPatientInNPList(int patientID) const
{
	if (NPList.isEmpty()) { return false; }

	// Start from the front of the queue (which is a LinkedQueue)
	Node<Patient*>* currentNode = NPList.getFront();

	// Traverse the queue until we find the patient or reach the end
	while (currentNode != nullptr)
	{
		if (currentNode->getItem()->getPatientID() == patientID) { return true; }
		currentNode = currentNode->getNext();
	}
	return false;
}


//Simulator Function will probably not need in phase 2
bool Hospital::getNP(Patient*& p) { return NPList.dequeue(p); }
bool Hospital::getEP(Patient*& p) { 
	int pri;
	return EPList.dequeue(p, pri);
}
bool Hospital::getSP(Patient*& p) { return SPList.dequeue(p); }
bool Hospital::getNC(Car*& c) { return NCList.dequeue(c); }
bool Hospital::getSC(Car*& c) { return SCList.dequeue(c); }

#endif