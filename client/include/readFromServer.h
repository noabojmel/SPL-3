#pragma once

#include "StompProtocol.h"

class readFromServer
{
private:
    StompProtocol &protocol;
    ConnectionHandler &ch;

public:
    readFromServer(StompProtocol &protocol, ConnectionHandler &ch);
    virtual ~readFromServer();
    void run();
};


