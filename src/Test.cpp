#ifndef __TEST_CPP__
#define __TEST_CPP__

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <queue>
#include <sstream>
#include <dirent.h>
#include <iostream> 

using namespace std;

#include "Cmd.h"
#include "Test.h"

Test::Test(queue<char*> flags): Cmd(flags) {}
Test::Test(): Cmd() {}

//executes the test according the flag specified
//if flag == -e it will be tested to see if it is either a directory or a regular file
//if flag == -f it will be tested to see if it is a regular file
//if flag == -d it will be tested to see if it is a directory
//if no flag is provided then -e will be assumed
bool Test::execute() {
    struct stat buf;
    string flag;
    string path;
    bool exists = false;

    if(flags.size() == 1) {
        flag = "-e";
        path = flags.front();
    }
    else {
        flag = flags.front();
        flags.pop();
        path = flags.front();
    }
      
    int statret = stat(path.c_str(),&buf);
    if (statret == 0) {
        exists = true;
    }
    if(statret == -1) { 
        perror("stat");
        return false;
    }

    if(flag == "-e") {
        cout << "true" << endl;
        return exists;
    }
    else if(flag == "-d") {
        if(S_ISDIR(buf.st_mode)) {
            cout << "true" << endl;
            return true;
        }
        else {
            cout << "false" << endl;
            return false;
        }
    }
    else if(flag == "-f") {
         if(S_ISREG(buf.st_mode)) {
             cout << "true" << endl;
             return true;
         }
        else {
            cout << "false" << endl;
            return false;
        }
    }
    else {
        cout << "Error: In test flag " << flag << " is not valid."  << endl;
        return false;
    }
}

#endif
