#ifndef __AND_CPP__
#define __AND_CPP__

#include "Connector.h"
#include "AND.h"

//constructors
AND::AND(Base* lhs, Base* rhs): Connector(lhs,rhs) {}
AND::AND(): Connector() {}
//executes its right child if its left child has executed
//returns true if both children execute successfully 
//otherwise returns false
bool AND::execute() {
    if(lhs->execute()){
       if(rhs->execute()){
           return true;
       }
       else {
           return false;
       }
    }
    else {
        return false;
     }
}

#endif
