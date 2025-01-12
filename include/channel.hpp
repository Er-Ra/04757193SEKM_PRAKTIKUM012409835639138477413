#ifndef CHANNEL_CLASS
#define CHANNEL_CLASS

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include "./queue.hpp"
#include "../include/translatedMessageFactoryPool.hpp"

class ChannelClass{
public:
    // Constructors
    ChannelClass(int, QueueClass*);

    // Destructor
    ~ChannelClass();

    void writeToChannel(std::string);
    TranslatedMessage* readFromChannel(std::string);
    bool get_deleteRequest();
    bool get_isEmpty();
    int get_ChannelID();

private:
    int channelID;
    QueueClass* channelQueue;
    bool deleteRequest;
    bool isEmpty;
    TranslatedMessageFactoryPool* factoryPool;
    std::mutex channelMutex;
};

#endif // CHANNEL_CLASS