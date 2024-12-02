#ifndef HOSPITAL_H
#define HOSPITAL_H

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
	void addPatientToList(Patient* patient); //update this to return bool in case EP request cannot be served

	//Car and Patient Assignment
	bool assignPatientToCar(Patient* patient);
	bool cancelRequest(int patientID); //update this to return car&

	//TODO: Function to loop and assign patients until either all patients are assigned or no more patients 
	//		can be assigned 
	//void updateHospital();


	//Setter
	void setID(int id) { hospitalID = id; }

	//Getters
	int getHospitalID() const { return hospitalID; }	// Getter for Hospital ID
	int getSCarsCount() { return SCList.getCount(); }	// Getter for SCars count
	int getNCarsCount() { return NCList.getCount(); }	// Getter for NCars count
	int getEPListLength() { return EPList.getCount(); }	// Getter for EPList count
	bool isEmpty() { return (NPList.isEmpty() && SPList.isEmpty() && EPList.isEmpty()); }	// Checks if all patient lists are empty
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

bool Hospital::cancelRequest(int patientID)
{
	return NPList.cancelRequest(patientID);
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