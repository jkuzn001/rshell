#ifndef __CMD_CPP__
#define __CMD_CPP__

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

#include "Base.h"
#include "Cmd.h"

//constructors
Cmd::Cmd(string command, vector<char*> flags)
: command(command), flags(flags) {}
Cmd::Cmd(string command): command(command){}
//executes the command using the system calls fork
//execvp and wait returns true if the command is executed 
//and false if it fails
bool Cmd::execute() {
    
    //puts null character at the end of the flags array
    flags.push_back(NULL);

    
    //c-string array to pass to execvp
    char* args[flags.size() + 1];
    for(int i = 1; i < flags.size() + 1; ++i) {
        args[i] = flags.at(i - 1);
    }
      
    //return value of the function
    //true if command executes
    bool ret = true;
 
    pid_t pid = fork();
    if(pid == -1) {
        perror("fork");
    }
    else if (pid == 0) {
        if(execvp(command.c_str(),args) == -1) {
            ret = false;
            perror("execvp");
            exit(1);
        }
    }
    else if(pid > 0) {
       int status;
       if(waitpid(pid,&status,0) == -1) {
           perror("wait");
       }
       if(WEXITSTATUS(status) == 1) {
           ret = false;
       }
        return ret;
    }
}

#endif
