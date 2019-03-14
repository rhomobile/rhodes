#pragma once


#include "../api/IMutableInteger.h"
#include "ObjectImpl.h"


namespace rho {
namespace ruby {



class MutableIntegerImpl : public ObjectImpl  {

public:
    MutableIntegerImpl(bool is_mutable);
    MutableIntegerImpl();
    virtual ~MutableIntegerImpl();

    virtual const char* getClassName();

    virtual long int getLong();
    virtual void setLong(long int value);

private:
    long int mValue;
};

// C++ is sucks !
class CMutableInteger : public IMutableInteger, public MutableIntegerImpl  {
public:
    CMutableInteger(bool is_mutable):MutableIntegerImpl(is_mutable) {}
    CMutableInteger() {}
    virtual void addRef() {RefCountedObjectImpl::addRef();}
    virtual void release() {RefCountedObjectImpl::release();}
    virtual const char* getClassName() {return ObjectImpl::getClassName();}
    virtual BASIC_TYPES getBasicType() {return ObjectImpl::getBasicType();}
    virtual long int getLong() {return MutableIntegerImpl::getLong();}
    virtual void setLong(long int value) {MutableIntegerImpl::setLong(value);}
};


}
}
