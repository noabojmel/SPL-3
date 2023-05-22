#include "keyBoardThread.h"
extern bool isLoggedIn;

keyBoardThread::keyBoardThread(StompProtocol &protocol, ConnectionHandler &ch):protocol(protocol), ch(ch){}

void keyBoardThread::run(){
    while(1){
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        int i = line.find(" ");
        if(line.substr(0, i)=="login"){
            if(!isLoggedIn){//if not log in create connection hendler
                string cmd = line.substr(i+1);
                i = cmd.find(":");
                string host = cmd.substr(0, i);
                cmd = cmd.substr(i+1);
                i=cmd.find(" ");
                short port = stoi(cmd.substr(0,i));
                ch.setValues(host, port);
                if (!ch.connect()) {
                    std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
                    //return 1;
                }
                isLoggedIn=true;
            }
        }
        protocol.proccessKeyBoard(line);//will also cover if we alredy were logged in
    }

}

keyBoardThread::~keyBoardThread() {
}