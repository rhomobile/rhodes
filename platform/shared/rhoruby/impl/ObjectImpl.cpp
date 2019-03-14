
#include "ObjectImpl.h"
#include <stdlib.h>
#include "string.h"

namespace rho {
namespace ruby {

ObjectImpl::ObjectImpl(BASIC_TYPES basicType, const char* className, VALUE value)
:RefCountedObjectImpl() {
    mBasicType = basicType;
    mClassName = NULL;
    setClassName(className);
    mValue = NULL;
    setValue(value);
}
    
ObjectImpl::ObjectImpl() {
    mBasicType = BASIC_TYPES::None;
    mClassName = NULL;
    mValue = NULL;
}

void ObjectImpl::setBasicType(BASIC_TYPES basicType){
    mBasicType = basicType;
}

void ObjectImpl::setClassName(const char* className) {
    if (mClassName != NULL) {
        free(mClassName);
        mClassName = NULL;
    }
    if (className != NULL) {
        int len = (int)strlen(className);
        mClassName = (char*)malloc(len+1);
        strcpy(mClassName, className);
    }

}


ObjectImpl::~ObjectImpl(){
    if (mClassName != NULL) {
        free(mClassName);
    }
    if (mValue != NULL) {
        rho_ruby_releaseValue(mValue);
    }
}

const char* ObjectImpl::getClassName() {
    return (const char*)mClassName;
}
    
BASIC_TYPES ObjectImpl::getBasicType(){
    return mBasicType;
}

void ObjectImpl::setValue(VALUE value) {
    if (mValue != NULL) {
        rho_ruby_releaseValue(mValue);
    }
    mValue = NULL;
    if (value != NULL) {
        mValue = value;
        rho_ruby_holdValue(mValue);
    }
}

VALUE ObjectImpl::getValue() {
    return mValue;
}

    

}
}
