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
    sc->addChannel(1,5,5); //ID, slotCount, slotLen
    sc->addChannel(2,5,5);
}

InitUnit::InitUnit(){
    
}