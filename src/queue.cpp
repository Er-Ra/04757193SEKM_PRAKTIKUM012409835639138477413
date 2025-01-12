//Message Queue
#include "../include/queue.hpp"
#include "../include/sharedMutex.hpp"

QueueClass::QueueClass(int slotsNumber,int slotLength){
    this->slotsNumber = slotsNumber;
    this->slotLength = slotLength;
    this->isEmpty = false;
}

bool QueueClass::get_isEmpty(){ //Erwartet im universellen Systemformat, protokollieren, und speichern
    return this->isEmpty;
}

void QueueClass::set_isEmpty(bool isEmpty){ //Erwartet im universellen Systemformat, protokollieren, und speichern
    this->isEmpty = isEmpty;
}

void QueueClass::enqueue(std::string slot){ //Erwartet im universellen Systemformat, protokollieren, und speichern
    //if slotLength < slot.length()
    this->queue.push_back(slot);
    if(this->get_isEmpty()){
        this->set_isEmpty(false);           //Lässt andere Classen wissen, wenn ein Channel nicht leer ist
    }
}

void QueueClass::dequeue(){ 
    if(this->queue.empty()){
        //Do nothing for the moment, in the future print error
    }
    else{
        this->queue.erase(this->queue.begin());
        if(this->queue.empty()){
            this->set_isEmpty(true);           //Lässt andere Classen wissen, wenn ein Channel leer ist
        }
    }
}

std::string QueueClass::getFront(){ //in das spezifisch für den Empfänger umsetzen
    if(this->queue.empty()){
        //Do nothing for the moment, in the future print error
        return "";
    }
    else{
        return this->queue.front();
    }
}