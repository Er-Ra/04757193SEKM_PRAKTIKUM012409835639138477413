//MPK (System)
#include "sharedChannels.hpp"

SharedChannels* SharedChannels::mySharedChannels = nullptr;

SharedChannels* SharedChannels::getSharedChannels(){
    if(SharedChannels::mySharedChannels == nullptr){
        SharedChannels::mySharedChannels = new SharedChannels();
    }
    return SharedChannels::mySharedChannels;
}

Channel* SharedChannels::getChannel(int channelID){
    for(int i = 0; i < this->channels.size();i++){
        if(channelID == this->channels[i].channelID){
            return &(this->channels[i]);
        }
    }
}

void SharedChannels::addChannel(int channelID){
    QueueClass* newQueue = new QueueClass(5,1);
    Channel newChannel = {channelID, newQueue};
    channels.push_back(newChannel);
    std::cout << "Channel added with ID: " << channelID << std::endl;
}

void SharedChannels::deleteChannel(int channelID){
        for(int i = 0; i < this->channels.size();i++){
        if(channelID == this->channels[i].channelID){
            this->channels.erase(this->channels.begin() + i);
            return ;
        }
    }
}

SharedChannels::SharedChannels(){

}
