#ifndef __PRECEDENCE_CPP__
#define __PRECEDENCE_CPP__

#include "Base.h"

class Precedence: public Base {
    private:
    Base* child;
    public:
    Precedence(Base* c): child(c) {}
    Precedence(): child(NULL) {}
    virtual bool execute() {
        return child->execute();
    }
};

#endif
