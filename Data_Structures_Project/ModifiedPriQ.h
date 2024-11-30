#pragma once
#include "Car.h"

class ModifiedPriQ : public priQueue<Car*>
{
public:
    // Cancel an item based on a specific condition
    bool cancelRequest(int patientID, Car*& c)
    {
        priNode<Car*>* Current = this->head;
        priNode<Car*>* Previous = nullptr;  //keep track of the previous node
        int pri;
        while (Current)
        {
            //For each node in the queue, it checks whether the car's assigned patient ID matches the given patientID
            if (Current->getItem(pri)->getAssignedPatientID() == patientID) 
            {// if item meets the condition, then it is removed
                
                if (Previous) 
                    Previous->setNext(Current->getNext());//If it's not the head, update the Previous node's next pointer to skip the Current node
                else 
                    this->head = Current->getNext(); //If the Current node is the head, update the head pointer to skip this node:
               
                c = Current->getItem(pri);
                delete Current; // Free the memory of the removed node
                count--;
                return true; //item successfully removed
            }

            //Moving to the next node
            Previous = Current;
            Current = Current->getNext();
        }

        return false; // No matching item found
    } 

};

