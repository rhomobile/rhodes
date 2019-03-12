#pragma once
#include "IObject.h"

namespace rho {
namespace ruby {

class IString : public IObject {
    
public:
    virtual ~IString() = 0;
    
    virtual const char* getUTF8() = 0;
    
};

}
}
