#ifndef __PIPE_H__
#define __PIPE_H__

#include "Connector.h"
#include "Connector.cpp"

class Pipe: public Connector {
    public:
    Pipe(Base* lhs, Base* rhs);
    Pipe();
    virtual bool execute(int in, int out);

};


#endif
