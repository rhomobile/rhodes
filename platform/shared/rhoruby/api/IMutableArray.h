#pragma once

#include "IArray.h"

namespace rho {
namespace ruby {

class IMutableArray : public IArray {

public:
    virtual ~IMutableArray() = 0;

    virtual void addItem(IObject* item) = 0;

};

}
}
