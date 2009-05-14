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

    typename std::vector<Type>::reference get(typename std::vector<Type>::size_type i){ return at(i);}
    typename std::vector<Type>::const_reference get(typename std::vector<Type>::size_type i) const{ return at(i); }
};

template<class Type>
class VectorPtr : public Vector<Type>{
public:
    VectorPtr(){}
    ~VectorPtr(){clear();}

    void clear()
    {
        for (typename std::vector<Type>::iterator it =Vector<Type>::begin();  it !=Vector<Type>::end(); ++it )
            delete *it;
        
        Vector<Type>::clear();
    }
};

} 

#endif //_RHOSTD_H_
