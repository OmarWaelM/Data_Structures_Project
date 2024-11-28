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

public:
	//Member Function
	Hospital();
	void setID(int id) { hospitalID = id; }
	int getHospitalID() const { return hospitalID; }
	void setSCarsCount(int count) { scCount = count; }
	void setNCarsCount(int count) { ncCount = count; }
	int getSCarsCount() const { return scCount; } // Getter for SCars count
	int getNCarsCount() const { return ncCount; } // Getter for NCars count
	void addCarToList(Car* car);
	void addPatientToList(Patient* patient);
	bool assignPatientToCar(Patient* patient);

	//Simulation Specific Function
	bool empty();
	bool getNP(Patient*& p);
	bool getEP(Patient*& p);
	bool getSP(Patient*& p);
	bool getNC(Car*& c);
	bool getSC(Car*& c);


	friend ostream& operator <<(ostream& os, Hospital& h);

};

Hospital::Hospital(): hospitalID(0), scCount(0), ncCount(0) {}

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

bool Hospital::assignPatientToCar(Patient* p)
{
	Car* ambulance = nullptr;
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

		}
		else if (SCList.getCount() != 0)
		{
			SCList.dequeue(ambulance);
			EPList.dequeue(patient, x);
			ambulance->AssignPatient(patient);
		}
		return true;
	}
	
	return false;
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


bool Hospital::empty()
{
	bool res = (NPList.isEmpty() && SPList.isEmpty() && EPList.isEmpty());
	return res;
}

bool Hospital::getNP(Patient*& p)
{
	bool res = NPList.dequeue(p);
	return res;
}

bool Hospital::getEP(Patient*& p)
{
	int pri;
	return EPList.dequeue(p, pri);
}

bool Hospital::getSP(Patient*& p)
{
	return SPList.dequeue(p);
}
bool Hospital::getNC(Car*& c)
{
	return NCList.dequeue(c);
}

bool Hospital::getSC(Car*& c)
{
	return SCList.dequeue(c);
}


#endif