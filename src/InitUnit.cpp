#include "../include/initUnit.hpp"

InitUnit* InitUnit::myInitUnit = nullptr;

InitUnit* InitUnit::getInitUnit(){
    if(InitUnit::myInitUnit == nullptr){
        InitUnit::myInitUnit = new InitUnit();
    }
    return InitUnit::myInitUnit;
}

void InitUnit::initializeChannels(){
    SharedChannels* sc = SharedChannels::getSharedChannels();
    sc->addChannel(1,10,10); //ID, slotCount, slotLen
    sc->addChannel(2,10,10);
}

InitUnit::InitUnit(){
    
}