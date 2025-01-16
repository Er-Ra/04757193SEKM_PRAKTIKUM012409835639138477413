//Message Queue
#include "../include/queue.hpp"
#include "../include/sharedMutex.hpp"

QueueClass::QueueClass(int slotsNumber, int slotLength){
    this->isEmpty = true;
    this->isFull = false;
    this->counter = 0;
    this->slotLength = slotLength;
    this->slotsNumber = slotsNumber;
    this->queue = new QueueElement();
    this->queue->data = new char[this->slotLength]; // Allocate dynamic array
    this->lastElement = this->queue;
}

QueueClass::~QueueClass() {
    while (!isEmpty) {
        dequeue();
    }
    delete[] queue->data; // Clean up initial element
    delete queue;
}

bool QueueClass::get_isEmpty(){ //Erwartet im universellen Systemformat, protokollieren, und speichern
    return this->isEmpty;
}

void QueueClass::set_isEmpty(bool isEmpty){ //Erwartet im universellen Systemformat, protokollieren, und speichern
    this->isEmpty = isEmpty;
}

void QueueClass::enqueue(const char* slot){ //Erwartet im universellen Systemformat, protokollieren, und speichern
    //if slotLength < slot.length()
    if (!this->isFull && slot){

        QueueElement* newElement = new QueueElement();
        this->lastElement->nextElement = newElement;
        newElement->data = new char[this->slotLength];
        std::memcpy(newElement->data, slot, this->slotLength);
        this->lastElement = newElement;

        this->counter += 1;
        //this->printQueue();
        if(this->counter == this->slotsNumber){
            this->isFull = true;
        }
        if(this->isEmpty){
            this->isEmpty = false;     //Lässt andere Classen wissen, wenn ein Channel nicht leer ist
        }
    }
}

void QueueClass::dequeue(){ 
    if(this->isEmpty){
        //Do nothing for the moment, in the future print error
    }
    else{
        QueueElement* elementToEliminate = this->queue;
        this->queue = this->queue->nextElement;
        delete[] elementToEliminate->data;  // Delete the char array first
        delete elementToEliminate;          // Then delete the element        
        this->counter -= 1;
        if(this->counter == 0) {
            this->isEmpty = true;
            this->lastElement = this->queue;
        }
    }
    if(this->isFull){
        this->isFull = false;     //Lässt andere Classen wissen, wenn ein Channel nicht leer ist
    }
}

const char* QueueClass::getFront(){ //in das spezifisch für den Empfänger umsetzen
    if(this->isEmpty){
        //Do nothing for the moment, in the future print error
        return "0";
    }
    else{
        return this->queue->data;
    }
}

void QueueClass::printQueue(){
    if(!this->isEmpty){
        QueueElement* current = this->queue;
        int position = 0;
        
        std::cout << "Queue contents:";
        std::cout << "[ ";
        while(current->nextElement != nullptr) {  // Start from the first actual element
            std::cout << current->nextElement->data << " ";
            current = current->nextElement;
            position++;
        }
        std::cout << "]" << std::endl;
    }
}