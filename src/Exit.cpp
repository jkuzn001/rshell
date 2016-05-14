#ifndef __EXIT_CPP__
#define __EXIT_CPP__

#include "Cmd.cpp"
#include "Exit.h"

//Constructors
Exit::Exit()
{}

bool Exit::execute() {
    exit(0);
}

#endif
