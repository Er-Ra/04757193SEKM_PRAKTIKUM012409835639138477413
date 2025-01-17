#include "../../include/translatedMessageFactory.hpp"
#include "./strMessage.cpp"

class StringMessageFactory : public TranslatedMessageFactory {
public:
    bool canHandle(std::string format) const override {
        return format == "string";
    }

    Message* createMessage(const char* message) const override {
        //Logik, um die Nachticht umzusetzen
        return new StringMessage(std::string(message));
    }
};
