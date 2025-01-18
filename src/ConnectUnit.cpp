#include "../include/connectUnit.hpp"

ConnectUnit* ConnectUnit::myConnectUnit = nullptr;

ConnectUnit* ConnectUnit::getConnectUnit(){
    if(ConnectUnit::myConnectUnit == nullptr){
        ConnectUnit::myConnectUnit = new ConnectUnit();
    }
    return ConnectUnit::myConnectUnit;
}

void ConnectUnit::connectReceiverToChannel(std::vector<int>* receiverVector, std::vector<int>* keyVector,int channelID){
    SharedChannels* sc = SharedChannels::getSharedChannels();
    std::vector<int> channelIDs;
    bool channelExists = false;
    channelIDs = sc->getChannelIDs();
    for(int channelNumber = 0; channelNumber < channelIDs.size(); channelNumber++){
        if(channelID == channelIDs[channelNumber]){
            channelExists = true;
            receiverVector->push_back(channelID);
            std::vector<int> channelKeyVector;
            channelKeyVector = sc->getChannel(channelID)->get_Keys();
            int keyToAdd = channelKeyVector.size() + 1;
            sc->getChannel(channelID)->addElementToKeys(keyToAdd);
            keyVector->push_back(keyToAdd);
        }
    }
    if(!channelExists)
        std::cout << "Channel with ID " << channelID << " is non-existant" << std::endl;
        //add eventually C2b
}

void ConnectUnit::connectSenderToChannel(std::vector<int>* senderVector, int channelID){
    SharedChannels* sc = SharedChannels::getSharedChannels();
    if(sc->getChannel(channelID) != nullptr){
        senderVector->push_back(channelID);
    }
    else
        std::cout << "Channel with ID " << channelID << " is non-existant" << std::endl;
        //add eventually C2b
}

void ConnectUnit::separateReceiverFromChannel(std::vector<int>* receiverVector, std::vector<int>* keyVector,int channelID){

}

void ConnectUnit::separateSenderFromChannel(std::vector<int>* senderVector, int channelID){

}

ConnectUnit::ConnectUnit(){
    
}