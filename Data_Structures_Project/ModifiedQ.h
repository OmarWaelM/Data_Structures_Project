#pragma once
class LinkedQueue;
template <typename T>
class ModifiedQ : public LinkedQueue<T>
{
public:

	bool cancelNPRequest(Patient* patient);
	
};

template<typename T>
 bool ModifiedQ<T>::cancelNPRequest(Patient* patient)
{
	 if (isEmpty())
		 return false;
	 Node<T>* nodeToDeletePtr = frontPtr;
	 frntEntry = frontPtr->getItem();
	 frontPtr = frontPtr->getNext();
	 // Queue is not empty; remove front
	 if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		 backPtr = nullptr;

	 // Free memory reserved for the dequeued node
	 delete nodeToDeletePtr;
	 count--;
	 return true;
 }
