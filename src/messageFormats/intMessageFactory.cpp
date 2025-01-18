#include "../../include/translatedMessageFactory.hpp"
#include "./intMessage.cpp"

class IntMessageFactory : public TranslatedMessageFactory {
public:
    bool canHandle(std::string format) const override {
        return format == "int";
    }

    Message* createMessage(const char* message) const override {
        //Logik, um die Nachticht umzusetzen
        try {
            int value = std::stoi(std::string(message)); // Attempt to convert the string to an integer
            return new IntMessage(value);
        } catch (const std::invalid_argument& e) {
            // Handle the case where the string is not a valid integer
            std::cerr << "Invalid message format: not a number. Message: " << message << std::endl;
        }
        return new IntMessage(0);
    }
};
