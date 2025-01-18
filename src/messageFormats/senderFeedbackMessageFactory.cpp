#include "../../include/translatedMessageFactory.hpp"
#include "./senderFeedbackMessage.cpp"

class SenderFeedbackMessageFactory : public TranslatedMessageFactory {
public:
    bool canHandle(std::string format) const override {
        return format == "senderFeedbackMessage";
    }

    Message* createMessage(const char* message) const override {
        //Logik, um die Nachticht umzusetzen
        bool queueFull;
        bool messageSuccesfullySent;
        int bytesLeftOut;

        if(message[0] == '1')
            queueFull = true;
        if(message[1] == '1')
            messageSuccesfullySent = true;
        std::memcpy(&bytesLeftOut, message + 2, sizeof(int));
        
        return new SenderFeedbackMessage(queueFull, messageSuccesfullySent, bytesLeftOut);
    }
};
