#include "../../include/translatedMessage.hpp"

class IntMessage : public TranslatedMessage {
    int value;
public:
    IntMessage(int val) : value(val) {}
    int getValue() const { return value; } // Method to retrieve the value
};
