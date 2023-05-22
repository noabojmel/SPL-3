#pragma once
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <unordered_map>
using std::unordered_map;
#include <list>
using std::list;

class Game
{
private:
    unordered_map<string, list<string>> teamA_updates;//maps a user to a list of their updates of team A in the game
    unordered_map<string, list<string>> teamB_updates;//maps a user to a list of their updates of team B in the game
    unordered_map<string, list<string>> generalUpdates;//maps a user to a list of their general updates of the game
    //list<unordered_map<int, string>> timeToDescriptionMap;//for each user we will create this map
    unordered_map<string, unordered_map<int, string>> userToEventNamenMap;//maps the user to his event name by time
    unordered_map<string, unordered_map<int, string>> userToDescriptionMap;//maps the user to his descriptions by time
    string teamA;
    string teamB;
    

public:
    Game(string teamA, string teamB);
    virtual ~Game();
    void summary(string user, string fileName);
    string getTeamA();
    string getTeamB();
    void addTeamAUpdate(string name, string update);
    void addTeamBUpdate(string name, string update);
    void addGeneralUpdate(string name, string update);
    void addDescription(string name, int time, string eventName, string description);
};


