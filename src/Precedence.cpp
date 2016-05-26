#ifndef __PARENTHESIS_CPP__
#define __PARENTHESIS_CPP__

#include "Base.h"

class Parenthesis: public Base {
    private:
    Base* child;
    public:
    virtual bool execute() {
        return child->execute();
    }
};

#endif
