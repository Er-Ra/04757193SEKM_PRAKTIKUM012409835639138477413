#include "../include/MPK.hpp"
#include "./communicatorFactory.cpp"
#include <vector>
#include <thread>
#include <iostream>
#include <chrono>

class App {
private:
    MPK* myMPK;
    std::vector<Communicator*> communicators; // Store raw pointers to senders and receivers
    std::vector<std::thread> threads;

    // Define grayFuncs for senders
    static void customSender1(SenderClass* sender) {
        while (true) {
            const char* message = "helloComoEstasLoquillooo";
            sender->write(message);
        }
    }

    static void customSender2(SenderClass* sender) {
        const char* message = "hello2";
        sender->write(message);
    }

    static void customSender3(SenderClass* sender) {
        while (true) {
            for (int i = 35; i < 100; i++) {
                std::string message = std::to_string(i);
                sender->write(message.c_str());
            }
        }
    }

    // Define grayFuncs for receivers
    static void customReceiver1(ReceiverClass* receiver) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            Message* messageReceived = receiver->read();
            if (messageReceived != nullptr) {
                StringMessage message = *(StringMessage*)messageReceived;
                std::cout << "Nachricht empfangen: " << message.getValue() << std::endl;
            }
        }
    }

    static void customReceiver2(ReceiverClass* receiver) {
        std::cout << "message" << std::endl;
    }

    static void customReceiver3(ReceiverClass* receiver) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            Message* messageReceived = receiver->read();
            if (messageReceived != nullptr) {
                IntMessage* message = (IntMessage*)receiver->read();
                std::cout << "Nachricht empfangen: " << message->getValue() + 1 << std::endl;
            }
        }
    }

public:
    // Constructor
    App() {
        myMPK = MPK::getMPK();

        // Initialize senders using factory
        communicators.push_back(CommunicatorFactory::createSender(1, customSender1));
        communicators.push_back(CommunicatorFactory::createSender(2, customSender3));

        // Initialize receivers using factory
        communicators.push_back(CommunicatorFactory::createReceiver(1, "string", customReceiver1));
        communicators.push_back(CommunicatorFactory::createReceiver(2, "int", customReceiver3));
        communicators.push_back(CommunicatorFactory::createReceiver(3, "int", customReceiver3));
    }

    // Destructor
    ~App() {
        // Clean up dynamically allocated communicators
        for (auto communicator : communicators) {
            delete communicator; // Delete each communicator
        }
    }

    void run() {
        // Initialize MPK
        myMPK->initializeMPK();

        // Linking communicators
        communicators[0]->joinToChannel(1); // Sender1 to channel 1
        communicators[1]->joinToChannel(2); // Sender2 to channel 2
        communicators[2]->joinToChannel(1); // Receiver1 to channel 1
        communicators[3]->joinToChannel(2); // Receiver2 to channel 2
        communicators[4]->joinToChannel(2); // Receiver2 to channel 2

        // Start all communicators
        for (auto communicator : communicators) {
            communicator->start(&threads);
        }

        // Keep the application running
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Join threads (cleanup on application shutdown, though this is unreachable in the current design)
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
};
