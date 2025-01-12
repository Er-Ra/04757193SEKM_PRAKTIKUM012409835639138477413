#ifndef CHANNELS_CLASS
#define CHANNELS_CLASS

#include <iostream>
#include <string>
#include <vector>
#include "./queue.hpp"
#include "./channel.hpp"
#include "./connectUnit.hpp"

class SharedChannels{
public:
    static SharedChannels* getSharedChannels();
    ChannelClass* getChannel(int);
    std::vector<int> getChannelIDs();
    void addChannel(int, int, int);
    void deleteChannel(int);

private:
    static SharedChannels* mySharedChannels;
    SharedChannels();

    std::vector<ChannelClass*> channels;
};

#endif // CHANNELS_CLASS