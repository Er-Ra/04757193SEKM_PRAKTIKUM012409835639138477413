#ifndef CHANNEL_CLASS
#define CHANNEL_CLASS

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include "./queue.hpp"
#include "./translatedMessageFactoryPool.hpp"

class ChannelClass{
public:
    // Constructors
    ChannelClass(int, QueueClass*);

    // Destructor
    ~ChannelClass();

    void writeToChannel(const char*);
    Message* readFromChannel(std::string, int);
    bool get_deleteRequest();
    bool get_isEmpty();
    int get_ChannelID();
    std::vector<int> get_Keys();
    void addElementToKeys(int);

private:
    int channelID;
    QueueClass* channelQueue;
    bool deleteRequest;
    bool isEmpty;
    TranslatedMessageFactoryPool* factoryPool;
    std::mutex channelMutex;
    std::vector<int> keys;
    std::vector<int> backupKeys;
};

#endif // CHANNEL_CLASS