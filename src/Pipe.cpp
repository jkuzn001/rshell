#ifndef __PIPE_CPP__
#define __PIPE_CPP__

#include "Connector.h"
#include "Pipe.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <fcntl.h>

using namespace std;

Pipe::Pipe(Base* lhs, Base* rhs): Connector(lhs,rhs) {}
Pipe::Pipe(): Connector() {}

//execute function pipes from lhs to rhs
bool Pipe::execute(int in, int out) {
    int fds[2];

    if(pipe(fds) == -1) {
        perror("pipe");
        return false;
    }


    if(!lhs->execute(in,fds[1])) {
        return false;
    }

    close(fds[1]);

    if(!rhs->execute(fds[0],out)) {
        return false;
    }
    close(fds[0]);
    return true;
}

#endif
