#pragma once
#include "StompProtocol.h"
#include "ConnectionHandler.h"

class keyBoardThread
{
private:
    StompProtocol &protocol;
    ConnectionHandler &ch;
public:
    keyBoardThread(StompProtocol &protocol, ConnectionHandler &ch);
    void run();
    virtual ~keyBoardThread();
};

