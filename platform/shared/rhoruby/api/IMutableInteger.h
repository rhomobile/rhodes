#pragma once
#include "IInteger.h"

namespace rho {
namespace ruby {

class IMutableInteger : public IInteger {

public:
    virtual ~IMutableInteger() = 0;

    virtual void setLong(long int value) = 0;

};

}
}
