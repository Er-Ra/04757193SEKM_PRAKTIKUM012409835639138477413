//Init und Connect Unit
//Die beiden können eventuell in 2 Scripts getrennt werden
#include "sender.hpp"
#include "receiver.hpp"
#include "sharedChannels.hpp"
#include "sharedMutex.hpp"
#include "Logger.cpp"
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

//INITIALIZATION
//define grayFuncs for senders
void customSender1(SenderClass* sender){
    lock_guard<mutex> guard(sharedMutex);
    string message;
    message = "\n hello \n";
    sender->write(message);
    cout << "\nsender " << sender << "\n"<<endl;
}

void customSender2(SenderClass* sender){
    lock_guard<mutex> guard(sharedMutex);
    for (int i = 0; i < 10; i++){
        sender->write(to_string(i));
    }
    cout  << "\nsender " << sender << "\n"<<endl;
}

//sender initialization
SenderClass* sender1 = new SenderClass(1, customSender1);
SenderClass* sender2 = new SenderClass(2, customSender2); 

//define grayFuncs for receivers
void customReceiver1(ReceiverClass* receiver){
    lock_guard<mutex> guard(sharedMutex);
    string message;
    message = receiver->read();
    cout << "\n receiver " << message << "\n" << endl; 
}
void customReceiver2(ReceiverClass* receiver){
    lock_guard<mutex> guard(sharedMutex);
    cout << "\n ${message} \n" << endl; 
}

//receiver initialization
ReceiverClass* receiver1 = new ReceiverClass(1, customReceiver1);
ReceiverClass* receiver2 = new ReceiverClass(2, customReceiver1);

//channel initialization
SharedChannels* sc = SharedChannels::getSharedChannels();

int main() {

    AsyncLogger logger("async_log_linked_list.txt");

    thread t1([&logger]() { logger.thread_function("Message 1"); });
    t1.join();
    logger.thread_function("Message 2");
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
    vector<thread> threads;

    for (size_t i = 0; i < 10; i++)
    {

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