#ifndef TRANSLATED_MESSAGE_FACTORY_POOL_CLASS
#define TRANSLATED_MESSAGE_FACTORY_POOL_CLASS

#include <iostream>
#include <string>
#include <vector>
#include "./translatedMessageFactory.hpp"

class TranslatedMessageFactoryPool {
private:
    std::vector<TranslatedMessageFactory*> factories;

public:
    TranslatedMessageFactoryPool();
    void addFactory(TranslatedMessageFactory*);
    TranslatedMessageFactory* getFactory(std::string);

};

#endif // TRANSLATED_MESSAGE_FACTORY_POOL_CLASS