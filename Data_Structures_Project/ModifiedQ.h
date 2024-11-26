#pragma once
class LinkedQueue;
template <typename T>
class ModifiedQ : public LinkedQueue<T>
{
public:
	bool cancelRequest(T& entry);
};

template<typename T>
 bool ModifiedQ<T>::cancelRequest(T& entry)
{
	 if (isEmpty())
		 return false;

	 Node<T>* nodeToDeletePtr = frontPtr;

	 if (nodeToDeletePtr->getItem() == entry)
	 {
		 frontPtr = nodeToDeletePtr->getNext();
		 delete nodeToDeletePtr;
		 return true;
	 }

	 while (nodeToDeletePtr->getNext()->getItem() != entry)
		{
			nodeToDeletePtr = nodeToDeletePtr->getNext();
		}

	 Node<T>* entryToDelete = nodeToDeletePtr->getNext();
	 nodeToDeletePtr->setNext(entryToDelete->getNext());
	 
	 if (entryToDelete == backPtr)
		 backPtr = entryToDelete;

	 delete nodeToDeletePtr;
	 return true;
 }
