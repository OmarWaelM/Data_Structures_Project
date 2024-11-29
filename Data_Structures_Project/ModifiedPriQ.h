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
            if (Current->getItem(pri)->getAssignedPatientID() == patientID) //test the current item with the condition
            {// if item meets the condition, then it is removed
                
                if (Previous) 
                    Previous->setNext(Current->getNext());//bypassing the current node
                else 
                    this->head = Current->getNext(); //updating the head if removing the first node
               
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

