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
#include <stack>
#include <iterator>

using namespace std;

#include "Base.h"
#include "Cmd.cpp"
#include "Connector.cpp"
#include "AND.cpp"
#include "OR.cpp"
#include "Semicolon.cpp"
#include "Exit.cpp"
#include "Test.cpp"

#include "SingleOutRed.cpp"
#include "DoubleOutRed.cpp"
#include "InputRed.cpp"
#include "Pipe.cpp"

//Global Variables used to check connectors
const string AND_STRING = "&&";
const string OR_STRING = "||";
const string SEMI_STRING = ";";

const string INPUT_RED = "<";
const string SINGLE_OUTPUT_RED = ">";
const string DOUBLE_OUTPUT_RED = ">>";
const string PIPE = "|";

//Function used to delete the first character of a cString
void splitUpFirstCharacter(char* p) {

    string tempP = string(p);
    tempP = tempP.substr(1, tempP.size()-1);
    strcpy(p, tempP.c_str());
}

//Function used to delete the last character of a cString but does not delete '#' and ';'
void splitUpLastCharacter(char* p) {
    string tempP = string(p);
    size_t comments = tempP.find('#');
    size_t comma = tempP.find(';');
    if(comma!=std::string::npos || comments!=std::string::npos) {
        tempP.erase(tempP.end()-2);
    }
    else {
        tempP.erase(tempP.end()-1);
    }
    strcpy(p, tempP.c_str());
}

//Function used to delete the last character no matter what
void splitUpLastCharacterAlways(char *p) {
    string tempP = string(p);
    tempP.erase(tempP.end()-1);
    strcpy(p, tempP.c_str());
}

/*checkCon() passes in a char * and returns a boolean value on whether or not q is a connector*/
bool checkCon(char *q) {
    vector<string> s;
    s.push_back("&&");
    s.push_back("||");
    //Note that ; and # are not included because they have special parsing properties

    s.push_back("|"); //Pipe
    s.push_back(">");
    s.push_back(">>");
    s.push_back("<");
    //Go through the vector of connectors and check if char *q is equal to any of the connectors
    for(unsigned i=0; i<s.size(); i++)
        if(q == s.at(i))
            return false;
    return true;
}

/*Function used to delete the connectors in between and return the newly created string*/
string SepCommands(char *q) {
    string Q = string(q);
    string results = "";

    for(string::iterator it = Q.begin(); it < Q.end(); it++) {
        if(*it == '<') {
            results += " < ";
        }
        else if(*it == '>') {
            string::iterator temp = it;
            temp++;
            if(*temp == '>') {
                results += " >> ";
                it = temp;
            }
            else {
                results += " > ";
            }
        }
        else if(*it == '|') {
            string::iterator temp = it;
            temp++;
            if(*temp == '|') {
                results += " || ";
                it = temp;
            }
            else {
                results += " | ";
            }
        }
        else if(*it == ')') {

        }
        else {
            results += *it;
        }
    }
    return results;
}

//Checks all the connectors
bool checkAllCon(char *q) {
    vector<string> s;
    s.push_back("&&");
    s.push_back("||");
    s.push_back("#");
    s.push_back(";");

    s.push_back("|"); //Pipe
    s.push_back(">");
    s.push_back(">>");
    s.push_back("<");

    for(unsigned i=0; i<s.size(); i++)
        if(q == s.at(i))
            return false;
    return true;
}

