#ifndef SENDER_CLASS
#define SENDER_CLASS

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include "./sharedChannels.hpp"

class SenderClass {
public:
    // Constructors
    SenderClass(int, std::function<void(SenderClass*)>);

    // Destructor
    ~SenderClass();

    // Member functions
    void joinToChannel(int);
    void start(std::vector<std::thread>* threads);
    void write(std::string);

private:
    // Member variables
    int senderID;
    Channel* senderChannel;

    // Functions
    std::function<void(SenderClass*)> grayFunc;
};

#endif // SENDER_CLASS