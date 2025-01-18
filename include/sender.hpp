#ifndef SENDER_CLASS
#define SENDER_CLASS

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include "./MPK.hpp"
#include "./communicator.hpp"

class SenderClass : public Communicator {
public:
    // Constructors
    SenderClass(int id, std::function<void(SenderClass*)> callback);

    // Destructor
    ~SenderClass();

    // Overridden methods
    void joinToChannel(int channelID) override;
    void start(std::vector<std::thread>* threads) override;

    // Additional methods
    void write(const char* message);

private:
    MPK* myMPK;
    int senderID;
    std::vector<int> joinedChannels;
    std::function<void(SenderClass*)> grayFunc;
};

#endif // SENDER_CLASS
