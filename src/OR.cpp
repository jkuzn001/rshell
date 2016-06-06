#ifndef __OR_CPP__
#define __OR_CPP__

#include "Connector.h"
#include "OR.h"

//Constructors
OR::OR(Base* lhs, Base* rhs): Connector(lhs,rhs) {}
OR::OR(): Connector() {}

//executes its right child if its left child has executed
//returns true if both children execute successfully
//otherwise returns false
bool OR::execute(int in, int out) {
    if(!lhs->execute(0,1)) {
        if(rhs->execute(0,1)) {
            return true;
        }
        else {
            return false;
        }
    }
    return true;
}


#endif
