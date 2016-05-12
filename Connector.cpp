#ifndef __CONNECTOR_CPP__
#define __CONNECTOR_CPP__

#include "Connector.h"

Connector::Connector(Base* lhs, Base* rhs): lhs(lhs), rhs(rhs) {}
Connector::Connector(): lhs(NULL), rhs(NULL) {}
#endif
