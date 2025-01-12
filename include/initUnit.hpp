#ifndef INIT_UNIT_CLASS
#define INIT_UNIT_CLASS

#include <iostream>
#include <vector>
#include <string>
#include "./sharedChannels.hpp"

class InitUnit{
public:

    static InitUnit* getInitUnit();
    //Functions
    void initializeChannels();

private:
    static InitUnit* myInitUnit;
    InitUnit();
};

#endif // INIT_UNIT_CLASS