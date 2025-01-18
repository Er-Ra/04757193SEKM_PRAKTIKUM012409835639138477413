#ifndef COMMUNICATOR_FACTORY_H
#define COMMUNICATOR_FACTORY_H

#include <functional>
#include "../include/sender.hpp"
#include "../include/receiver.hpp"

class CommunicatorFactory {
public:
    // Factory method to create a sender
    static Communicator* createSender(int senderID, std::function<void(SenderClass*)> callback) {
        return new SenderClass(senderID, callback);
    }

    // Factory method to create a receiver
    static Communicator* createReceiver(int receiverID, const std::string& format, std::function<void(ReceiverClass*)> callback) {
        return new ReceiverClass(receiverID, format, callback);
    }
};

#endif // COMMUNICATOR_FACTORY_H
