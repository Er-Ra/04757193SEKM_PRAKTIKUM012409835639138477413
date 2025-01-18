#include "../../include/message.hpp"

class SenderFeedbackMessage : public Message {
    bool queueFull;
    bool messageSuccesfullySent;
    int bytesLeftOut;
public:
    SenderFeedbackMessage(bool queueFull, bool messageSuccesfullySent, int bytesLeftOut){
        this->queueFull = queueFull;
        this->messageSuccesfullySent = messageSuccesfullySent;
        this->bytesLeftOut = bytesLeftOut;
    }

    void getMessageStatus() const { 
        if(this->queueFull)
            std::cout << "Queue is full, message could not be sent" << std::endl;
        else if (this->messageSuccesfullySent)
            std::cout << "Message succesfully sent. " << "The message was cut by " << this->bytesLeftOut << " bytes. " << std::endl;
            
    } // Method to retrieve the status of the message
};
