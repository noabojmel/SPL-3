#include "readFromServer.h"
extern bool isLoggedIn;

//this thred was not used, it will be the main thred part to procced the server input
readFromServer::readFromServer(StompProtocol &protocol, ConnectionHandler &ch):protocol(protocol), ch(ch){}

void readFromServer::run(){
    while(isLoggedIn){
        string input;
        if(ch.getFrameAscii(input, '\0')){
            if(input!="")
                protocol.proccessServer(input);//proccessing the frame from the server
            }
        input="";
    }
}

readFromServer::~readFromServer() {
}