#pragma once
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <unordered_map>
using std::unordered_map;
#include <mutex>

class User
{
private:
    std::string login;
    unordered_map <string, int> topicToSubIdMap;//maps a subscription id to a topic (game)
    unordered_map <int, string> receiptIdToCommandMap;//if the user recieved a receipt from the server 
    int subIdCounter;
    int receiptIdCounter;
    bool loggedIn;
    unordered_map <int, string> waitingForReceipt;//the commnads that are waiting for an ack from the server

public:
    User();
    virtual~User();
    int getSubId();
    int getReceiptId();
    void joinTopic(string &topic, int subId);
    void exitTopic(string &topic, int subId);
    string getCommandByReceipt(int recId);
    void setName(string& name);
    int getSubIdByTopic(string &topic);
    bool isLoggedIn();
    void commandAck(int recId);
    void resetUser();
    void receiptCommand(int id, string cmd);
    string getName();
    bool isSubscribed(string game);
    void setConnect(bool connected);
};


