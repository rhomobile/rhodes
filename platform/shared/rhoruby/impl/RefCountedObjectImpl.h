#pragma once


#include "../api/IRefCountedObject.h"


namespace rho {
namespace ruby {

class RefCountedObjectImpl : public IRefCountedObject {

public:
    RefCountedObjectImpl();

    virtual ~RefCountedObjectImpl();

    virtual void addRef();

    virtual void release();

private:
    int mRefCounter;

};

}
}
