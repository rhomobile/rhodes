#pragma once


#include "../api/IMutableHash.h"
#include "ObjectImpl.h"
#include <vector>
#include <string>
#include <map>

namespace rho {
namespace ruby {



class MutableHashImpl : public ObjectImpl  {

public:
    MutableHashImpl(bool is_mutable);
    MutableHashImpl();
    virtual ~MutableHashImpl();

    virtual const char* getClassName();

    virtual IArray* getKeys();
    virtual IObject* getItem(const char* key);
    virtual void addItem(const char* key, IObject* item);


private:
    std::map<std::string, IObject*> mHash;
};

// C++ is sucks !
class CMutableHash : public IMutableHash, public MutableHashImpl  {
public:
    CMutableHash(bool is_mutable):MutableHashImpl(is_mutable) {}
    CMutableHash() {}
    virtual void addRef() {RefCountedObjectImpl::addRef();}
    virtual void release() {RefCountedObjectImpl::release();}
    virtual const char* getClassName() {return ObjectImpl::getClassName();}
    virtual BASIC_TYPES getBasicType() {return ObjectImpl::getBasicType();}
    virtual IArray* getKeys() {return MutableHashImpl::getKeys();}
    virtual IObject* getItem(const char* key) {return MutableHashImpl::getItem(key);}
    virtual void addItem(const char* key, IObject* item) {MutableHashImpl::addItem(key, item);}
};


}
}
