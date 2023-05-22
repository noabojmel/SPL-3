#pragma once
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "ConnectionHandler.h"
#include "Frame.h"
#include "User.h"
#include "Game.h"
#include "receivedFrames.h"

class ConnectionHandler;

// TODO: implement the STOMP protocol
class StompProtocol
{
private:
    ConnectionHandler &ch;
    User& user;
    bool connected;
    unordered_map<string, Game*> games;
public:
    StompProtocol(ConnectionHandler& connectionHandler, User& user);
    void proccessKeyBoard(string & input);
    void proccessServer(string & input);
    bool isConnected();
    string report(string frame);

};
