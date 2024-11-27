
template <typename T>
class ModifiedQ : public LinkedQueue<T>
{
public:
	bool cancelRequest(T& entry);
};

template<typename T>
bool ModifiedQ<T>::cancelRequest(T& entry)
{
	if (this->isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = this->frontPtr;

	if (this->frontPtr->getItem() == entry)
	{
		this->frontPtr = this->frontPtr->getNext();
		if (this->frontPtr == nullptr)
			this->backPtr = nullptr;
		delete nodeToDeletePtr;
		this->count--;
		return true;
	}

	while (nodeToDeletePtr->getNext() != nullptr && nodeToDeletePtr->getNext()->getItem() != entry)
	{
		nodeToDeletePtr = nodeToDeletePtr->getNext();
	}
	if (nodeToDeletePtr->getNext() != nullptr)
	{
		Node<T>* entryToDelete = nodeToDeletePtr->getNext();
		nodeToDeletePtr->setNext(entryToDelete->getNext());

		if (entryToDelete == this->backPtr)
			this->backPtr = entryToDelete;
		delete entryToDelete;
		this->count--;
		return true;
	}

	return false;
}
