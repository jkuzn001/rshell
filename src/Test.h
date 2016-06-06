#ifndef __TEST_H__
#define __TEST_H__

using namespace std;
#include "Base.h"

class Test: public Base {
    protected:
    char *command; //name of the command
    queue<char*> flags; //vector holding the flags provided

	public:
    Test(char *command, queue<char*> flags);
    Test(queue<char*> flags);
    Test();

    //Adds a flag based on the container that is used (I.E. queue)
    void add_flag(char *a);
    //executes the command using the system calls fork
    //execvp and wait returns true if the command is executed
    //and false if it fails
    virtual bool execute(int in, int out);

    //For Debugging Purposes
    string get_data();
};
#endif
