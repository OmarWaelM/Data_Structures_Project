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
	bool failed;
	int failureTimeStep;

public:
	//Member Function
	
	//Constructor
	Hospital();

	//List Members movemen
	void addCarToList(Car* car);
	bool addPatientToList(Patient* patient);
	void addPatientToListEP(Patient* patient); //Only after EP is handled
	void addFailurePatient(Patient* patient); //adds patient to top after car failure

	//Car and Patient Assignment
	bool assignPatients(Car*&  ambulance);
	bool assignPatientToCar(Patient* patient, Car*& ambulance);
	bool cancelRequest(int patientID) { return NPList.cancelRequest(patientID); } //update this to return car&

	//Setter
	void setID(int id) { hospitalID = id; }
	void setFailureTimeStep(int t) { failureTimeStep = t; }

	//Getters
	int getHospitalID() const { return hospitalID; }	// Getter for Hospital ID
	int getSCarsCount() { return SCList.getCount(); }	// Getter for SCars count
	int getNCarsCount() { return NCList.getCount(); }	// Getter for NCars count
	int getNPListCount() { return NPList.getCount(); }
	int getSPListCount() { return SPList.getCount(); }
	int getEPListLength() { return EPList.getCount(); }	// Getter for EPList count
	bool isEmpty() { return (NPList.isEmpty() && SPList.isEmpty() && EPList.isEmpty()); }	// Checks if all patient lists are empty
	int getFailureTimeStep() {return failureTimeStep;}
	int getTotalBusyTime();

	//Outstream operator overloading
	friend ostream& operator <<(ostream& os, Hospital& h);

	// For the hospital failure feature
	bool isFailed() { return failed; }
	void setFailed(bool status) { failed = status; }

	LinkedQueue<Patient*> transferSPList()
	{
		LinkedQueue<Patient*> temp;
		Patient* tempItem;

		// Copy SPList to temp
		while (!SPList.isEmpty())
		{
			SPList.dequeue(tempItem);
			temp.enqueue(tempItem);
		}
		//Do NOT restore the SP List to nullify it
		return temp;
	}

	priQueue<Patient*> transferEPList()
	{
		priQueue<Patient*> temp;
		Patient* tempItem;
		int x;
		// Copy EPList to temp
		while (!EPList.isEmpty())
		{
			EPList.dequeue(tempItem, x);
			temp.enqueue(tempItem, x);
		}
		//Do NOT restore the EP List to nullify it
		return temp;
	}

	ModifiedQ transferNPList()
	{
		ModifiedQ temp;
		Patient* tempItem;

		// Copy NPList to temp
		while (!NPList.isEmpty())
		{
			NPList.dequeue(tempItem);
			temp.enqueue(tempItem);
		}
		//Do NOT restore the NP List to nullify it
		return temp;
	}

	LinkedQueue<Car*> transferSCList() { return SCList; }

	LinkedQueue<Car*> transferNCList() { return NCList; }
};

Hospital::Hospital(): hospitalID(0), failed(false) {}

void Hospital::addCarToList(Car* car)
{
	if (failed)
	{
		delete car;
	}
	else
	{
		if (car->getCarType() == SC)
			SCList.enqueue(car);
		else
			NCList.enqueue(car);
	}
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

void Hospital::addPatientToListEP(Patient* patient)
{
	EPList.enqueue(patient, patient->getPatientPriority());
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
	Patient* p = nullptr;
	int pri = -1;

	if (!EPList.isEmpty())
	{
		EPList.peek(p, pri);
		if (p && assignPatientToCar(p, ambulance))
		{
			return true;
		}
	}
	if (!SPList.isEmpty())
	{
		SPList.peek(p);
		if (p && assignPatientToCar(p, ambulance))
		{
			return true;
		}
	}
	if (!NPList.isEmpty())
	{
		NPList.peek(p);
		if (p && assignPatientToCar(p, ambulance))
		{
			return true;
		}
	}
	return false;
}

bool Hospital::assignPatientToCar(Patient* p, Car*& ambulance)
{
	int x;
	if (p->getPatientType() == NP && !NCList.isEmpty())
	{
		NPList.dequeue(p);
		NCList.dequeue(ambulance);
		ambulance->AssignPatient(p);
		return true;
	}
	else if (p->getPatientType() == SP && !SCList.isEmpty())
	{
		SPList.dequeue(p);
		SCList.dequeue(ambulance);
		ambulance->AssignPatient(p);
		return true;
	}
	else if (p->getPatientType() == EP)
	{
		if (!NCList.isEmpty())
		{
			NCList.dequeue(ambulance);
			EPList.dequeue(p, x);
			ambulance->AssignPatient(p);
			return true;
		}
		else if (!SCList.isEmpty())
		{
			SCList.dequeue(ambulance);
			EPList.dequeue(p, x);
			ambulance->AssignPatient(p);
			return true;
		}
	}
	return false;
}

int Hospital::getTotalBusyTime()
{
	int total = 0;
	LinkedQueue<Car*> temp;
	Car* c;
	while (!SCList.isEmpty())
	{
		SCList.dequeue(c);
		total += c->getBusyTime();
		temp.enqueue(c);
	}
	while (!temp.isEmpty())
	{
		temp.dequeue(c);
		SCList.enqueue(c);
	}
	while (!NCList.isEmpty())
	{
		NCList.dequeue(c);
		total += c->getBusyTime();
		temp.enqueue(c);
	}
	while (!temp.isEmpty())
	{
		temp.dequeue(c);
		NCList.enqueue(c);
	}
	return total;
}

ostream& operator <<(ostream& os, Hospital& h)
{
	if (!h.isFailed())
	{
		os << "==============	  Hospital #" << h.hospitalID << " data   ==============" << endl;
		os << h.EPList.getCount() << " EP requests: " << h.EPList << endl;
		os << h.SPList.getCount() << " SP requests: " << h.SPList << endl;
		os << h.NPList.getCount() << " NP requests: " << h.NPList << endl;
		os << "Free Cars: " << h.SCList.getCount() << " SCars, " << h.NCList.getCount() << " NCars" << endl;
		os << "==============	Hospital #" << h.hospitalID << " data end  =============" << endl;
	}
	else
	{
		os << "==============	  Hospital #" << h.hospitalID << " data   ==============" << endl;
		os << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
		os << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
		os << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
		os << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
		os << "==============	Hospital #" << h.hospitalID << " data end  =============" << endl;
	}
	return os;
}

#endif