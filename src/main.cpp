#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

#include "Base.h"
#include "Cmd.cpp"
#include "Connector.cpp"
#include "AND.cpp"
#include "OR.cpp"
#include "Semicolon.cpp"
#include "Exit.cpp"

bool checkCon(char *q) {
    vector<string> s;
    s.push_back("&&");
    s.push_back("||");
    //s.push_back(";");

    for(unsigned i=0; i<s.size(); i++) {
        if(q == s.at(i)) {
            return false;
        }
    }
    return true;
}

int main(int argc, char**argv) {
    /*vector<char*> flags;
    char a[15] = "-R";
    flags.push_back(a);

    Cmd *testing = new Cmd("ls", flags);
    //Cmd *testing("ls");
    testing->execute();
    return 0;*/

    for(; ;) {
        queue<Cmd* > commandList;
        queue<char *> connectorList;
        queue<Connector *> completedListToRun;

        cout << "$ ";

        string userInput;
        getline(cin, userInput);

        if(userInput == "exit") {
            Exit *exit = new Exit();
            exit->execute();
            break;
        }
        char *cstr = new char[userInput.size()+1];
        strcpy(cstr, userInput.c_str());

        char *p = strtok(cstr, " ");


        //Checking first Argument for semi
        bool firstArgSemi = false;
        char *checkingSemi;
        char *checkingComment;

        checkingSemi = (char *) memchr(p, ';', strlen(p));
        checkingComment = (char *) memchr(p, '#', strlen(p));

        if(checkingSemi != NULL) {
            string tempP = string(p);
            tempP = tempP.substr(0, tempP.size()-1);
            strcpy(p, tempP.c_str());

            string semiCol = ";";
            char *pushConnector = new char[2];
            strcpy(pushConnector, semiCol.c_str());
            connectorList.push(pushConnector);

            firstArgSemi = true;
        }

        if(checkingComment != NULL) {

        }
        else {
        //Parsing String
            while(p!=0) {
                char *commentP = (char *) memchr(p, '#', strlen(p));
                if(commentP != NULL) {
                    break;
                }

                Cmd *testingCommand = new Cmd(p);
                char *q = p;

                if(!firstArgSemi) {
                    q = strtok(NULL, " ");  //Checking for flag

                    while(q!=0) {
                        //bool checkCommands = checkCom(q);
                        bool checkConnectors = checkCon(q);

                        char *comment = (char *) memchr(q, '#', strlen(q));
                        if(comment != NULL) {

                        }
                        else {
                            if(checkConnectors) {
                                char *semi = (char *) memchr(q, ';', strlen(q));

                                if(semi != NULL) {
                                    string tempQ = string(q);
                                    tempQ = tempQ.substr(0, tempQ.size()-1);
                                    strcpy(q, tempQ.c_str());

                                    testingCommand->add_flag(q);

                                    string Colon = ";";
                                    char *pushColon = new char[2];
                                    strcpy(pushColon, Colon.c_str());
                                    connectorList.push(pushColon);
                                    break;
                                }

                                else {
                                    testingCommand->add_flag(q);
                                }
                            }
                            else {
                                if(!checkConnectors) {
                                    connectorList.push(q);
                                }
                                break;
                            }
                        }
                        q = strtok(NULL, " ");
                    }

                    p = q;
                    p = strtok(NULL, " ");

                    if(testingCommand->getCommand() == "exit") {
                        Exit *out = new Exit();
                        commandList.push(out);
                    }
                    else {
                        commandList.push(testingCommand);
                    }
                }
                else {
                    commandList.push(testingCommand);
                    p = q;
                    p = strtok(NULL, " ");
                    firstArgSemi = false;
                }
            }


            //Seperating Cmd * respectively
            if(connectorList.size() > 0) {
                string AND_STRING = "&&";
                string OR_STRING = "||";
                string SEMI_STRING = ";";

                Cmd *lhs = commandList.front();
                commandList.pop();
                Cmd *rhs = commandList.front();
                commandList.pop();

                char *temp = connectorList.front();
                connectorList.pop();
                //completedListToRun
                if(temp == AND_STRING) {
                    AND *n = new AND(lhs, rhs);
                    completedListToRun.push(n);
                }
                if(temp == OR_STRING) {
                    OR *n = new OR(lhs, rhs);
                    completedListToRun.push(n);
                }
                if(temp == SEMI_STRING) {
                    Semicolon *n = new Semicolon(lhs, rhs);
                    completedListToRun.push(n);
                }

                while(connectorList.size() != 0) {
                    Connector *tempLHS = completedListToRun.front();
                    completedListToRun.pop();
                    Cmd *rhs = commandList.front();
                    commandList.pop();

                    char *temp2 = connectorList.front();
                    connectorList.pop();
                    if(temp2 == AND_STRING) {
                        AND *n = new AND(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                    else if(temp2 == OR_STRING) {
                        OR *n = new OR(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                    else if(temp2 == SEMI_STRING) {
                        Semicolon *n = new Semicolon(tempLHS, rhs);
                        completedListToRun.push(n);
                    }

                }

                Connector *singleRun = completedListToRun.front();
                completedListToRun.pop();
                singleRun->execute();

            }
            else {
                if(commandList.size() != 1) {
                    cout << "Error commandList has more than 1 Cmd*" << endl;
                    return 1;
                }
                else {
                    Cmd *temporaryCmd = commandList.front();
                    commandList.pop();
                    temporaryCmd->execute();
                }
            }
        }

        delete[] cstr;
    }
    return 0;
}
