//RecvUnit
#include "../include/channel.hpp"

ChannelClass::ChannelClass(int channelID, QueueClass* channelQueue){
    this->channelID = channelID;
    this->channelQueue = channelQueue;
    this->deleteRequest = false;
    this->factoryPool = new TranslatedMessageFactoryPool();
}

ChannelClass::~ChannelClass(){
    delete this->factoryPool;
    delete this-> channelQueue;    
}

bool ChannelClass::get_deleteRequest(){
    return deleteRequest;
}

void ChannelClass::set_deleteRequest(bool deleteRequest){
    this->deleteRequest = deleteRequest;
}

bool ChannelClass::get_isEmpty(){
    return isEmpty;
}

int ChannelClass::get_ChannelID(){
    return channelID;
}

std::vector<int> ChannelClass::get_Keys(){
    return this->keys;
}

void ChannelClass::addElementToKeys(int element){
    this->keys.push_back(element);
    this->backupKeys.push_back(element);
}


Message* ChannelClass::writeToChannel(const char* message){
    std::lock_guard<std::mutex> guard(this->channelMutex);
    TranslatedMessageFactory* factory = this->factoryPool->getFactory("senderFeedbackMessage");
    if(!this->deleteRequest){
        const char* senderFeedbackMessage;
        senderFeedbackMessage = this->channelQueue->enqueue(message);
        this->isEmpty = false;
        return factory->createMessage(senderFeedbackMessage);
    }
        //from here on the message can be deleted, because it is now safe in the queue
}

Message* ChannelClass::readFromChannel(std::string receiverFormat, int key){
    std::lock_guard<std::mutex> guard(this->channelMutex);
    TranslatedMessageFactory* factory = this->factoryPool->getFactory(receiverFormat);
    if(!this->channelQueue->get_isEmpty()){
        const char* message;

        int position = -1;
        int index = 0;
        for (int num : this->keys) {
            if (num == key) {
                position = index;
                break;
            }
            index++;
        }
        if (position != -1){
            message = this->channelQueue->getFront();
            this->keys.erase(this->keys.begin() + position);
        }
        if (this->keys.empty()){
            this->channelQueue->dequeue();
            this->keys = this->backupKeys;
        }
        if(this->channelQueue->get_isEmpty())
            this->isEmpty = true;
        
        return factory->createMessage(message);
    }
    return factory->createMessage("0");
     //Die nachricht auf das Empfaenger spezifisch Format umzusestzen
}