//Function will take in a cstr and return a tree representation of said cstr.
Base* grabTree(char *cstr) {
    //Checking for input/output redirection and piping
    string StringCstr = string(cstr);

    size_t findInput = StringCstr.find('<');
    size_t findOutput = StringCstr.find('>');
    size_t findPipe = StringCstr.find('|');
    if(findInput!=std::string::npos || findOutput!=std::string::npos || findPipe!=std::string::npos) {
        StringCstr = SepCommands(cstr);
        strcpy(cstr, StringCstr.c_str());
    }

    queue<Base *> commandList;               //Separates the Commands to create Cmd's objects respectively
    queue<char *> connectorList;            //Same here, but for Connectors ofc ^
    queue<Connector *> completedListToRun;  //This queue should only contain one tree of all the commmands
                                            //and connectors
	char *p = strtok(cstr, " ");                                //Initialize a array of Tokens

    //Checking first Argument for semi and comment
    bool firstArgSemi = false; //used within the while loop
    char *checkingSemi = (char *) memchr(p, ';', strlen(p));          //check first Token for Semi
    char *checkingComment = (char *) memchr(p, '#', strlen(p));       //check first Token for Comment
    char *checkingTest = (char *) memchr(p, '[', strlen(p));          //check first Token for Test
    string checkingStringTest = string(p);                            //checks for 'test'

    if(checkingSemi != NULL) {                                        //if 1st Token == ';'
        //parse the Token to not include the ';'
        string tempP = string(p);
        tempP = tempP.substr(0, tempP.size()-1);
        strcpy(p, tempP.c_str());

        //pushes the ';' into the ConnectorList
        string semiCol = ";";
        char *pushConnector = new char[2];
        strcpy(pushConnector, semiCol.c_str());
        connectorList.push(pushConnector);

        //Set the boolean to true
        firstArgSemi = true;
    }

    //checks if first string is 'test'
    if(checkingStringTest.compare("test") == 0) {
        p = strtok(NULL, " "); //skip the actual string 'test'
        Test *c = new Test();  //Creates a Test instance

        char *checkFlag = (char *) memchr(p, '-', strlen(p));   //Checks if the next element is a flag
        if(checkFlag != NULL) { //if so take in the flag into account by adding two elements to the flag.
            //cout << p << endl;
            c->add_flag(p);
            p = strtok(NULL, " ");
            c->add_flag(p);
            p = strtok(NULL, " ");
        }
        else {                  //if not only add one element to the flag
            c->add_flag(p);
            p = strtok(NULL, " ");
        }

        commandList.push(c);   //Push the finish Test into the queue to be turned to a tree later

        if(p != 0) {           //Check before going into the loop
            bool checkingConnectors = checkAllCon(p);
            char *commentP = (char *) memchr(p, '#', strlen(p));

            if(commentP != NULL) {
                checkingComment = commentP;
            }
            else if(checkingConnectors) {
                cout << "Error: Expected a connector, Received: " << p << endl;
                exit(1);
            }
            else {
                connectorList.push(p);
                p = strtok(NULL, " ");
            }
        }
    }

    //Used to check '[' same algorithm as the 'test'
    if(checkingTest != NULL) {
        Test *c = new Test();

        p = strtok(NULL, " ");

        char *checkFlag = (char *) memchr(p, '-', strlen(p));
        if(checkFlag != NULL) {
            c->add_flag(p);
            p = strtok(NULL, " ");
            c->add_flag(p);
            p = strtok(NULL, " ");
        }
        else {
            c->add_flag(p);
            p = strtok(NULL, " ");
        }
        //cout << p << endl;
        if(p != 0) {    //A throw statement for error user checking
            char *checkingEndTest = (char *) memchr(p, ']', strlen(p));
            if(checkingEndTest == NULL) {
                cout << "Error: Expected: ']', Received: " << p << endl;
                exit(1);
            }
        }
        p = strtok(NULL, " ");
        //cout << p << endl;
        commandList.push(c);

        if(p!=0) {
            bool checkingConnectors = checkAllCon(p);
            if(checkingConnectors) {
                cout << "Error: Expected a connector, Received: " << p << endl;
                exit(1);
            }
            char *commentP = (char *) memchr(p, '#', strlen(p));
            if(commentP != NULL) {
                checkingComment = commentP;
            }
            else {
                connectorList.push(p);
                p = strtok(NULL, " ");
            }
        }
    }


    if(checkingComment == NULL) {    //if there is a '#' in the first Token, disregard everything
        bool commentDetected = false;
        while(p!=0 && !commentDetected) {
            //Check if the next token contains a #
            char *commentP = (char *) memchr(p, '#', strlen(p));
            if(commentP != NULL) {
                commentDetected = true;
                break;
            }

            string checkingStringTest = string(p);  //Same algorithm as commented on top

            if(checkingStringTest.compare("test") == 0) {
                p = strtok(NULL, " ");
                Test *c = new Test();


                char *checkFlag = (char *) memchr(p, '-', strlen(p));
                if(checkFlag != NULL) {
                    c->add_flag(p);
                    p = strtok(NULL, " ");
                    c->add_flag(p);
                    p = strtok(NULL, " ");
                }
                else {
                    c->add_flag(p);
                    p = strtok(NULL, " ");
                }
                commandList.push(c);

                if(p != 0) {
                    bool checkingConnectors = checkAllCon(p);
                    char *commentP = (char *) memchr(p, '#', strlen(p));

                    if(commentP != NULL) {
                        checkingComment = commentP;
                    }
                    else if(checkingConnectors) {
                        cout << "Error: Expected a connector, Received: " << p << endl;
                        exit(1);
                    }
                    else {
                        connectorList.push(p);
                        p = strtok(NULL, " ");
                    }
                }
                else {
                    break;
                }
            }

            char *testingTest = (char *) memchr(p, '[', strlen(p)); //Same algorithm as top
            if(testingTest != NULL) {
                Test *c = new Test();

				p = strtok(NULL, " ");

				for(unsigned i=0; i<2; i++) {
				    //cout << p << endl;
				    c->add_flag(p);
				    p = strtok(NULL, " ");
				}

                if(p != 0) {
                    char *checkingEndTest = (char *) memchr(p, ']', strlen(p));
				    if(checkingEndTest == NULL) {
				        cout << "Error: Expected: ']', Received: " << p << endl;
				        exit(1);
				    }
				}
				p = strtok(NULL, " ");
				//cout << p << endl;
				commandList.push(c);

				if(p!=0) {
				    bool checkingConnectors = checkAllCon(p);
				    if(checkingConnectors) {
				        cout << "Error: Expected a connector, Received: " << p << endl;
				    }
				    char *commentP = (char *) memchr(p, '#', strlen(p));
				    if(commentP != NULL) {
				        checkingComment = commentP;
				    }
				    else {
				        connectorList.push(p);
				        p = strtok(NULL, " ");
				    }
				}
                else {
                    break;
                }
            }

            //Assume that the first Token is always a command thus make a Cmd Object
            Cmd *testingCommand = new Cmd(p);
            char *q = p;

            if(!firstArgSemi && !commentDetected) {
                q = strtok(NULL, " ");  //Move on to check for the flags

                while(q!=0 && !commentDetected) {   //Keep checking for connectors while add flags.
                    bool checkConnectors = checkCon(q);     //check if Token is a connector

                    char *comment = (char *) memchr(q, '#', strlen(q)); //Special check for comments
                    if(comment != NULL) {           //Disregard everything if comment is found
                        commentDetected = true;
                    }
                    else {
                        if(checkConnectors) {       //If token is not a Connector
                            char *semi = (char *) memchr(q, ';', strlen(q));    //Check if ';' is in Token
                            char *com = (char *) memchr(q, '#', strlen(q));     //Check if '#' is in Token

                            //If so do the same algorithm except add the flag ';' to Cmd testingCommand
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
                            if(com != NULL) {
                                cout << "# found!" << endl;
                                commentDetected = true;
                                break;
                            }
                            //other wise just add the whole token as a flag
                            else {
                                testingCommand->add_flag(q);
                            }
                        }
                        else {
                            //If token is a Connector
                            char *com1 = (char *) memchr(q, '#', strlen(q));     //Check if '#' is in Token
                            if(com1 != NULL) {
                                cout << "# found!" << endl;
                                commentDetected = true;
                                break;
                            }
                            if(!checkConnectors) {
                                connectorList.push(q);  //push the token into the connectorList
                            }
                            break;
                        }
                    }
                    q = strtok(NULL, " ");  //Advance through until you find a connector or reach the end
                }//End of while
                p = q;                  //q is already checked, set p to q
                p = strtok(NULL, " ");  //Then advance p
                if(testingCommand->get_data() == "exit") {    //If the current token is an exit
                    Exit *out = new Exit();                     //Create a Exit Object
                    commandList.push(out);                      //push it to the commandList
                }                                               //Instead of pushing the testingCommand
                else {
                    commandList.push(testingCommand);           //Otherwise just push the testingCommand
                }
            }
            else {  //if the first token had a ';'
                commandList.push(testingCommand);               //Then push the new command w/o ';'
                p = q;                                          //Advance p
                p = strtok(NULL, " ");
                firstArgSemi = false;                           //reset the firstArgSemi condiction
            }
        }
        //Construction of tree execution
        if(connectorList.size() > 0) { //Only runs when there are 2 or more commands
            //Pop the first two commands out of the commandList
            Base *lhs = commandList.front();
            commandList.pop();
            Base *rhs = commandList.front();
            commandList.pop();

            //Pop a connector of the connectorList
            char *temp = connectorList.front();
            connectorList.pop();

            //Check what the connector is and create a object respectively
            //and push it to the completedListToRun
            if(temp == AND_STRING) {
                AND *n = new AND(lhs, rhs);
                completedListToRun.push(n);
            }
            else if(temp == OR_STRING) {
                OR *n = new OR(lhs, rhs);
                completedListToRun.push(n);
            }
            else if(temp == SEMI_STRING) {
                Semicolon *n = new Semicolon(lhs, rhs);
                completedListToRun.push(n);
            }
            else if(temp == INPUT_RED) {
                InputRed *n = new InputRed(lhs, rhs);
                completedListToRun.push(n);
            }
            else if(temp == SINGLE_OUTPUT_RED) {
                SingleOutRed *n = new SingleOutRed(lhs, rhs);
                completedListToRun.push(n);
            }
            else if(temp == DOUBLE_OUTPUT_RED) {
                DoubleOutRed *n = new DoubleOutRed(lhs, rhs);
                completedListToRun.push(n);
            }
            else if(temp == PIPE) {
                Pipe *n = new Pipe(lhs, rhs);
                completedListToRun.push(n);
            }

            //If there are more connectors, pop out the one command from the completedListToRun
            //and set that as the left leaf and pop out another command from the commandList
            //and another connector from the connectorList.
            //
            //create a new Connector object with the completedListToRun object as the left leaf,
            //the commandList object as the right leaf and the connectorList object as the parent
            while(connectorList.size() != 0) {
                Connector *tempLHS = completedListToRun.front();
                completedListToRun.pop();
                Base *rhs = commandList.front();
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
                else if(temp2 == INPUT_RED) {
                    InputRed *n = new InputRed(tempLHS, rhs);
                    completedListToRun.push(n);
                }
                else if(temp2 == SINGLE_OUTPUT_RED) {
                    SingleOutRed *n = new SingleOutRed(tempLHS, rhs);
                    completedListToRun.push(n);
                }
                else if(temp2 == DOUBLE_OUTPUT_RED) {
                    DoubleOutRed *n = new DoubleOutRed(tempLHS, rhs);
                    completedListToRun.push(n);
                }
                else if(temp2 == PIPE) {
                    Pipe *n = new Pipe(tempLHS, rhs);
                    completedListToRun.push(n);
                }
            }
            //After building the tree, pop the single command from completedListToRun
            //and run execute on the command. The execute function will travel through the completed tree
            //and execute the individual Cmd objects respectively and return a boolean value based on
            //whether it passes or fails.
            Connector *singleRun = completedListToRun.front();
            completedListToRun.pop();
            return singleRun;
        }
        else {  //If there are no connectors then there must only be one cmd and/or a bunch of flags
            if(commandList.size() != 1) {   //MAKE SURE THAT THERE IS ONLY ONE CMD
                //Throw and error if so
                cout << "Error commandList has more than 1 Cmd*" << endl;
                exit(1);
            }
            else {
                //Simply pop the one command from the CommandList and execute it
                Base *temporaryCmd = commandList.front();
                commandList.pop();
                return temporaryCmd;
            }
        }
    }//End of infinite for loop
    if(commandList.size() > 1) {
        cout << "Error commandList has more than 1 Cmd*" << endl;
        exit(1);
    }
    else {
        Base* resultCmd = commandList.front();
        commandList.pop();
        return resultCmd;
    }
    return NULL;
}

