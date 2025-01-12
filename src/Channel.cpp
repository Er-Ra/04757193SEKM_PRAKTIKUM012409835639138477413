//RecvUnit
#include "../include/channel.hpp"

ChannelClass::ChannelClass(int channelID, QueueClass* channelQueue){
    this->channelID = channelID;
    this->channelQueue = channelQueue;
    this->deleteRequest = false;
    this->factoryPool = new TranslatedMessageFactoryPool();
}

void ChannelClass::writeToChannel(std::string message){
    //Bestätigen dass die Nachricht, die Universellenformat hat
    std::lock_guard<std::mutex> guard(this->channelMutex);
    if(!this->deleteRequest)
        this->channelQueue->enqueue(message);
}

bool ChannelClass::get_deleteRequest(){
    return deleteRequest;
}

bool ChannelClass::get_isEmpty(){
    return isEmpty;
}

int ChannelClass::get_ChannelID  (){
    return channelID;
}

TranslatedMessage* ChannelClass::readFromChannel(std::string receiverFormat){
    std::lock_guard<std::mutex> guard(this->channelMutex);
    std::string message;
    message = this->channelQueue->getFront();
    this->channelQueue->dequeue();

    if(this->channelQueue->get_isEmpty())
        this->isEmpty = true;

    TranslatedMessageFactory* factory = this->factoryPool->getFactory(receiverFormat);

    return factory->createMessage(message); //Die nachricht auf das Empfaenger spezifisch Format umzusestzen
}