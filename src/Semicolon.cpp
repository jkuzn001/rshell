#ifndef __SEMICOLON_CPP__
#define __SEMICOLON_CPP__

#include "Connector.h"
#include "Semicolon.h"

//constructors
Semicolon::Semicolon(Base* lhs, Base* rhs): Connector(lhs,rhs) {}
Semicolon::Semicolon(): Connector() {}

//executes both of its children regardless of the success of the
//other child
//returns true or false based on the success of the right child
bool Semicolon::execute(int in, int out) {
    lhs->execute(in,1);
    if(rhs->execute(0,out)) {
        return true;
    }
    else {
        return false;
   }
}

#endif
