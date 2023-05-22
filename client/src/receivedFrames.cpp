#include "receivedFrames.h"

receivedFrames::receivedFrames(string& input):input(input), type(""),frame(""){
    string start = "";
    int counter = 0;
    bool stop = false;
    for(int i = 0; i<(int)input.length()&&!stop; i++){

        if(input[i]=='\n')//found the first word of the frame
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
    frame = input.substr(input.find('\n')+1);
}

receivedFrames::~receivedFrames() {
}

string receivedFrames::getType(){
    return type;
}

int receivedFrames::receiptId(){//assuming this is a receipt frame the function will return the receipt id
    int size = frame.find('\n')-frame.find(':')-1;//the numbers of characters that represents the rec id in the frame
    int id = stoi(frame.substr(frame.find(':')+1, size));
    return id;
}

string receivedFrames::getErrorMessage(){//assuming this is an error frame the function will return the error massge from the server

    return frame;
}

string receivedFrames::getFrame(){
    return frame;
}