#ifndef __SINGLEOUTRED_H__
#define __SINGLEOUTRED_H__

#include "Connector.h"
#include "Connector.cpp"

class SingleOutRed: public Connector {
    public:
    SingleOutRed(Base* lhs, Base* rhs);
    SingleOutRed();
    virtual bool execute(int in, int out);
};


#endif
