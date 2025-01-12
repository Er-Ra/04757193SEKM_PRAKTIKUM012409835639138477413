//RecvUnit
#include "../include/receiver.hpp"
#include "../include/sharedMutex.hpp"

ReceiverClass::ReceiverClass(int receiverID, std::string receiverFormat, std::function<void(ReceiverClass*)> grayFunc){
    this->receiverID = receiverID;
    this->grayFunc = grayFunc;
    this->receiverFormat = receiverFormat;
}

void ReceiverClass::joinToChannel(int channelID){
    ConnectUnit* cu = ConnectUnit::getConnectUnit();
    cu->connectReceiverToChannel(&this->joinedChannels, channelID);
}

TranslatedMessage* ReceiverClass::read(){
    SharedChannels* sc = SharedChannels::getSharedChannels();
    TranslatedMessage* message;
    for (int channelNumber = 0; channelNumber < this->joinedChannels.size(); channelNumber++){
        int channelID = this->joinedChannels[channelNumber];
        message = sc->getChannel(channelID)->readFromChannel(this->receiverFormat);
        
        if(sc->getChannel(channelID)->get_deleteRequest() == true && sc->getChannel(channelID)->get_isEmpty())
            sc->deleteChannel(channelID);
    }
    return message;
}

void ReceiverClass::start(std::vector<std::thread>* threads){
    threads->emplace_back(std::thread([this]() { this->grayFunc(this); }));
}