#ifndef __BASE_H__
#define __BASE_H__

#include <string>
using namespace std;
class Base {
    public:
    //executes the leaf or composite node in the tree
    virtual bool execute(int in, int out) = 0;
    virtual string get_data() = 0;
};

#endif
