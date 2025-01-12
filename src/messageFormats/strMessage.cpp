#include "../../include/translatedMessage.hpp"
#include <iostream>
#include <string>

class StringMessage : public TranslatedMessage {
    std::string value;
public:
    StringMessage(const std::string val) : value(val) {}
    const std::string getValue() const { return value; } // Method to retrieve the value
};
