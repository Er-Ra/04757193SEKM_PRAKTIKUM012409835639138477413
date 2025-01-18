#ifndef RECEIVER_CLASS
#define RECEIVER_CLASS

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include "./MPK.hpp"

class ReceiverClass {
public:
    // Constructors
    ReceiverClass(int, std::string, std::function<void(ReceiverClass*)>);

    // Destructor
    ~ReceiverClass();

    // Member functions
    void joinToChannel(int);
    void start(std::vector<std::thread>* threads);
    Message* read();
    void deleteChannel(int);
    
private:
    // Variables
    MPK* myMPK;
    int receiverID;
    std::vector<int> joinedChannels;
    std::vector<int> channelKeys;
    std::string receiverFormat;
    Message* receiverMessage;

    // Functions
    std::function<void(ReceiverClass*)> grayFunc;
};

#endif // RECEIVER_CLASS