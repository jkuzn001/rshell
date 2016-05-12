#ifndef __CMD_H__
#define __CMD_H__

#include <vector>
using namespace std;
#include "Base.h"

class Cmd: public Base {
    private:
    string command; //name of the command
    vector<char*> flags; //vector holding the flags provided
    public:
    //constructors
    Cmd(string command, vector<char*> flags);
    Cmd(string command);
    //executes the command using the system calls fork
    //execvp and wait returns true if the command is executed 
    //and false if it fails
    virtual bool execute();
};

#endif
