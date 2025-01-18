//RecvUnit
#include "../include/sender.hpp"

SenderClass::SenderClass(int senderID, std::function<void(SenderClass*)> grayFunc){
    this->myMPK = MPK::getMPK();
    this->senderID = senderID;
    this->grayFunc = grayFunc;
}

void SenderClass::joinToChannel(int channelID){
    myMPK->connectSenderToChannel(channelID, &this->joinedChannels);
}

void SenderClass::write(const char* message){
    for (int channelNumber = 0; channelNumber < this->joinedChannels.size(); channelNumber++){
        int channelID = this->joinedChannels[channelNumber];
        SenderFeedbackMessage* senderFeedbackMessage = (SenderFeedbackMessage*)myMPK->writeToChannel(channelID, message);
        senderFeedbackMessage->getMessageStatus();
    }
}

void SenderClass::start(std::vector<std::thread>* threads){
    threads->emplace_back(std::thread([this]() { this->grayFunc(this); }));
}

SenderClass::~SenderClass(){
    
}