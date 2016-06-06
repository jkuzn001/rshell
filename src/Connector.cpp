#ifndef __CONNECTOR_CPP__
#define __CONNECTOR_CPP__

#include "Connector.h"

Connector::Connector(Base* lhs, Base* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

Connector::Connector()  {
    this->lhs = NULL;
    this->lhs = NULL;
}
string Connector::get_data() {
    return "error called get data on connector";
}
#endif
