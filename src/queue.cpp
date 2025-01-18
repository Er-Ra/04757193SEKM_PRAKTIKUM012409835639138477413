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
    this->queue->data = new char[this->slotLength + 1]; // Allocate dynamic array
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

const char* QueueClass::enqueue(const char* slot){ //Erwartet im universellen Systemformat, protokollieren, und speichern
    // Allocate memory for the return result
    char* senderFeedbackMessage = new char[6]; // 1 byte for 'isFull', 1 byte for 'sent status', 4 bytes for difference
    senderFeedbackMessage[0] = this->isFull ? '1' : '0'; // '1' if the queue is full, '0' otherwise
    if (!this->isFull && slot) {
        // Calculate the difference in length
        int slotLen = std::strlen(slot); // Assuming `slot` is a null-terminated string
        int lengthDiff = slotLen - this->slotLength;
        if (lengthDiff < 0) lengthDiff = 0;
        // Attempt to enqueue the slot

        try {
            QueueElement* newElement = new QueueElement();
            newElement->data = new char[this->slotLength + 1]; // Allocate memory for the slot data
            std::memcpy(lastElement->data, slot, this->slotLength); // Copy the slot data
            lastElement->data[this->slotLength] = '\0';
            this->lastElement->nextElement = newElement;
            this->lastElement = newElement;

            // Update queue state
            this->counter++;
            this->isFull = (this->counter == this->slotsNumber);
            this->isEmpty = false;

            senderFeedbackMessage[1] = '1'; // Message was successfully sent
        } catch (std::bad_alloc&) {
            senderFeedbackMessage[1] = '0'; // Memory allocation failed, message not sent
        }

        // Add the length difference to the result
        std::memcpy(senderFeedbackMessage + 2, &lengthDiff, sizeof(int)); // Copy the 4 bytes of the integer difference
    } else {
        senderFeedbackMessage[1] = '0'; // Queue is full, message not sent
        int lengthDiff = 0; // slotLength is effectively zero since slot wasn't used
        std::memcpy(senderFeedbackMessage + 2, &lengthDiff, sizeof(int));
    }

    return senderFeedbackMessage; // Caller is responsible for freeing this memory
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