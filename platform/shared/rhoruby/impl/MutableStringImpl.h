#pragma once


#include "../api/IMutableString.h"
#include "ObjectImpl.h"


namespace rho {
namespace ruby {



class MutableStringImpl : public ObjectImpl  {

public:
    MutableStringImpl(bool is_mutable);
    MutableStringImpl();
    virtual ~MutableStringImpl();

    virtual const char* getClassName();

    virtual const char* getUTF8();
    virtual void setUTF8(const char* utf8_string);
    
private:
    char* mString;
};

// C++ is sucks !
class CMutableString : public IMutableString, public MutableStringImpl  {
public:
    CMutableString(bool is_mutable):MutableStringImpl(is_mutable) {}
    CMutableString() {}
    virtual void addRef() {RefCountedObjectImpl::addRef();}
    virtual void release() {RefCountedObjectImpl::release();}
    virtual const char* getClassName() {return ObjectImpl::getClassName();}
    virtual BASIC_TYPES getBasicType() {return ObjectImpl::getBasicType();}
    virtual const char* getUTF8() {return MutableStringImpl::getUTF8();}
    virtual void setUTF8(const char* utf8_string) {MutableStringImpl::setUTF8(utf8_string);}
};
    
    
}
}
