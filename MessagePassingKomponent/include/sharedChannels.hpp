#ifndef CHANNELS_CLASS
#define CHANNELS_CLASS

#include <iostream>
#include <string>
#include <vector>
#include "./queue.hpp"

struct Channel { 
    int channelID;
    QueueClass* channelQueue;
};

class SharedChannels{
public:
    static SharedChannels* getSharedChannels();
    Channel* getChannel(int);
    void addChannel(int);
    void deleteChannel(int);

private:
    static SharedChannels* mySharedChannels;
    SharedChannels();

    std::vector<Channel> channels;
};

#endif // CHANNELS_CLASS