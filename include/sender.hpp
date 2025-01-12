#ifndef SENDER_CLASS
#define SENDER_CLASS

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include "./sharedChannels.hpp"
#include "./connectUnit.hpp"

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
    std::vector<int> joinedChannels;

    // Functions
    std::function<void(SenderClass*)> grayFunc;
};

#endif // SENDER_CLASS