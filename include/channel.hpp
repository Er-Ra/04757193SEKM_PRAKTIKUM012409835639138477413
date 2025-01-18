#ifndef CHANNEL_CLASS
#define CHANNEL_CLASS

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include "./queue.hpp"
#include "./translatedMessageFactoryPool.hpp"
#include "./Logger.hpp"

class ChannelClass{
public:
    // Constructors
    ChannelClass(int, QueueClass*);

    // Destructor
    ~ChannelClass();

    Message* writeToChannel(const char*);
    Message* readFromChannel(std::string, int);
    bool get_deleteRequest();
    void set_deleteRequest(bool);
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
    Logger* logger;
};

#endif // CHANNEL_CLASS