#pragma once


#include "../api/INil.h"
#include "ObjectImpl.h"

namespace rho {
namespace ruby {


class NilImpl : public ObjectImpl {

public:
    NilImpl();
    virtual ~NilImpl();

    virtual const char* getClassName();
};

}
}
