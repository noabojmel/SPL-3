#include "StompProtocol.h"
using boost::asio::ip::tcp;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
extern bool isLoggedIn;

StompProtocol::StompProtocol(ConnectionHandler& connectionHandler, User& user):ch(connectionHandler), user(user), connected(true), games(){}

void StompProtocol::proccessKeyBoard(string& input){
    Frame f(input);
	string msgType = f.getFrameType();
	string op="";
	if(msgType=="login"){
		if(user.isLoggedIn()){
			std::cout << "The client is already logged in, log out before trying again.\n" << std::endl;
		}
		else{
			op = f.connectFrame();
			string name = f.findName();
			user.setName(name);
		}
	}

	else if(msgType=="join"){
		int subId = user.getSubId();
		int recId = user.getReceiptId();
		op = f.subscribeFrame(subId, recId);
		user.receiptCommand(recId, "Joined channel "+f.getCommand());
	}

	else if(msgType=="exit"){
		int id = user.getSubIdByTopic(f.getCommand());
		int recId = user.getReceiptId();
		op = f.unsubscribeFrame(id, recId);
		user.receiptCommand(recId, "Exited channel "+f.getCommand());
	}

	else if(msgType=="logout"){
		int recId = user.getReceiptId();
		op = f.disconnectFrame(recId);
		user.receiptCommand(recId, "logout");
	}

	else if(msgType=="report"){
		string fName = f.getCommand();
		names_and_events newEvents = parseEventsFile(fName);
		string game = newEvents.team_a_name+"_"+newEvents.team_b_name;
		vector<string> events;
		if(user.isSubscribed(game)){
			events = f.reportFrame(fName, user.getName());
			for(string event : events)//sending each event seperatly
				ch.sendFrameAscii(event, '\0');
		}
		else
			std::cout<<"you are not subscribed to "+game+", therefore you cannot send updates on this game"<<std::endl;
	}

	else if(msgType=="summary"){
		string cmd = f.getCommand();
		string game = "";
    	while(cmd.substr(0,1)!=" "){
        	game+=cmd.substr(0,1);
        	cmd=cmd.substr(1);
    	}
		cmd=cmd.substr(1);
		string name ="";
		while(cmd.substr(0,1)!=" "){
        	name+=cmd.substr(0,1);
        	cmd=cmd.substr(1);
    	}
		cmd=cmd.substr(1);
		string fileNmae="";
		int i=0;
		int len = cmd.length();
		while(i<len){
        	fileNmae+=cmd.substr(0,1);
        	cmd=cmd.substr(1);
			i++;
    	}
		std::unordered_map<string, Game*>::iterator it = games.find(game);//finding the rigth game for the requsted summery
    	if(it!=games.end()){
			it->second->summary(name, fileNmae);
		}
		else
			std::cout<<"you are not subscribed to "+game<<std::endl;
	}

	if(op!=""){ch.sendFrameAscii(op, '\0');}
}

