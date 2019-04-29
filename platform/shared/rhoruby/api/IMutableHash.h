#pragma once

#include "IHash.h"

namespace rho {
namespace ruby {

class IMutableHash : public IHash {

public:
    virtual ~IMutableHash() {}

    virtual void addItem(const char* key, IObject* item) = 0;

};

}
}
