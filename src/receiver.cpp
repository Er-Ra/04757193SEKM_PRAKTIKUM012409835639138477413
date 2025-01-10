//RecvUnit
#include "receiver.hpp"
#include "sharedChannels.hpp"
#include "sharedMutex.hpp"

ReceiverClass::ReceiverClass(int receiverID, std::function<void(ReceiverClass*)> grayFunc){
    this->receiverID = receiverID;
    this->grayFunc = grayFunc;
}

void ReceiverClass::joinToChannel(int channelID){
    SharedChannels* sc = SharedChannels::getSharedChannels();
    this->receiverChannel = sc->getChannel(channelID); //exception for when channel does not exist
}

std::string ReceiverClass::read(){
    std::string message;
    message = this->receiverChannel->channelQueue->getFront();
    this->receiverChannel->channelQueue->dequeue();
    return message;
}

void ReceiverClass::start(std::vector<std::thread>* threads){
    threads->emplace_back(std::thread([this]() { this->grayFunc(this); }));
}