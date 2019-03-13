#pragma once


#include "../api/IObject.h"
#include "RefCountedObjectImpl.h"
//#include "common/RhoStd.h"

namespace rho {
namespace ruby {



class ObjectImpl : public IObject, RefCountedObjectImpl  {

public:
    ObjectImpl(BASIC_TYPES basicType, const char* className);
    ObjectImpl();
    void setBasicType(BASIC_TYPES basicType);
    void setClassName(const char* className);
    virtual ~ObjectImpl();
    virtual const char* getClassName();
    virtual BASIC_TYPES getBasicType();

    virtual void addRef();
    virtual void release();


private:
    BASIC_TYPES mBasicType;
    char* mClassName;
};

}
}
