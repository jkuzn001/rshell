#ifndef __BASE_H__
#define __BASE_H__

class Base {
    public:
    //executes the leaf or composite node in the tree
    virtual bool execute() = 0;
};

#endif
