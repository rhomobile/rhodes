#pragma once


#include "../api/IObject.h"
#include "RefCountedObjectImpl.h"
#include "common/RhoStd.h"

namespace rho {
namespace ruby {



class ObjectImpl : public IObject, RefCountedObjectImpl  {

public:
    ObjectImpl(BASIC_TYPES basicType, char* className);
    virtual ~ObjectImpl();
    virtual const char* getClass();
    virtual BASIC_TYPES getBasicType();

    virtual void addRef();
    virtual void release();


private:
    BASIC_TYPES mBasicType;
    rho::String mClassName;
};

}
}
