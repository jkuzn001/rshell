#ifndef __INPUTRED_CPP__
#define __INPUTRED_CPP__

#include "Connector.h"
#include "InputRed.h"

#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fcntl.h>
#include <string>

using namespace std;

InputRed::InputRed(Base* lhs, Base* rhs): Connector(lhs,rhs) {}
InputRed::InputRed(): Connector() {}

//redirects input from the standard in to the input given on the command line
bool InputRed::execute(int in, int out) {
    string infile = rhs->get_data();
    in = open(infile.c_str(),O_RDONLY);

    return lhs->execute(in,1);
}



#endif
