#ifndef RECEIVER_CLASS
#define RECEIVER_CLASS

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include "./sharedChannels.hpp"

class ReceiverClass {
public:
    // Constructors
    ReceiverClass(int, std::function<void(ReceiverClass*)>);

    // Destructor
    ~ReceiverClass();

    // Member functions
    void joinToChannel(int);
    void start(std::vector<std::thread>* threads);
    std::string read();
    
private:
    // Variables
    int receiverID;
    Channel* receiverChannel;

    // Functions
    std::function<void(ReceiverClass*)> grayFunc;
};

#endif // RECEIVER_CLASS