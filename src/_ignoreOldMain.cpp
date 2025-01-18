#include "../include/sender.hpp"
#include "../include/receiver.hpp"
#include "../include/sharedChannels.hpp"
#include <iostream>
#include <chrono>
#include <vector>

int main() {

std::vector<SenderClass*> senders;
std::vector<ReceiverClass*> receivers;

SharedChannels* sc = SharedChannels::getSharedChannels();

senders.push_back(new SenderClass("hola", 0));
receivers.push_back(new ReceiverClass(0));

senders[0]->start();
receivers[0]->start();

std::this_thread::sleep_for(std::chrono::milliseconds(3000));


senders[0]->stop();
receivers[0]->stop();

return 0;
}

