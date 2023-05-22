#include "Game.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::fstream;
using std::ios;


Game::Game(string teamA, string teamB):teamA_updates(), teamB_updates(), 
generalUpdates(), userToEventNamenMap(), userToDescriptionMap(), teamA(teamA), teamB(teamB){}

string Game::getTeamA(){
    return teamA;
}

string Game::getTeamB(){
    return teamB;
}

Game::~Game() {
}


void Game::addTeamAUpdate(string name, string update){
    teamA_updates[name].push_back(update);
}

void Game::addTeamBUpdate(string name, string update){
    teamB_updates[name].push_back(update);
}

void Game::addGeneralUpdate(string name, string update){
    generalUpdates[name].push_back(update);
}

void Game::addDescription(string name, int time, string eventName, string description){
    std::pair<int, string> newRec1(time, eventName);
    userToEventNamenMap[name].insert(newRec1);
    std::pair<int, string> newRec2(time, description);
    userToDescriptionMap[name].insert(newRec2);
}


void Game::summary(string user, string fileName){
    string op = teamA + " vs " + teamB+'\n';
    op+="Game stats:\n";
    op+="General stats:\n";
    list<string> updates = generalUpdates[user];
    for(const auto& stat:updates)
        op+=stat+'\n';

    op+=teamA +" stats:\n";
    updates = teamA_updates[user];
    for(const auto& stat:updates)
        op+=stat+'\n';

    op+=teamB +" stats:\n";
    updates = teamA_updates[user];
    for(const auto& stat:updates)
        op+=stat+'\n';

    op+="Game event reports:\n";

    unordered_map<int, string> timeAndEvents = userToEventNamenMap[user];
    unordered_map<int, string> timeAndDesc = userToDescriptionMap [user];

    vector<int> keys;//a vector of all the game time updates of 'user'
    for(const auto& time:timeAndEvents)
        keys.push_back(time.first);

    sort(keys.begin(), keys.end());//sorting the time of the events

    for(const auto& time:keys){
        string t = std::to_string(time);
        op+= t+" - "+ timeAndEvents[time]+'\n';
        op+=timeAndDesc[time]+'\n';
    }

    std::fstream file;
    file.open(fileName);
    if(file.is_open()){//given file exists
        file.close();
        file.open(fileName, ios::out | ios::app);
        file<<op<<endl;//write to file
    }
    else{
        file.open(fileName, ios::out);//open file and ready to write
        file<<op<<endl;
    }
    file.close();
}