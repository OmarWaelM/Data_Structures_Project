#pragma once
#include "priQueue.h"

template <typename T>
class ModifiedPriQ : public priQueue<T>
{
public:
    // Cancel an item based on a specific condition
    bool cancelItemByCondition(bool (*condition)(T)) 
    {
        priNode<T>* Current = this->head;
        priNode<T>* Previous = nullptr;  //keep track of the previous node

        while (Current)
        {
            if (condition(Current->getItem())) //test the current item with the condition
            {// if item meets the condition, then it is removed
                
                if (Previous) 
                    Previous->setNext(Current->getNext());//bypassing the current node
                else 
                    this->head = Current->getNext(); //updating the head if removing the first node
               

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

