//Init und Connect Unit
//Die beiden können eventuell in 2 Scripts getrennt werden
#include "../include/sender.hpp"
#include "../include/receiver.hpp"
#include "../include/sharedChannels.hpp"
#include "../include/sharedMutex.hpp"
#include <iostream>
#include <chrono>
#include <vector>

//INITIALIZATION
//define grayFuncs for senders
void customSender1(SenderClass* sender){
    std::lock_guard<std::mutex> guard(sharedMutex);
    std::string message;
    message = "hello";
    sender->write(message);
}
void customSender2(SenderClass* sender){
    std::lock_guard<std::mutex> guard(sharedMutex);
    for (int i = 0; i < 10; i++){
        sender->write(std::to_string(i));
    }
}

//sender initialization
SenderClass* sender1 = new SenderClass(1, customSender1);
SenderClass* sender2 = new SenderClass(2, customSender2); 

//define grayFuncs for receivers
void customReceiver1(ReceiverClass* receiver){
    std::lock_guard<std::mutex> guard(sharedMutex);
    std::string message;
    message = receiver->read();
    std::cout << message << std::endl; 
}
void customReceiver2(ReceiverClass* receiver){
    std::lock_guard<std::mutex> guard(sharedMutex);
    std::cout << "message" << std::endl; 
}

//receiver initialization
ReceiverClass* receiver1 = new ReceiverClass(1, customReceiver1);
ReceiverClass* receiver2 = new ReceiverClass(2, customReceiver1);

//channel initialization
SharedChannels* sc = SharedChannels::getSharedChannels();

int main() {

    //LINKING
    //channel creation
    sc->addChannel(1);
    sc->addChannel(2);
    //linking senders
    sender1->joinToChannel(1);
    sender2->joinToChannel(2);

    //linking receivers
    receiver1->joinToChannel(1);
    receiver2->joinToChannel(2);

    //initialize thread vector
    std::vector<std::thread> threads;

    while (true){

        sender1->start(&threads);
        sender2->start(&threads);
        receiver1->start(&threads);
        receiver2->start(&threads); //possible to automize this with a loop. Make sender/receiver 
                                    //to have same base class, factory pattern possible

        for (auto& thread : threads) { 
            if (thread.joinable()) { thread.join(); }
        }
    }

    return 0;
}

//Missing (wichtig):
//-beliebige Nachrichtenkanäle im laufenden Betrieb zu (re)konfigurieren, anzulegen oder zu zerstören
//-Log-File
//-universellen Systemformat (Transportformat) erwartet

//Andere Anforderungen fehlen auch noch, PDF lesen 

