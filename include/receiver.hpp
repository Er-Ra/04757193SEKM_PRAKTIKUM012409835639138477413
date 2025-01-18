#ifndef RECEIVER_CLASS
#define RECEIVER_CLASS

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include "./MPK.hpp"
#include "./communicator.hpp"

class ReceiverClass : public Communicator {
public:
    // Constructors
    ReceiverClass(int id, std::string format, std::function<void(ReceiverClass*)> callback);

    // Destructor
    ~ReceiverClass();

    // Overridden methods
    void joinToChannel(int channelID) override;
    void start(std::vector<std::thread>* threads) override;

    // Additional methods
    Message* read();
    void deleteChannel(int channelID);

private:
    MPK* myMPK;
    int receiverID;
    std::vector<int> joinedChannels;
    std::vector<int> channelKeys;
    std::string receiverFormat;
    Message* receiverMessage;
    std::function<void(ReceiverClass*)> grayFunc;
};

#endif // RECEIVER_CLASS
