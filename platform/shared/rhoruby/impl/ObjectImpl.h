#pragma once


#include "../api/IObject.h"
#include "RefCountedObjectImpl.h"
#include <time.h>
#import "ruby/ext/rho/rhoruby.h"

//#include "common/RhoStd.h"

namespace rho {
namespace ruby {



class ObjectImpl : public RefCountedObjectImpl   {

public:
    ObjectImpl(BASIC_TYPES basicType, const char* className, VALUE value);
    ObjectImpl();
    void setBasicType(BASIC_TYPES basicType);
    void setClassName(const char* className);
    void setValue(VALUE value);
    VALUE getValue();
    
    virtual ~ObjectImpl();
    virtual const char* getClassName();
    virtual BASIC_TYPES getBasicType();

private:
    BASIC_TYPES mBasicType;
    char* mClassName;
    VALUE mValue;
};
    
    
// C++ is sucks !
class CObject : public ObjectImpl, public IObject {
public:
    CObject(BASIC_TYPES basicType, const char* className, VALUE value):ObjectImpl(basicType, className, value) {}
    CObject() {}
    virtual void addRef() {RefCountedObjectImpl::addRef();}
    virtual void release() {RefCountedObjectImpl::release();}
    virtual const char* getClassName() {return ObjectImpl::getClassName();}
    virtual BASIC_TYPES getBasicType() {return ObjectImpl::getBasicType();}
};
    
    
}
}
