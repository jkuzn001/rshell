#ifndef __PRECEDENCE_CPP__
#define __PRECEDENCE_CPP__

#include "Base.h"

#include <cstdlib>

using namespace std;

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
