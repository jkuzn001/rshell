#ifndef __OR_H__
#define __OR_H__

#include "Connector.h"

class OR: public Connector {
    //constructors
    public:
    OR(Base* lhs, Base* rhs);
    OR();
    //execute function
    virtual bool execute(int in, int out);
};

#endif
