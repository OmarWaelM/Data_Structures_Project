#pragma once
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

	 if (frontPtr->getItem() == entry)
	 {
		 frontPtr = frontPtr->getNext();
		 if (frontPtr == nullptr)  
			 backPtr = nullptr;
		 delete nodeToDeletePtr;
		 count--;
		 return true;
	 }

	 while (nodeToDeletePtr->getNext() != nullptr && nodeToDeletePtr->getNext()->getItem() != entry)
		{
			nodeToDeletePtr = nodeToDeletePtr->getNext();
		}
	 if (nodeToDeletePtr->getNext()!=nullptr)
	 {
		 Node<T>* entryToDelete = nodeToDeletePtr->getNext();
		 nodeToDeletePtr->setNext(entryToDelete->getNext());

		 if (entryToDelete == backPtr)
			 backPtr = entryToDelete;
		 delete entryToDelete;
		 count--;
		 return true;
	 }
	 
	 return false;
 }
