#include "sender.hpp"
#include "receiver.hpp"
#include "MPK.hpp"
#include <vector>
#include <thread>
#include <iostream>
#include <chrono>

class App {
private:
    MPK* myMPK;
    SenderClass* sender1;
    SenderClass* sender2;
    ReceiverClass* receiver1;
    ReceiverClass* receiver2;
    ReceiverClass* receiver3;
    std::vector<std::thread> threads;

    // Define grayFuncs for senders
    static void customSender1(SenderClass* sender) {
        while (true) {
            const char* message;
            message = "hello";
            sender->write(message);
        }
    }

    static void customSender2(SenderClass* sender) {
        const char* message;
        message = "hello2";
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
            StringMessage message = *(StringMessage*)receiver->read();
            std::cout << "Mensaje recibido: " << message.getValue() << std::endl;
        }
    }

    static void customReceiver2(ReceiverClass* receiver) {
        std::cout << "message" << std::endl;
    }

    static void customReceiver3(ReceiverClass* receiver) {
        while (true) {
            IntMessage* message = (IntMessage*)receiver->read();
            std::cout << "Mensaje procesado: " << message->getValue() + 1 << std::endl;
        }
    }

public:
    // Constructor
    App() {
        myMPK = MPK::getMPK();

        // Initialize senders
        sender1 = new SenderClass(1, customSender1);
        sender2 = new SenderClass(2, customSender3);

        // Initialize receivers
        receiver1 = new ReceiverClass(1, "string", customReceiver1);
        receiver2 = new ReceiverClass(2, "int", customReceiver3);
        receiver3 = new ReceiverClass(2, "int", customReceiver3);
    }

    // Destructor to clean up resources
    ~App() {
        delete sender1;
        delete sender2;
        delete receiver1;
        delete receiver2;
        delete receiver3;
    }

    void run() {
        // Initialize MPK
        myMPK->initializeMPK();

        // Linking senders
        sender1->joinToChannel(1); // Channel ID
        sender2->joinToChannel(2);

        // Linking receivers
        receiver1->joinToChannel(1); // Channel ID
        receiver2->joinToChannel(2);
        receiver3->joinToChannel(2);

        // Start threads
        sender1->start(&threads);
        sender2->start(&threads);
        receiver1->start(&threads);
        receiver2->start(&threads);
        receiver3->start(&threads);

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