void StompProtocol::proccessServer(string& input){
    receivedFrames f(input);
	string type = f.getType();
	if(type=="CONNECTED"){
		std::cout << "Login successful\n" << std::endl;
		user.setConnect(true);
	}
	else if(type=="ERROR"){
		std::cout<< f.getErrorMessage() <<std::endl;
		std::cout<<"user is logged out"<<std::endl;
		connected = false;
		isLoggedIn=false;
		//resetting the games map
		for(std::unordered_map<std::string, Game*>::iterator it=games.begin(); it!=games.end(); ++it){
			games.erase(it);
			delete it->second;
		}
		user.resetUser();
		ch.close();
	}

	else if(type=="RECEIPT"){//telling the user that his request was proccessed by the server
		int id = f.receiptId();
		user.commandAck(id);
		string cmd = user.getCommandByReceipt(id);
		if(cmd!="logout"){
			string game = user.getCommandByReceipt(id).substr(15);
			if(cmd.substr(0,1)=="J"){//subscribing to game
				user.joinTopic(game, id);
				int i = game.find("_");
				string teamA = game.substr(0, i);
				string teamB = game.substr(i+1);
				Game* g = new Game(teamA, teamB);
				std::pair<string, Game*> newRec(game, g);
				games.insert(newRec);
			}
			else{//deleteing the subscription from game
				user.exitTopic(game, id);
				std::unordered_map<string, Game*>::iterator it = games.find(game);
    			if(it!=games.end()){
					games.erase(it);
					delete it->second;
				}
			}
			std::cout<<cmd<<std::endl;//printing Joined/Exited
		}
		else{//logging out
			std::cout<<"user is logged out"<<std::endl;
			connected = false;
			isLoggedIn=false;
			//resetting the games map
			int size = games.size();
			for(std::unordered_map<std::string, Game*>::iterator it=games.begin(); it!=games.end(); ++it){//we need to call the game destructor because we deacleard "new" once
				if(size>0){
					games.erase(it);
					delete it->second;
				}
				size--;
			}
			user.resetUser();
			ch.close();
		}
	}

	//adding the report's information to game
	else if(type=="MESSAGE"){
		string frame = f.getFrame();
		string temp=frame;
		string gameName="";
		for(int j=0; j<3; j++){//finding the game by the header destenatin
			int end = temp.find('\n');
			if(temp[0]=='d'){
				gameName = temp.substr(12, end-12);
			}
			temp = temp.substr(end+1);
		}
	
		Game* game;
		std::unordered_map<string, Game*>::iterator it = games.find(gameName);
    	if(it!=games.end()){
			game = it->second;
		}
		else
			std::cout<<"you received an update on a game you are not subscribed to"<<std::endl;

		int endNextLine = temp.find('\n');
		string name=temp.substr(6, endNextLine-6);
		temp = temp.substr(endNextLine+1);//team a line
		endNextLine = temp.find('\n');
		temp = temp.substr(endNextLine+1);//team b line
		endNextLine = temp.find('\n');
		temp = temp.substr(endNextLine+1);//event name line
		endNextLine = temp.find('\n');
		string eventName = temp.substr(12, endNextLine-12);
		temp = temp.substr(endNextLine+1);//time line
		endNextLine = temp.find('\n');
		string sTime = temp.substr(6, endNextLine-6);
		int time = stoi(sTime);
		endNextLine = temp.find('\n');//general updates line
		temp = temp.substr(endNextLine+1);
		endNextLine = temp.find('\n');
		temp = temp.substr(endNextLine+1);
		string generalUpdates="";
		while(temp.substr(0,6)!="team a"){
			endNextLine = temp.find('\n');
			generalUpdates+=temp.substr(0, endNextLine)+'\n';
			temp = temp.substr(endNextLine+1);
		}

		endNextLine = temp.find('\n');
		temp = temp.substr(endNextLine+1);
		string teamAUpdates="";
		while(temp.substr(0,6)!="team b"){
			endNextLine = temp.find('\n');
			teamAUpdates+=temp.substr(0, endNextLine)+'\n';
			temp = temp.substr(endNextLine+1);
		}

		endNextLine = temp.find('\n');
		temp = temp.substr(endNextLine+1);
		string teamBUpdates="";
		while(temp.substr(0,11)!="description"){
			endNextLine = temp.find('\n');
			teamBUpdates+=temp.substr(0, endNextLine)+'\n';
			temp = temp.substr(endNextLine+1);
		}

		endNextLine = temp.find('\n');
		temp = temp.substr(endNextLine+1);
		string description = temp.substr(0, temp.length()-1)+'\n';

		//adding the updates of the user who sent them to game
		if(generalUpdates!="")
			game->addGeneralUpdate(name, generalUpdates);
		if(teamAUpdates!="")
			game->addTeamAUpdate(name, teamAUpdates);
		if(teamBUpdates!="")
			game->addTeamBUpdate(name, teamBUpdates);
		if(description!="")
			game->addDescription(name, time, eventName, description);
		
	}
}

bool StompProtocol::isConnected(){
    return connected;
}