#ifndef __FILE_H__
#define __FILE_H__

#include "Base.h"

#include <string>

using namespace std;

class File: public Base {
    private:
    string filename;

    public:
    File(string filename);
    virtual bool execute(int in, int out);

};


#endif
