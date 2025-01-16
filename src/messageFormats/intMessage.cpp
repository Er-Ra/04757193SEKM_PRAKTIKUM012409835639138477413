#include "../../include/message.hpp"

class IntMessage : public Message {
    int value;
public:
    IntMessage(int val) : value(val) {}
    int getValue() const { return value; } // Method to retrieve the value
};
