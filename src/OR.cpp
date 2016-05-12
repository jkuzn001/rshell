#ifndef __OR_CPP__
#define __OR_CPP__

#include "Connector.h"
#include "OR.h"

OR::OR(Base* lhs, Base* rhs): Connector(lhs,rhs) {}
OR::OR(): Connector() {}
bool OR::execute() {
    if(!lhs->execute()) {
        if(rhs->execute()) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return true;
    }
}


#endif
