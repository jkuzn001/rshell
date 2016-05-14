#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include "Base.h"
#include <cstdlib>
using namespace std;

class Connector: public Base {
    protected:

    Base* lhs; //left hand operand of the connector
    Base* rhs; //right hand operand of the connector

    public:

    //constructors
    Connector(Base* lhs, Base* rhs);
    Connector();

    virtual bool execute() = 0; //execute function to be overridden by derived classes
};

#endif
