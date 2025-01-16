#ifndef QUEUE_CLASS
#define QUEUE_CLASS

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

struct QueueElement {
    char* data;  // A single character
    QueueElement* nextElement;  // Pointer to a dynamically allocated string
};

class QueueClass {
public:

    QueueClass(int,int);
    ~QueueClass();

    //Functions
    void enqueue(const char*); //in die Queue schreiben
    void dequeue(void); //von der Queue löschen
    const char* getFront(); //von der Queue lesen
    bool get_isEmpty();
    bool get_isFull();
    void printQueue();

private:
    // Member variables
    void set_isEmpty(bool);
    QueueElement* queue;
    QueueElement* lastElement;
    int slotsNumber;
    int slotLength;
    int counter;
    bool isEmpty;
    bool isFull;
};

#endif // QUEUE_CLASS