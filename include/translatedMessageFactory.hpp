#ifndef TRANSLATED_MESSAGE_FACTORY_CLASS
#define TRANSLATED_MESSAGE_FACTORY_CLASS

#include <iostream>
#include <string>
#include "./translatedMessage.hpp"

class TranslatedMessageFactory {
public:
    virtual ~TranslatedMessageFactory() = default;

    // Returns true if the factory can handle the format
    virtual bool canHandle(std::string) const = 0;

    // Creates the TranslatedMessage for the given format
    virtual TranslatedMessage* createMessage(std::string) const = 0;
};

#endif // TRANSLATED_MESSAGE_FACTORY_CLASS