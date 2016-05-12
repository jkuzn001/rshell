#ifndef __SEMICOLON_H__
#define __SEMICOLON_H__
#include "Connector.cpp"

class Semicolon: public Connector {
    public:
    //constructors
    Semicolon(Base* lhs, Base* rhs);
    Semicolon();
    //executes both of its children regardless of the success of the 
    //other child
    //returns true or false based on the success of the right child
    virtual bool execute();
};

#endif
