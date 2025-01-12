#ifndef RECEIVER_CLASS
#define RECEIVER_CLASS

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include "./sharedChannels.hpp"
#include "./connectUnit.hpp"
#include "./translatedMessage.hpp"

class ReceiverClass {
public:
    // Constructors
    ReceiverClass(int, std::string, std::function<void(ReceiverClass*)>);

    // Destructor
    ~ReceiverClass();

    // Member functions
    void joinToChannel(int);
    void start(std::vector<std::thread>* threads);
    TranslatedMessage* read();
    
private:
    // Variables
    int receiverID;
    std::vector<int> joinedChannels;
    std::string receiverFormat;
    TranslatedMessage* receiverMessage;

    // Functions
    std::function<void(ReceiverClass*)> grayFunc;
};

#endif // RECEIVER_CLASS