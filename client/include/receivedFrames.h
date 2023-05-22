#pragma once
#include <string>
using std::string;

class receivedFrames
{
private:
    string input;
    string type;
    string frame;
public:
    receivedFrames(string& input);
    virtual ~receivedFrames();
    string getType();
    int receiptId();
    string getErrorMessage();
    string getFrame();
};
