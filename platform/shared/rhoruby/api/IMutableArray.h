#pragma once

#include "IArray.h"

namespace rho {
namespace ruby {

class IMutableArray : public IArray {

public:
    virtual ~IMutableArray() {}

    virtual void addItem(IObject* item) = 0;

};

}
}
