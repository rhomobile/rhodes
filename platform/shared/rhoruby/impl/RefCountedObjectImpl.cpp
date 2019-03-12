
#include "RefCountedObjectImpl.h"


namespace rho {
namespace ruby {

RefCountedObjectImpl::RefCountedObjectImpl() {
    mRefCounter = 1;
}

RefCountedObjectImpl::~RefCountedObjectImpl() {

}

void RefCountedObjectImpl::addRef() {
    mRefCounter++;
}

void RefCountedObjectImpl::release() {
    mRefCounter--;
    if (mRefCounter <= 0) {
        delete this;
    }
}

}
}
