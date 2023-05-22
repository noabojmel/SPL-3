CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

#all: StompWCIClient
  #  g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/User.o bin/Frame.o bin/ReceivedFrames.o bin/Game.o bin/keyBoardThread.o bin/readFromServer.o bin/readFromServer.o $(LDFLAGS)

StompWCIClient: bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/User.o bin/Frame.o bin/receivedFrames.o bin/Game.o bin/keyBoardThread.o bin/readFromServer.o bin/readFromServer.o bin/StompProtocol.o
	g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/User.o bin/Frame.o bin/receivedFrames.o bin/Game.o bin/keyBoardThread.o bin/readFromServer.o bin/StompProtocol.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/User.o: src/User.cpp
	g++ $(CFLAGS) -o bin/User.o src/User.cpp

bin/Frame.o: src/Frame.cpp
	g++ $(CFLAGS) -o bin/Frame.o src/Frame.cpp

bin/receivedFrames.o: src/receivedFrames.cpp
	g++ $(CFLAGS) -o bin/receivedFrames.o src/receivedFrames.cpp

bin/Game.o: src/Game.cpp
	g++ $(CFLAGS) -o bin/Game.o src/Game.cpp

bin/keyBoardThread.o: src/keyBoardThread.cpp
	g++ $(CFLAGS) -o bin/keyBoardThread.o src/keyBoardThread.cpp

bin/readFromServer.o: src/readFromServer.cpp
	g++ $(CFLAGS) -o bin/readFromServer.o src/readFromServer.cpp

bin/StompProtocol.o: src/StompProtocol.cpp
	g++ $(CFLAGS) -o bin/StompProtocol.o src/StompProtocol.cpp

.PHONY: clean
clean:
	rm -f bin/*
	