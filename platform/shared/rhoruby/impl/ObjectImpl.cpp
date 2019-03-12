
#include "ObjectImpl.h"


namespace rho {
namespace ruby {

    ObjectImpl::ObjectImpl(BASIC_TYPES basicType, char* className)
    :RefCountedObjectImpl() {
        mBasicType = basicType;
        mClassName = className;
    }

    ObjectImpl::~ObjectImpl(){

    }

    const char* ObjectImpl::getClass() {
        return mClassName.c_str();
    }
   BASIC_TYPES ObjectImpl::getBasicType(){
        return mBasicType;
    }


}
}
