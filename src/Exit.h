#ifndef __EXIT_H__
#define __EXIT_H__

#include "Cmd.h"

class Exit: public Cmd {
    public:
    Exit();
    virtual bool execute();
};

#endif
