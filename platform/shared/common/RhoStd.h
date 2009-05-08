#ifndef _RHOSTD_H_
#define _RHOSTD_H_

#include "RhoPort.h"

#include <string>
#include <vector>

namespace rho {

typedef std::string String;
typedef std::wstring StringW;
typedef bool boolean;
//#define Vector std::vector

template<class Type>
class Vector : public std::vector<Type>{
public:
    Vector(){}

    void add(const Type& item)
    {
        push_back(item);
    }

    reference get(size_type i){ return at(i);}
    const_reference get(size_type i) const{ return at(i); }
};

template<class Type>
class VectorPtr : public Vector<Type>{
public:
    VectorPtr(){}
    ~VectorPtr(){clear();}

    void clear()
    {
        for (iterator it = begin();  it != end(); ++it )
            delete *it;
        
        Vector<Type>::clear();
    }
};

} 

#endif //_RHOSTD_H_
