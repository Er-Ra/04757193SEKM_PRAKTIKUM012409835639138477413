#ifndef QUEUE_CLASS
#define QUEUE_CLASS

#include <iostream>
#include <vector>
#include <string>

class QueueClass {
public:

    QueueClass(int,int);
    ~QueueClass();

    void enqueue(std::string);
    void dequeue(void);
    std::string getFront();

private:
    // Member variables
    int receiverID;
    std::vector<std::string> queue;
    int slotsNumber;
    int elementsPerSlot;
};

#endif // QUEUE_CLASS