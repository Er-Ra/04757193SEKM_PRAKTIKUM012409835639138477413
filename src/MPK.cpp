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

void MPK::deleteChannel(int channelID){
    if(sc->getChannel(channelID) != nullptr){
        std::cout << "Deleting Channel: " << channelID << std::endl;
        sc->getChannel(channelID)->set_deleteRequest(true);
        return;
    }
    std::cout << "Channel non-existent, cannot delete Channel" << std::endl;
}

void MPK::connectReceiverToChannel(int channelID, std::vector<int>* joinedChannels, std::vector<int>* channelKeys){
    cu->connectReceiverToChannel(joinedChannels, channelKeys, channelID);
}

void MPK::connectSenderToChannel(int channelID, std::vector<int>* joinedChannels){
    cu->connectSenderToChannel(joinedChannels, channelID);
}

Message* MPK::readFromChannel(int channelID, std::string receiverFormat, int key){
    if(sc->getChannel(channelID) != nullptr){
        if(sc->getChannel(channelID)->get_isEmpty() && sc->getChannel(channelID)->get_deleteRequest()){
            sc->deleteChannel(channelID);
        }
        return sc->getChannel(channelID)->readFromChannel(receiverFormat, key);
    }
    std::cout << "Channel non-existent, cannot read from Channel" << std::endl;
    return nullptr;
}

Message* MPK::writeToChannel(int channelID, const char* message){
    if(sc->getChannel(channelID) != nullptr){
        return sc->getChannel(channelID)->writeToChannel(message);;
    }
    std::cout << "Channel non-existent, cannot write to Channel" << std::endl;
}

MPK::MPK(){
    this->sc = SharedChannels::getSharedChannels();
    this->cu = ConnectUnit::getConnectUnit();
    this->iu = InitUnit::getInitUnit();
}
