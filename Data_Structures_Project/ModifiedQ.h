#pragma once
#include "Patient.h"

class ModifiedQ : public LinkedQueue<Patient*>
{
public:
	bool cancelRequest(int patientID);
};

bool ModifiedQ::cancelRequest(int patientID)
{
	//check if the queue is empty
	if (this->isEmpty())
		return false;

	Node<Patient*>* nodeToDeletePtr = this->frontPtr;
	// check if the entry is at the front
	if (this->frontPtr->getItem()->getPatientID() == patientID)
	{
		this->frontPtr = this->frontPtr->getNext();
		if (this->frontPtr == nullptr)
			this->backPtr = nullptr;
		delete nodeToDeletePtr->getItem();
		delete nodeToDeletePtr;
		this->count--;
		return true;
	}
	// check if the entry is in the middle or at the back
	while (nodeToDeletePtr->getNext() != nullptr && nodeToDeletePtr->getNext()->getItem()->getPatientID() != patientID)
	{
		nodeToDeletePtr = nodeToDeletePtr->getNext();
	}

	if (nodeToDeletePtr->getNext() != nullptr)
	{
		Node<Patient*>* entryToDelete = nodeToDeletePtr->getNext();
		nodeToDeletePtr->setNext(entryToDelete->getNext());
		// if the entry is at the back, set the backPtr
		if (entryToDelete == this->backPtr)
			this->backPtr = entryToDelete;
		// deallocate the pointer
		delete entryToDelete->getItem();
		delete entryToDelete;
		this->count--;
		return true;
	}

	return false;
}