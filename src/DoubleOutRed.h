#ifndef __DOUBLEOUTRED_H__
#define __DOUBLEOUTRED_H__

#include "Connector.h"
#include "Connector.cpp"

class DoubleOutRed: public Connector {
    public:
    DoubleOutRed(Base* lhs, Base* rhs);
    DoubleOutRed();
    virtual bool execute(int in, int out);
};
#endif
