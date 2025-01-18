#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <vector>
#include <thread>

class Communicator {
public:
    virtual ~Communicator() = default;

    // Pure virtual methods
    virtual void joinToChannel(int channelID) = 0;
    virtual void start(std::vector<std::thread>* threads) = 0;
};

#endif // COMMUNICATOR_H
