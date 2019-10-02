#pragma once


#include "../api/IMutableBoolean.h"
#include "ObjectImpl.h"


namespace rho {
namespace ruby {



class MutableBooleanImpl : public ObjectImpl  {

public:
    MutableBooleanImpl(bool is_mutable);
    MutableBooleanImpl();
    virtual ~MutableBooleanImpl();

    virtual const char* getClassName();

    virtual bool getBool();
    virtual void setBool(bool value);

private:
    bool mValue;
};

// C++ is sucks !
class CMutableBoolean : public IMutableBoolean, public MutableBooleanImpl  {
public:
    CMutableBoolean(bool is_mutable):MutableBooleanImpl(is_mutable) {}
    CMutableBoolean() {}
    virtual void addRef() {RefCountedObjectImpl::addRef();}
    virtual void release() {RefCountedObjectImpl::release();}
    virtual const char* getClassName() {return ObjectImpl::getClassName();}
    virtual BASIC_TYPES getBasicType() {return ObjectImpl::getBasicType();}
    virtual bool getBool() {return MutableBooleanImpl::getBool();}
    virtual void setBool(bool value) {MutableBooleanImpl::setBool(value);}
};


}
}
