#pragma once
#include"Car.h"

template <typename T>
class ModifiedQ : public LinkedQueue<T>
{
public:
    // Peek at the car that will arrive first
    bool peekEarliestCar(T& earliestCar) const
    {
        Node<T>* current = this->frontPtr; // Start at the front of the queue
        if (!current) {
            return false; // Queue is empty
        }

    
        Car minCar = current->getItem();
        int minArrivalTime = minCar.getArrivalTime();
        current = current->getNext();

        // Traversing the list and finding the car with the smallest arrival time
        while (current) {
            T currentCar = current->getItem();
            if (currentCar->getArrivalTime() < minArrivalTime) {
                minCar = currentCar;
                minArrivalTime = currentCar->getArrivalTime();
            }
            current = current->getNext();
        }

        earliestCar = minCar;
        return true;
    }

    bool cancelCarByCondition(bool (*condition)(T)) 
    {
        Node<T>* current = this->frontPtr;
        Node<T>* previous = nullptr;

        while (current)
        {
            if (condition(current->getItem()))
            {
                if (previous)
                {
                    previous->setNext(current->getNext());
                }
                else 
                {
                    this->frontPtr = current->getNext(); // Removing the first node
                }

                if (current == this->backPtr) 
                {
                    this->backPtr = previous; // Update the back pointer if needed
                }

                delete current; // Free memory
                return true;
            }

            previous = current;
            current = current->getNext();
        }

        return false; // No matching car found
    }

    void displayOutCars() const
    {
        Node<T>* current = this->frontPtr;
        cout << "Out Cars List:" << endl;

        while (current)
        {
            T car = current->getItem();
            cout << "Car ID: " << car->getID()
                << ", Arrival Time: " << car->getArrivalTime() << endl;
            current = current->getNext();
        }
    }
};

