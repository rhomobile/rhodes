#pragma once


#include "../api/IMutableArray.h"
#include "ObjectImpl.h"
#include <vector>

namespace rho {
namespace ruby {



class MutableArrayImpl : public ObjectImpl  {

public:
    MutableArrayImpl(bool is_mutable);
    MutableArrayImpl();
    virtual ~MutableArrayImpl();

    virtual const char* getClassName();

    virtual int getItemsCount();
    virtual IObject* getItem(int index);
    virtual void addItem(IObject* item);
    
    void reserve(int capacity);

private:
    std::vector<IObject*> mVector;
};

// C++ is sucks !
class CMutableArray : public IMutableArray, public MutableArrayImpl  {
public:
    CMutableArray(bool is_mutable):MutableArrayImpl(is_mutable) {}
    CMutableArray() {}
    virtual void addRef() {RefCountedObjectImpl::addRef();}
    virtual void release() {RefCountedObjectImpl::release();}
    virtual const char* getClassName() {return ObjectImpl::getClassName();}
    virtual BASIC_TYPES getBasicType() {return ObjectImpl::getBasicType();}
    virtual int getItemsCount() {return MutableArrayImpl::getItemsCount();}
    virtual IObject* getItem(int index) {return MutableArrayImpl::getItem(index);}
    virtual void addItem(IObject* item) {MutableArrayImpl::addItem(item);}
};


}
}
