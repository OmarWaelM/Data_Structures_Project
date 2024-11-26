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
	 return true;
 }
