#ifndef _RHOSTD_H_
#define _RHOSTD_H_

#include "RhoPort.h"

#include <string>
#include <vector>
#include <map>

namespace rho {

typedef std::string String;
typedef std::wstring StringW;
typedef bool boolean;
//#define Vector std::vector

#define null 0

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
        for (typename std::vector<Type>::iterator it = Vector<Type>::begin();  it !=Vector<Type>::end(); ++it )
            delete *it;
        
        Vector<Type>::clear();
    }
};

template<class TKEY, class TVALUE>
class Hashtable : public std::map<TKEY,TVALUE>
{
public:
    Hashtable(){}

    void put(const TKEY& key, const TVALUE& val)
    {
        (*this)[key] = val;
    }

    void remove(const TKEY& key)
    {
        erase(key);
    }

    TVALUE& get(const TKEY& key)
    {
        return (*this)[key];
    }
};

template<class TKEY, class TVALUE>
class HashtablePtr : public Hashtable<TKEY,TVALUE>
{
public:
    ~HashtablePtr()
    {
        clear();
    }
    void clear()
    {
        for (typename std::map<TKEY,TVALUE>::iterator it = Hashtable<TKEY,TVALUE>::begin();  it != Hashtable<TKEY,TVALUE>::end(); ++it )
            delete it->second;
        
        Hashtable<TKEY,TVALUE>::clear();
    }

    void remove(const TKEY& key)
    {
        typename std::map<TKEY,TVALUE>::iterator it = find(key);
        if ( it != Hashtable<TKEY,TVALUE>::end() )
            delete it->second;

        erase(key);
    }

};

} 

#endif //_RHOSTD_H_
