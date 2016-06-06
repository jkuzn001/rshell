#ifndef __INPUTRED_H__
#define __INPUTRED_H__

#include "Connector.h"
#include "Connector.cpp"

class InputRed: public Connector {
  public:
  InputRed(Base* rhs, Base* lhs);
  InputRed();
  virtual bool execute(int in, int out);

};

#endif
