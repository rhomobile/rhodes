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

// C++ is sucks !
class CNil : public NilImpl, public INil {
public:
    CNil() {}
    virtual ~CNil() {}
    virtual void addRef() {RefCountedObjectImpl::addRef();}
    virtual void release() {RefCountedObjectImpl::release();}
    virtual const char* getClassName() {return ObjectImpl::getClassName();}
    virtual BASIC_TYPES getBasicType() {return ObjectImpl::getBasicType();}};
    
}
}
