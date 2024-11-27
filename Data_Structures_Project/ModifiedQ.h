
template <typename T>
class ModifiedQ : public LinkedQueue<T>
{
public:
	bool cancelRequest(T& entry);
};

template<typename T>
bool ModifiedQ<T>::cancelRequest(T& entry)
{
	//check if the queue is empty
	if (this->isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = this->frontPtr;
	// check if the entry is at the front
	if (this->frontPtr->getItem() == entry)
	{
		this->frontPtr = this->frontPtr->getNext();
		if (this->frontPtr == nullptr)
			this->backPtr = nullptr;
		delete nodeToDeletePtr;
		this->count--;
		return true;
	}
	// check if the entry is in the middle or at the back
	while (nodeToDeletePtr->getNext() != nullptr && nodeToDeletePtr->getNext()->getItem() != entry)
	{
		nodeToDeletePtr = nodeToDeletePtr->getNext();
	}

	if (nodeToDeletePtr->getNext() != nullptr)
	{
		Node<T>* entryToDelete = nodeToDeletePtr->getNext();
		nodeToDeletePtr->setNext(entryToDelete->getNext());
		// if the entry is at the back, set the backPtr
		if (entryToDelete == this->backPtr)
			this->backPtr = entryToDelete;
		// deallocate the pointer
		delete entryToDelete;
		this->count--;
		return true;
	}

	return false;
}
