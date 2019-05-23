#pragma once
#include "IInteger.h"

namespace rho {
namespace ruby {

class IMutableInteger : public IInteger {

public:
    virtual ~IMutableInteger() {}

    virtual void setLong(long int value) = 0;

};

}
}
