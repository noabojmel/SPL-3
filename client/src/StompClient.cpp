#include <stdlib.h>
#include "../include/ConnectionHandler.h"
#include "keyBoardThread.h"
#include "readFromServer.h"
#include "StompProtocol.h"
#include <thread>
bool isLoggedIn=false;

int main(int argc, char *argv[]) {
	//if (argc < 3) {
    //    std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
     //   return -1;
    //}

    //std::string host = "127.0.0.1";
    //short port = 7777;
    
	User* user = new User();
    ConnectionHandler connectionHandler("", 0);//dump valus, will be changed when a new connection will be asked by this client
    StompProtocol protocol(connectionHandler, *user);
    keyBoardThread kbt(protocol, connectionHandler);
    std::thread th1(&keyBoardThread::run, &kbt);

    while(1){//the main thred will be the thred that procces the server input
        if(isLoggedIn){//if we are not logged in anymore we need to close the connection until the client will ask for a new one
            string input;
            if(connectionHandler.getFrameAscii(input, '\0')){
                if(input!="")
                    protocol.proccessServer(input);//proccessing the frame from the server
            }
            else{
                std::cout<<"goodbye"<<std::endl;
                break;
            }
            input="";
        }
    }
       
    delete user;
    return 0;
}