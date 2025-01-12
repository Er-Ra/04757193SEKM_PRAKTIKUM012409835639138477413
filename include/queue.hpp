#ifndef QUEUE_CLASS
#define QUEUE_CLASS

#include <iostream>
#include <vector>
#include <string>

class QueueClass {
public:

    QueueClass(int,int);
    ~QueueClass();

    //Functions
    void enqueue(std::string); //in die Queue schreiben
    void dequeue(void); //von der Queue löschen
    std::string getFront(); //von der Queue lesen
    bool get_isEmpty();

private:
    // Member variables
    void set_isEmpty(bool);
    std::vector<std::string> queue;
    int slotsNumber;
    int slotLength;
    bool isEmpty;
};

#endif // QUEUE_CLASS