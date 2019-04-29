#pragma once


#include "../api/IMutableFloat.h"
#include "ObjectImpl.h"


namespace rho {
namespace ruby {



class MutableFloatImpl : public ObjectImpl  {

public:
    MutableFloatImpl(bool is_mutable);
    MutableFloatImpl();
    virtual ~MutableFloatImpl();

    virtual const char* getClassName();

    virtual double getDouble();
    virtual void setDouble(double value);

private:
    double mValue;
};

// C++ is sucks !
class CMutableFloat : public IMutableFloat, public MutableFloatImpl  {
public:
    CMutableFloat(bool is_mutable):MutableFloatImpl(is_mutable) {}
    CMutableFloat() {}
    virtual void addRef() {RefCountedObjectImpl::addRef();}
    virtual void release() {RefCountedObjectImpl::release();}
    virtual const char* getClassName() {return ObjectImpl::getClassName();}
    virtual BASIC_TYPES getBasicType() {return ObjectImpl::getBasicType();}
    virtual double getDouble() {return MutableFloatImpl::getDouble();}
    virtual void setDouble(double value) {MutableFloatImpl::setDouble(value);}
};


}
}
