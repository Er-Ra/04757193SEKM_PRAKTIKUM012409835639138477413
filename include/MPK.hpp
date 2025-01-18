#ifndef MPK_CLASS
#define MPK_CLASS

#include <iostream>
#include <chrono>
#include <vector>
#include "./sharedChannels.hpp"
#include "./sharedMutex.hpp"
#include "./initUnit.hpp"
#include "../src/messageFormats/intMessage.cpp"
#include "../src/messageFormats/strMessage.cpp"
#include "../src/messageFormats/senderFeedbackMessage.cpp"

class MPK{
public:
    static MPK* getMPK();
    //Functions
    void initializeMPK();
    void deleteChannel(int);
    void connectReceiverToChannel(int, std::vector<int>*, std::vector<int>*);
    void connectSenderToChannel(int, std::vector<int>*);
    Message* readFromChannel(int, std::string, int);
    Message* writeToChannel(int, const char*);

private:
    static MPK* myMPK;
    MPK();

    SharedChannels* sc;
    ConnectUnit* cu;
    InitUnit* iu;
};

#endif // MPK_CLASS