int main(int argc, char**argv) {

    //This is an infinite for loop for the shell. We break out of the loop only when the user enters an
    //exit command or when the user inputs an exit command that was executed based on the user's input
    for(; ;) {
        //Working on login info
        char *userName = getlogin();            //Grab the user's name
        if(!userName) {
            perror("getlogin() error");         //Just in case you can't find it
        }

        char hostName[1000];
        gethostname(hostName, sizeof hostName); //Grab the machine name

        cout << userName << "@" <<  hostName << "$ ";                           //Prints the bash $
        //End of login info

        string userInput;                       //holds the user's input
        getline(cin, userInput);                //prompts the user to input a command and populates the string ^

        if(userInput == "exit") {               //Check if the user just inputed 1 exit command
            Exit *exit = new Exit();
            exit->execute(0,1);
            break;
        }

        char *cstr = new char[userInput.size()+1];                  //Initialize a C string array
        strcpy(cstr, userInput.c_str());                            //Parse the string into a *cstr

        //Doing the Precedence Algorithm
        queue<Base *> precedenceTrees;
        queue<Connector *> outsideConnectors;

        size_t foundPrecedence = userInput.find('(');               //Check if '(' is in the userString
        size_t foundTest = userInput.find('[');                     //Same for '['
        size_t foundOutput = userInput.find('>');
        size_t foundInput = userInput.find('<');
        size_t foundPipe = userInput.find('|');

        if(foundPrecedence!=std::string::npos || (foundPrecedence!=std::string::npos && foundTest!=std::string::npos) || (foundPrecedence!=std::string::npos && (foundOutput!=std::string::npos && foundInput!=std::string::npos && foundPipe!=std::string::npos))) {    //If we find the '(' or if we find both '(' and '[' operators
            string totalString = "";

            char *p = strtok(cstr, " ");

            //Go through the entire string while checking if any element contains the following listed below.
            //If so then separate the operators from the "command/flags/paths" parts
            while(p!=0) {
                char *checkingPrecedenceF = (char *) memchr(p, '(', strlen(p));
                char *checkingPrecedenceE = (char *) memchr(p, ')', strlen(p));
                char *checkingTestB = (char *) memchr(p, '[', strlen(p));
                char *checkingTestE = (char *) memchr(p, ']', strlen(p));
                char *checkingInput = (char *) memchr(p, '<', strlen(p));
                char *checkingPipe = (char *) memchr(p, '|', strlen(p));
                char *checkingOutput = (char *) memchr(p, '>', strlen(p));

                int totalEndingPrecedence = 0;

                if(checkingPrecedenceF != NULL && (checkingInput != NULL || checkingPipe != NULL || checkingOutput!= NULL)) {
                    while(checkingPrecedenceF != NULL) {
                        totalString += "( ";
                        splitUpFirstCharacter(p);
                        checkingPrecedenceF = (char *) memchr(p, '(', strlen(p));
                    }
                    string pString = SepCommands(p);

                    //cout << pString << endl;

                    totalString += pString;
                    while(checkingPrecedenceE != NULL) {
                        totalEndingPrecedence++;
                        splitUpLastCharacter(p);
                        checkingPrecedenceE = (char *) memchr(p, ')', strlen(p));
                    }

                    char *checkingComma = (char *) memchr(p, ';', strlen(p));
                    char *checkingComment = (char *) memchr(p, '#', strlen(p));

                    if(checkingComma != NULL || checkingComment != NULL) {
                        splitUpLastCharacterAlways(p);
                        totalString += string(p);
                        totalString += " ";
                        for(int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                        if(checkingComma != NULL) {
                            totalString += "; ";
                        }
                    }
                    else {
                        for(int i=0; i<totalEndingPrecedence; i++) {
                            totalString += " ) ";
                        }
                    }
                }
                else if(checkingPrecedenceF != NULL && checkingTestB != NULL && checkingTestE != NULL) {
                    while(checkingPrecedenceF != NULL) {
                        totalString += "( ";
                        splitUpFirstCharacter(p);
                        checkingPrecedenceF = (char *) memchr(p, '(', strlen(p));
                    }

                    totalString += "[ ";
                    splitUpFirstCharacter(p);

                    while(checkingPrecedenceE != NULL) {
                        totalEndingPrecedence++;
                        splitUpLastCharacter(p);
                        checkingPrecedenceE = (char *) memchr(p, ')', strlen(p));
                    }

                    char *checkingComma = (char *) memchr(p, ';', strlen(p));
                    char *checkingComment = (char *) memchr(p, '#', strlen(p));

                    if(checkingComma != NULL || checkingComment != NULL) {
                        splitUpLastCharacterAlways(p);
                        totalString += string(p);
                        totalString += " ";
                        for(int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                        if(checkingComma != NULL) {
                            totalString += "; ";
                        }
                        else {
                            totalString += "# ";
                        }
                    }
                    else {
                        splitUpLastCharacterAlways(p);
                        totalString += string(p);
                        totalString += " ";
                        totalString += "] ";
                        for(int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                    }

                }
                else if(checkingInput != NULL || checkingOutput != NULL || checkingPipe != NULL) {
                    string pString = SepCommands(p);

                    //cout << pString << endl;

                    totalString += pString;

                    while(checkingPrecedenceE != NULL) {
                        totalEndingPrecedence++;
                        splitUpLastCharacter(p);
                        checkingPrecedenceE = (char *) memchr(p, ')', strlen(p));
                    }

                    char *checkingComma = (char *) memchr(p, ';', strlen(p));
                    char *checkingComment = (char *) memchr(p, '#', strlen(p));

                    if(checkingComma != NULL || checkingComment != NULL) {
                        splitUpLastCharacterAlways(p);
                        totalString += string(p);
                        totalString += " ";
                        for(int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                        if(checkingComma != NULL) {
                            totalString += "; ";
                        }
                    }
                    else {
                        for(int i=0; i<totalEndingPrecedence; i++) {
                            totalString += " ) ";
                        }
                    }
                }
                else if(checkingPrecedenceF != NULL && checkingTestB != NULL) {
                    while(checkingPrecedenceF != NULL) {
                        totalString += "( ";
                        checkingPrecedenceF = (char *) memchr(p, '(', strlen(p));
                        splitUpFirstCharacter(p);
                    }
                    totalString += "[ ";
                    totalString += string(p);
                    totalString += " ";
                }
                else if(checkingTestB != NULL && checkingTestE != NULL) {
                    totalString += "[ ";
                    splitUpFirstCharacter(p);
                    splitUpLastCharacter(p);
                    totalString += string(p);
                    totalString += " ";
                    totalString += "] ";
                }
                else if(checkingTestB != NULL) {
                    totalString += "[ ";
                    splitUpFirstCharacter(p);
                    totalString += string(p);
                    totalString += " ";
                }
                else if(checkingTestE != NULL) {
                    splitUpLastCharacter(p);

                    totalString += string(p);
                    totalString += " ";
                    totalString += "] ";
                }
                else if(checkingPrecedenceF != NULL) {
                    while(checkingPrecedenceF != NULL) {
                        totalString += "( ";
                        splitUpFirstCharacter(p);
                        checkingPrecedenceF = (char *) memchr(p, '(', strlen(p));
                    }
                    totalString += string(p);
                    totalString += " ";
                }
                else if(checkingPrecedenceE != NULL) {
                    while(checkingPrecedenceE != NULL) {
                        totalEndingPrecedence++;
                        splitUpLastCharacter(p);
                        checkingPrecedenceE = (char *) memchr(p, ')', strlen(p));
                    }

                    char *checkingComma = (char *) memchr(p, ';', strlen(p));
                    char *checkingComment = (char *) memchr(p, '#', strlen(p));

                    if(checkingComma != NULL || checkingComment != NULL) {
                        splitUpLastCharacterAlways(p);
                        totalString += string(p);
                        totalString += " ";
                        for(int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                        if(checkingComma != NULL) {
                            totalString += "; ";
                        }
                        else {
                            totalString += "# ";
                        }
                    }
                    else {
                        totalString += string(p);
                        totalString += " ";
                        for(int i=0; i<totalEndingPrecedence; i++) {
                            totalString += ") ";
                        }
                    }
                }
                else {
                    totalString += string(p);
                    totalString += " ";
                }

                p = strtok(NULL, " ");
            }
            //The returned String will contain everything with the operators being separate from the real
            //commands/flags/paths
            //cout << totalString << endl;

            //size_t CheckForPrecdence = userInput.find('(');
            //foundPrecedence!=std::string::npos
            //Separating stuff
            char *totalChar = new char[totalString.size()+1];                  //Initialize a C string array
            strcpy(totalChar, totalString.c_str());                            //Parse the string into a *cstr

            char *c = strtok(totalChar, " ");

	        stack<char *> stringStack;                              //Used to separate to different strings
	        queue<string> branches;
	        queue<char *> connectors;

	        bool withinPrecedence = false;                          //Used for connectors.
	        bool newPrecedence = false;                             //Whether or not in a closed area.
	        //newPrecedence true = within a closed area. false != within a closed area

            //The second pass will allow the program to use a stack pos-fix method to determine what is
            //within two Precedence Operators.
            //Basically we shall continue to push cstring elements into a stack and setting the
            //withinPrecedence and newPrecedence respectively. When we find the endPrecedence, we shall
            //continue to pop out cstring elements from the stack until we reach the respective beginPrecedence
	        while(c!=0) {
	            char *beginPrecedence = (char *) memchr(c, '(', strlen(c));
	            char *endPrecedence = (char *) memchr(c, ')', strlen(c));
	            bool checkConnectors = checkAllCon(c);     //check if Token is a connector

	            if(beginPrecedence != NULL) {
	                withinPrecedence = true;

	                if(!newPrecedence) {
	                    stack<char *> currentString;

	                    while(!stringStack.empty()) {
	                        char *a = stringStack.top();
	                        char *checkingStartPrecedence = (char *) memchr(a, '(', strlen(a));

	                        if(checkingStartPrecedence != NULL) {
	                            stringStack.pop();
	                            break;
	                        }

	                        currentString.push(a);
	                        stringStack.pop();
	                    }
	                    string finalString;

	                    while(!currentString.empty()) {
	                        finalString += currentString.top();
	                        finalString += " ";
	                        currentString.pop();
	                    }
	                    //cout << finalString << endl;
	                    if(finalString.size() > 0) {
	                        branches.push(finalString);
	                    }
	                }

	                newPrecedence = true;
	            }
	            else if(endPrecedence != NULL) {
	                withinPrecedence = false;
	                newPrecedence = false;
	                stack<char *> currentString;

	                while(!stringStack.empty()) {
	                    char *a = stringStack.top();
	                    char *checkingStartPrecedence = (char *) memchr(a, '(', strlen(a));

	                    if(checkingStartPrecedence != NULL) {
	                        stringStack.pop();
	                        break;
	                    }

	                    currentString.push(a);
	                    stringStack.pop();
	                }
	                string finalString;

	                while(!currentString.empty()) {
	                    finalString += currentString.top();
	                    finalString += " ";
	                    currentString.pop();
	                }
	                //cout << finalString << endl;
	                if(finalString.size() > 0) {
	                    branches.push(finalString);
	                }
	            }
	            else if(!checkConnectors && !withinPrecedence) {
	                //cout << "Outside Connectors: " << c << endl;
	                connectors.push(c);
	            }
	            else {
	                //cout << "Pushing into stringStack: " << c << endl;
	                stringStack.push(c);
	            }
	            c = strtok(NULL, " ");
	        }

            //Used to reverse the order
	        stack<char *> currentOvers;
	        while(!stringStack.empty()) {
	            currentOvers.push(stringStack.top());
	            stringStack.pop();
	        }

            //Used to populate a new string so we can push the leftOver stuff in the branch Queue.
	        string leftOvers;
	        while(!currentOvers.empty()) {
	            leftOvers += currentOvers.top();
	            leftOvers += " ";
	            currentOvers.pop();
	        }

	        //cout << leftOvers << endl;
	        if(leftOvers.size() != 0) {
	            branches.push(leftOvers);
	        }

            queue<Base *> commandTreeList;

            //This will go through the entire branches queue and call the grabTree() function to return
            //a tree representation so that we may push it into our queue<Base*> commandTreeList
	        while(!branches.empty()) {
	            //cout << branches.front() << endl;
	            char *r = new char[branches.front().size()+1];
	            strcpy(r, branches.front().c_str());

	            branches.pop();
	            Base* tree = grabTree(r);
                if(tree != NULL) {
                    commandTreeList.push(tree);
	            }
                //tree->execute();
	        }

            //Same algorithm described in the grabTree() Function
            if(connectors.size() > 0) {
                queue<Connector *>completedListToRun;

                Base* lhs = commandTreeList.front();
                commandTreeList.pop();
                Base* rhs = commandTreeList.front();
                commandTreeList.pop();

                char *temp = connectors.front();
                connectors.pop();

                if(temp == AND_STRING) {
                    AND *n = new AND(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if(temp == OR_STRING) {
                    OR *n = new OR(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if(temp == SEMI_STRING) {
                    Semicolon *n = new Semicolon(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if(temp == INPUT_RED) {
                    InputRed *n = new InputRed(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if(temp == SINGLE_OUTPUT_RED) {
                    SingleOutRed *n = new SingleOutRed(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if(temp == DOUBLE_OUTPUT_RED) {
                    DoubleOutRed *n = new DoubleOutRed(lhs, rhs);
                    completedListToRun.push(n);
                }
                else if(temp == PIPE) {
                    Pipe *n = new Pipe(lhs, rhs);
                    completedListToRun.push(n);
                }
                while(connectors.size() > 0) {
                    Connector *tempLHS = completedListToRun.front();
                    completedListToRun.pop();
                    Base *rhs = commandTreeList.front();
                    commandTreeList.pop();

                    char *temp2 = connectors.front();
                    connectors.pop();

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
                    else if(temp2 == INPUT_RED) {
                        InputRed *n = new InputRed(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                    else if(temp2 == SINGLE_OUTPUT_RED) {
                        SingleOutRed *n = new SingleOutRed(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                    else if(temp2 == DOUBLE_OUTPUT_RED) {
                        DoubleOutRed *n = new DoubleOutRed(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                    else if(temp2 == PIPE) {
                        Pipe *n = new Pipe(tempLHS, rhs);
                        completedListToRun.push(n);
                    }
                }


                Connector *singleRun = completedListToRun.front();
                completedListToRun.pop();
                singleRun->execute(0,1);
           }
           else {
                Base* s = commandTreeList.front();
                commandTreeList.pop();

                if(commandTreeList.size() != 0) {
                    cout << "Error: commandTreeList.size() != 0" << endl;
                    exit(1);
                }
                s->execute(0,1);
            }
        }
        else {
            //Special case where there is only a test command
            size_t foundTest = userInput.find('[');
            if(foundTest!=std::string::npos) {
                string totalString = "";
                char *p = strtok(cstr, " ");
                while(p!=0) {
                    char *checkingTestB = (char *) memchr(p, '[', strlen(p));
                    char *checkingTestE = (char *) memchr(p, ']', strlen(p));

                    //Special case for [(flag) (path)]
                    if(checkingTestB != NULL && checkingTestE != NULL) {
                        totalString += "[ ";
                        splitUpFirstCharacter(p);
                        splitUpLastCharacter(p);
                        totalString += string(p);
                        totalString += " ";
                        totalString += "] ";
                    }
                    else if(checkingTestB != NULL) {
                        totalString += "[ ";
                        splitUpFirstCharacter(p);
                        totalString += string(p);
                        totalString += " ";
                    }
                    else if(checkingTestE != NULL) {
                        splitUpLastCharacter(p);
                        totalString += string(p);
                        totalString += " ";
                        totalString += "] ";
                    }
                    else {
                        totalString += string(p);
                        totalString += " ";
                    }
                    p = strtok(NULL, " ");
                }

                //cout << totalString << endl;
	            char *r = new char[totalString.size()+1];
	            strcpy(r, totalString.c_str());

                Base *s = grabTree(r);
                s->execute(0,1);
            }
            else {
                Base* s = grabTree(cstr);
                s->execute(0,1);
            }
        }

        delete[] cstr;  //Deallocate the memory
    }
    return 0;
}
