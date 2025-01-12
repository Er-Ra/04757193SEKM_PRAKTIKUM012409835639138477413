//Init und Connect Unit
//Die beiden können eventuell in 2 Scripts getrennt werden
#include "../include/sender.hpp"
#include "../include/receiver.hpp"
#include "../include/sharedChannels.hpp"
#include "../include/sharedMutex.hpp"
#include "../include/initUnit.hpp"
#include "./messageFormats/intMessage.cpp"
#include "./messageFormats/strMessage.cpp"
#include <iostream>
#include <chrono>
#include <vector>

//INITIALIZATION
//define grayFuncs for senders
void customSender1(SenderClass* sender){
    std::string message;
    message = "hello";
    sender->write(message);
}
void customSender2(SenderClass* sender){
    std::string message;
    message = "hello2";
    sender->write(message);
}
void customSender3(SenderClass* sender){
    for (int i = 0; i < 20; i++){
        sender->write(std::to_string(i));
    }
}
//sender initialization
SenderClass* sender1 = new SenderClass(1, customSender1); //sender ID, sender Funktion
SenderClass* sender2 = new SenderClass(2, customSender3); 
SenderClass* sender3 = new SenderClass(3, customSender2); 

//define grayFuncs for receivers
void customReceiver1(ReceiverClass* receiver){
    std::lock_guard<std::mutex> guard(sharedMutex);
    StringMessage* message = (StringMessage*)receiver->read();
    std::cout << message->getValue() << std::endl; 
}
void customReceiver2(ReceiverClass* receiver){
    std::lock_guard<std::mutex> guard(sharedMutex);
    std::cout << "message" << std::endl; 
}
void customReceiver3(ReceiverClass* receiver){
    std::lock_guard<std::mutex> guard(sharedMutex);
    IntMessage* message = (IntMessage*)receiver->read();
    std::cout << "Mensaje procesado: " << message->getValue() + 1 << std::endl; 
}

//receiver initialization
ReceiverClass* receiver1 = new ReceiverClass(1, "string", customReceiver1); //receiver ID, receiver Data format, receiver Funktion
ReceiverClass* receiver2 = new ReceiverClass(2, "int", customReceiver3);

//channel initialization
InitUnit* iu = InitUnit::getInitUnit();

int main() {

    iu->initializeChannels();
    //LINKING    
    //linking senders
    sender1->joinToChannel(1); // Channel ID
    sender2->joinToChannel(2);
    sender3->joinToChannel(1);

    //linking receivers
    receiver1->joinToChannel(1); // Channel ID
    receiver2->joinToChannel(2);

    //initialize thread vector
    std::vector<std::thread> threads;
    while (true){

        sender1->start(&threads);
        sender2->start(&threads);
        sender3->start(&threads);
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

