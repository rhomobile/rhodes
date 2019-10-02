#pragma once


#include "../api/IRefCountedObject.h"


namespace rho {
namespace ruby {

class RefCountedObjectImpl {

public:
    RefCountedObjectImpl();

    virtual ~RefCountedObjectImpl();

    virtual void addRef();

    virtual void release();

private:
    int mRefCounter;

};

class CRefCountedObject : public RefCountedObjectImpl, public IRefCountedObject {
    CRefCountedObject() {}
    virtual void addRef() {RefCountedObjectImpl::addRef();}
    virtual void release() {RefCountedObjectImpl::release();}
};
    
}
}
