//MPK (System)
#include "../include/MPK.hpp"

MPK* MPK::myMPK = nullptr;

MPK* MPK::getMPK(){
    if(MPK::myMPK == nullptr){
        MPK::myMPK = new MPK();
    }
    return MPK::myMPK;
}

void MPK::initializeMPK(){
    iu->initializeChannels();
}

void MPK::connectReceiverToChannel(int channelID, std::vector<int>* joinedChannels, std::vector<int>* channelKeys){
    cu->connectReceiverToChannel(joinedChannels, channelKeys, channelID);
}

void MPK::connectSenderToChannel(int channelID, std::vector<int>* joinedChannels){
    cu->connectSenderToChannel(joinedChannels, channelID);

}

Message* MPK::readFromChannel(int channelID, std::string receiverFormat, int key){
    return sc->getChannel(channelID)->readFromChannel(receiverFormat, key);
}

void MPK::writeToChannel(int channelID, const char* message){
    sc->getChannel(channelID)->writeToChannel(message); 
}

MPK::MPK(){
    this->sc = SharedChannels::getSharedChannels();
    this->cu = ConnectUnit::getConnectUnit();
    this->iu = InitUnit::getInitUnit();
}
