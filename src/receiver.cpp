//RecvUnit
#include "../include/receiver.hpp"

ReceiverClass::ReceiverClass(int receiverID, std::string receiverFormat, std::function<void(ReceiverClass*)> grayFunc){
    this->myMPK = MPK::getMPK();
    this->receiverID = receiverID;
    this->grayFunc = grayFunc;
    this->receiverFormat = receiverFormat;
}

void ReceiverClass::joinToChannel(int channelID){
    myMPK->connectReceiverToChannel(channelID, &this->joinedChannels, &this->channelKeys);
}

Message* ReceiverClass::read(){
    Message* message;
    for (int channelNumber = 0; channelNumber < this->joinedChannels.size(); channelNumber++){
        int channelID = this->joinedChannels[channelNumber];
        int key = this->channelKeys[channelNumber];
        message = myMPK->readFromChannel(channelID, this->receiverFormat, key);
    }
    if(message != nullptr)
        return message;
    else
        return nullptr;
}

void ReceiverClass::start(std::vector<std::thread>* threads){
    threads->emplace_back(std::thread([this]() { this->grayFunc(this); }));
}

void ReceiverClass::deleteChannel(int channelID){
    myMPK->deleteChannel(channelID);
}

ReceiverClass::~ReceiverClass(){
    
}