#include "../../include/translatedMessageFactory.hpp"
#include "./intMessage.cpp"

class IntMessageFactory : public TranslatedMessageFactory {
public:
    bool canHandle(std::string format) const override {
        return format == "int";
    }

    TranslatedMessage* createMessage(std::string message) const override {
        //Logik, um die Nachticht umzusetzen
        return new IntMessage(std::stoi(message));
    }
};
