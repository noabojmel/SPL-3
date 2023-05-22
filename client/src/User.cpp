#include "User.h"
#include <mutex>

User::User():login(), topicToSubIdMap(),
 receiptIdToCommandMap(), subIdCounter(0), receiptIdCounter(0), loggedIn(false), waitingForReceipt(){}


void User::setName(string& name){login=name;}

User::~User() {
}

int User::getReceiptId(){
    receiptIdCounter++;
    return receiptIdCounter-1;
}

void User::receiptCommand(int id,string cmd){
    std::pair<int, string> newRec(id, cmd);
    waitingForReceipt.insert(newRec);
}

void User::setConnect(bool connected){
    loggedIn = connected;
}

//generate a new subscription id
int User::getSubId(){
    subIdCounter++;
    return subIdCounter-1;
}

//this user has joined topic with subscription id of subId
void User::joinTopic(string &topic, int subId){
    std::pair<string,int> newSub (topic,subId);
    topicToSubIdMap.insert(newSub);
}

void User::exitTopic(string &topic, int subId){
    topicToSubIdMap.erase(topic);
}


//the client received a repeipt from the server with repeict-id of recId
string User::getCommandByReceipt(int recId){
    string op;
    std::unordered_map<int, string>::iterator it = receiptIdToCommandMap.find(recId);
    if(it!=receiptIdToCommandMap.end())
        op = it->second;
    return op;
}

bool User::isSubscribed(string game){
    std::unordered_map<string, int>::iterator it = topicToSubIdMap.find(game);
    if(it!=topicToSubIdMap.end())
        return true;
    return false;
}

//returns the subId of this user to topic, if the user is not subscribed to topic it will return -1
int User::getSubIdByTopic(string &topic){
    int id = -1;
    std::unordered_map<string, int>::iterator it = topicToSubIdMap.find(topic);
    if(it!=topicToSubIdMap.end())
        id = it->second;
    return id;
}

bool User::isLoggedIn(){
    return loggedIn;
}

//moving the command that the server acknowledged from the waiting list to the commands list
void User::commandAck(int recId){
    string cmd;
    std::unordered_map<int, string>::iterator it = waitingForReceipt.find(recId);
    if(it!=waitingForReceipt.end()){
        cmd = it->second;
        std::pair<int, string> newRec(recId, cmd);
        receiptIdToCommandMap.insert(newRec);
        waitingForReceipt.erase(recId);
    }
}

void User::resetUser(){//called when the user logged out
    receiptIdToCommandMap.clear();
    topicToSubIdMap.clear();
    waitingForReceipt.clear();
    subIdCounter=0;
    receiptIdCounter=0;
    loggedIn = false;
}

string User::getName(){
    return login;
}