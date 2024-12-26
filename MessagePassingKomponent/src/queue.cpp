//Message Queue
#include "../include/queue.hpp"

QueueClass::QueueClass(int slotsNumber,int elementsPerSlot){
    this->slotsNumber = slotsNumber;
    this->elementsPerSlot = elementsPerSlot;
}

void QueueClass::enqueue(std::string slot){
    this->queue.push_back(slot);
}

void QueueClass::dequeue(){
    if(this->queue.empty()){
        //Do nothing for the moment, in the future print error
    }
    else{
        this->queue.erase(this->queue.begin());
    }
}

std::string QueueClass::getFront(){
    if(this->queue.empty()){
        //Do nothing for the moment, in the future print error
        return "";
    }
    else{
        return this->queue.front();
    }
}