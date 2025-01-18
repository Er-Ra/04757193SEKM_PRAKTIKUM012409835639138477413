//MPK (System)
#include "sharedChannels.hpp"

SharedChannels* SharedChannels::mySharedChannels = nullptr;

SharedChannels* SharedChannels::getSharedChannels(){
    if(SharedChannels::mySharedChannels == nullptr){
        SharedChannels::mySharedChannels = new SharedChannels();
    }
    return SharedChannels::mySharedChannels;
}

ChannelClass* SharedChannels::getChannel(int channelID){
    for(int i = 0; i < this->channels.size();i++){
        if(channelID == this->channels[i]->get_ChannelID()){
            return this->channels[i];
        }
    }
}

std::vector<int> SharedChannels::getChannelIDs(){
    std::vector<int> channelsIDs;
    for(int i = 0; i < this->channels.size();i++){
        int channelID = this->channels[i]->get_ChannelID();
        channelsIDs.push_back(channelID);
    }
    return channelsIDs;
}

void SharedChannels::addChannel(int channelID, int slotsNumber, int slotNumber){
    QueueClass* newQueue = new QueueClass(slotsNumber, slotNumber);
    ChannelClass* newChannel = new ChannelClass(channelID, newQueue);
    channels.push_back(newChannel);
    std::cout << "Channel added with ID: " << channelID << std::endl;
}

void SharedChannels::deleteChannel(int channelID){
    for(int i = 0; i < this->channels.size();i++){
    if(channelID == this->channels[i]->get_ChannelID())
        this->channels.erase(this->channels.begin() + i);
        return ;
    }
}

SharedChannels::SharedChannels(){
    
}
