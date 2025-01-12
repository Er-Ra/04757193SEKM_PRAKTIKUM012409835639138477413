#include "../../include/translatedMessageFactoryPool.hpp"
#include "./intMessageFactory.cpp"
#include "./strMessageFactory.cpp"

TranslatedMessageFactoryPool::TranslatedMessageFactoryPool(){
    IntMessageFactory* intMessageFactory = new IntMessageFactory();
    this->addFactory(intMessageFactory);
    StringMessageFactory* stringMessageFactory = new StringMessageFactory();
    this->addFactory(stringMessageFactory);
}

void TranslatedMessageFactoryPool::addFactory(TranslatedMessageFactory* factory){
    factories.push_back(factory);
}

TranslatedMessageFactory* TranslatedMessageFactoryPool::getFactory(std::string format){
    for (TranslatedMessageFactory* factory : this->factories) {
        if (factory->canHandle(format)) {
            return factory;
        }
    }
    throw std::runtime_error("Unsupported format: " + format);
}