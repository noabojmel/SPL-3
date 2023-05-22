#include "Frame.h"
using std::vector;
#include "vector"
#include <fstream>

Frame::Frame(string &input):input(input), command(""), type(""), gameName(""){
    string start = "";
    int counter = 0;
    bool stop = false;
    for(int i = 0; i<(int)input.length()&&!stop; i++){

        if(input[i]==' ')//found the first word of the command
            stop=true;

       //collecting the first word from the command to indicate the massage's type
       else if(i!=(int)input.length()-1)
           start.append(&input[i],1);
        
       else if(i==(int)input.length()-1){//for commands with one word
           start.append(&input[i],1);
           stop=true;
       }
       counter++;
    }
    type = start;
    command = input.substr(counter);//the command without the first word
}

string Frame::getFrameType(){
    return type;
}

string Frame::connectFrame(){
    string op = "CONNECT\n";
    string nextHeader;
    string restOfCmd = command;
    int findNextHeader = command.find(" ");

    nextHeader = restOfCmd.substr(0, findNextHeader);
    op+="host:"+nextHeader+'\n';
    restOfCmd = restOfCmd.substr(findNextHeader+1);
    findNextHeader = restOfCmd.find(" ");
    nextHeader = restOfCmd.substr(0, findNextHeader);
    op+="login:"+nextHeader+'\n';
    restOfCmd = restOfCmd.substr(findNextHeader+1);
    op+="passcode:"+restOfCmd+'\n';
    op+="accept-version:1.2\n";
    op+='\0';

    return op;
}

string Frame::subscribeFrame(int subId, int recId){
    string op = "SUBSCRIBE\n";
    op+="destination:"+command+'\n';
    string header="";
    string subIdString = std::to_string(subId);
    string recIdString = std::to_string(recId);
    op+="id:"+subIdString+'\n';
    op+="receipt:"+recIdString+'\n';
    op+='\0';
    return op;
}


string& Frame::getCommand(){//the whole input without the first word
    return command;
}

string Frame::unsubscribeFrame(int subId, int recId){
    string op = "UNSUBSCRIBE\n";
    string ssId = std::to_string(subId);
    string srId = std::to_string(recId);
    op+="id:"+ssId+'\n';
    op+="receipt:"+srId+'\n';
    op+='\0';

    return op;
}

string Frame::disconnectFrame(int recId){
    string op = "DISCONNECT\n";
    string recIdString = std::to_string(recId);
    op+="receipt:"+recIdString+'\n';
    op+='\0';

    return op;
}

/*string Frame::getUnsubTopic(){
    string op="";
    if(type=="exit"){

    }
}*/

vector<string> Frame::reportFrame(string fName, string user){
    std::vector<string> opEvents;
    names_and_events newEvents = parseEventsFile(fName);
    vector<Event> eventsVector = newEvents.events;
    for(Event event : eventsVector){
        string op = "SEND\n";
        op+="destination:"+newEvents.team_a_name+"_"+newEvents.team_b_name+'\n';
        op+="user: "+user+'\n';
        op+="team a: "+newEvents.team_a_name+'\n';
        op+="team b: "+newEvents.team_b_name+'\n';
        op+="event name: "+event.get_name()+'\n';
        op+="time: "+std::to_string(event.get_time())+'\n';
        op+=event.eventString();
        op+="description:\n"+event.get_discription()+"\n\0";
        opEvents.push_back(op);
    }
    return opEvents;
}

string Frame::findName(){
    string userPass = command.substr(command.find(" ")+1);
    string name = "";
    while(userPass.substr(0,1)!=" "){
        name+=userPass.substr(0,1);
        userPass=userPass.substr(1);
    }
    return name;
}

Frame::~Frame(){
}

