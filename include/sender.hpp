#ifndef SENDER_CLASS
#define SENDER_CLASS

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include "./MPK.hpp"

class SenderClass {
public:
    // Constructors
    SenderClass(int, std::function<void(SenderClass*)>);

    // Destructor
    ~SenderClass();

    // Member functions
    void joinToChannel(int);
    void start(std::vector<std::thread>* threads);
    void write(const char*);

private:
    // Member variables
    MPK* myMPK;
    int senderID;
    std::vector<int> joinedChannels;

    // Functions
    std::function<void(SenderClass*)> grayFunc;
};

#endif // SENDER_CLASS