//RecvUnit
#include "sender.hpp"
#include "sharedMutex.hpp"

SenderClass::SenderClass(int senderID, std::function<void(SenderClass*)> grayFunc){
    this->senderID = senderID;
    this->grayFunc = grayFunc;
}

void SenderClass::joinToChannel(int channelID){
    SharedChannels* sc = SharedChannels::getSharedChannels();
    this->senderChannel = sc->getChannel(channelID); //exception for when channel does not exist
}

void SenderClass::write(std::string message){
    this->senderChannel->channelQueue->enqueue(message);
}

void SenderClass::start(std::vector<std::thread>* threads){
    threads->emplace_back(std::thread([this]() { this->grayFunc(this); }));
}