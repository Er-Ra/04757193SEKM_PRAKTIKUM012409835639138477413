//RecvUnit
#include "../include/sender.hpp"
#include "../include/sharedMutex.hpp"

SenderClass::SenderClass(int senderID, std::function<void(SenderClass*)> grayFunc){
    this->senderID = senderID;
    this->grayFunc = grayFunc;
}

void SenderClass::joinToChannel(int channelID){
    ConnectUnit* cu = ConnectUnit::getConnectUnit();
    cu->connectSenderToChannel(&this->joinedChannels, channelID);
}

void SenderClass::write(std::string message){ 
    SharedChannels* sc = SharedChannels::getSharedChannels();
    for (int channelNumber = 0; channelNumber < this->joinedChannels.size(); channelNumber++){
        int channelID = this->joinedChannels[channelNumber];
        sc->getChannel(channelID)->writeToChannel(message); 
    }
}

void SenderClass::start(std::vector<std::thread>* threads){
    threads->emplace_back(std::thread([this]() { this->grayFunc(this); }));
}