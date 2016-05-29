#ifndef __TEST_H__
#define __TEST_H__

#include "Base.h"
#include "Cmd.h"

class Test: public Cmd {
    public:
    Test(queue<char*>);
    Test();
    virtual bool execute();
};
#endif
