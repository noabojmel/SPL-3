#pragma once
#include <string>
#include <vector>
#include "event.h"
using std::string;
using std::vector;


//this class will recieve a string and convert it to a frame format

class Frame
{
private:
    string input;//the command line from the terminal
    string command;//the command without the first word - just the headers data
    string type;//the first word of the command that indicates the type of the massage 
    string gameName;
public:
    Frame(string &input);
    virtual ~Frame();
    string getFrameType();
    string connectFrame();
    string sendFrame();
    string subscribeFrame(int subId, int recId);
    string unsubscribeFrame(int subId, int recId);
    string disconnectFrame(int recId);
    string getUnsubTopic();//assuming this is an unsuvscribe frame, the func will return the topic that the user want to disconnect from
    string& getCommand();
    vector<string> reportFrame(string file, string user);
    string findName();//assuming this is a login command, this will return the user name
};


