#ifndef CONNECT_UNIT_CLASS
#define CONNECT_UNIT_CLASS

#include <iostream>
#include <vector>
#include <string>
#include "./sharedChannels.hpp"

class ConnectUnit{
public:

    static ConnectUnit* getConnectUnit();
    //Functions
    void connectReceiverToChannel(std::vector<int>*, std::vector<int>*, int);
    void connectSenderToChannel(std::vector<int>*, int);

private:
    static ConnectUnit* myConnectUnit;
    ConnectUnit();
};

#endif // CONNECT_UNIT_CLASS