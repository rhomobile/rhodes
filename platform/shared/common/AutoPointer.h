/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#pragma once

#include "common/IRhoThreadImpl.h"

namespace rho {
namespace common {

//From boost
class CNonCopyable
{
 protected:
    CNonCopyable() {}
    ~CNonCopyable() {}
 private:  // emphasize the following members are private
    CNonCopyable( const CNonCopyable& );
    const CNonCopyable& operator=( const CNonCopyable& );
};

template <typename PTRTYPE>
class CBaseAutoPointer //: public CNonCopyable
{
public:
    CBaseAutoPointer() { m_ptr = 0; }

    PTRTYPE* operator &(){ Close(); return &m_ptr; }
    void Set( PTRTYPE ptr ){ Close(); m_ptr = ptr; }

    PTRTYPE operator ->(){ return m_ptr; }
    operator PTRTYPE(){ return m_ptr; }

    const PTRTYPE operator ->() const { return m_ptr; }
    operator const PTRTYPE() const { return m_ptr; }

    virtual void FreePtr() = 0;
    void Close()
    {
        if ( m_ptr ) 
        { 
            FreePtr(); 
            m_ptr = 0;
        }
    }
    PTRTYPE ReleasePtr()
    {
        PTRTYPE ptr = m_ptr;
        m_ptr = 0;
        return ptr;
    }

protected:
    PTRTYPE m_ptr;
};

template <typename TYPE, typename PTRTYPE=TYPE*>
class CAutoPtr : public CBaseAutoPointer<PTRTYPE>
{
public:
    CAutoPtr( PTRTYPE ptr ){   CBaseAutoPointer<PTRTYPE>::Set(ptr); }
    CAutoPtr(){}
    ~CAutoPtr(){ CBaseAutoPointer<PTRTYPE>::Close(); }

    CAutoPtr( const CAutoPtr& orig){ *this = orig; }
    CAutoPtr& operator=( const CAutoPtr& orig)
    {
        if ( CBaseAutoPointer<PTRTYPE>::m_ptr ) 
            FreePtr(); 

        CBaseAutoPointer<PTRTYPE>::m_ptr = orig.m_ptr;
        const_cast<CAutoPtr&>(orig).m_ptr = 0;
        return *this;
    }

    bool operator==(const CAutoPtr& orig)const{ return true;}

    //operator PTRTYPE(){ return m_ptr; }
    virtual void FreePtr()
    { 
        delete CBaseAutoPointer<PTRTYPE>::m_ptr;
    }
};

template <typename PTRTYPE, class BaseClass>
class CAutoPointer1 : public CBaseAutoPointer<PTRTYPE>, public BaseClass
{
public:
    ~CAutoPointer1(){ CBaseAutoPointer<PTRTYPE>::Close(); }

    virtual void FreePtr()
    { 
        (*((BaseClass*)this))(CBaseAutoPointer<PTRTYPE>::m_ptr); 
    }
};
/*
class CFunctor_NetApiBufferFree
{
public:
    void operator() ( void* pObject ){ NetApiBufferFree(pObject); }
};*/

template <typename PTRTYPE, typename FUNCTYPE, FUNCTYPE pFunc>
class CAutoPointer2 : public CBaseAutoPointer<PTRTYPE>
{
public:
    ~CAutoPointer2(){ CBaseAutoPointer<PTRTYPE>::Close(); }

    virtual void FreePtr()
    { 
        if(pFunc) 
            (*pFunc)(CBaseAutoPointer<PTRTYPE>::m_ptr); 
    }
};

template <typename PTRTYPE>
class CAutoPointer : public CBaseAutoPointer<PTRTYPE>
{
    struct CBaseDeleter{ virtual void Delete(PTRTYPE) = 0; };

    template <typename FUNCTYPE>
    struct CDeleter : public CBaseDeleter
    {
        CDeleter( FUNCTYPE pFunc ): m_pFunc(pFunc){;}
        void Delete(PTRTYPE ptr)
        { 
            m_pFunc(ptr); 
        }

        FUNCTYPE m_pFunc;
    };

public:

    template <typename FUNCTYPE>
    CAutoPointer( FUNCTYPE pFunc ) 
    { 
        static CDeleter<FUNCTYPE> oDeleter(pFunc);
        CBaseAutoPointer<PTRTYPE>::m_ptr = 0; 
        m_pDeleter = &oDeleter;
    }
    ~CAutoPointer(){ CBaseAutoPointer<PTRTYPE>::Close(); }

    virtual void FreePtr()
    { 
        if(m_pDeleter) 
            m_pDeleter->Delete(CBaseAutoPointer<PTRTYPE>::m_ptr); 
    }
private:
    CBaseDeleter* m_pDeleter;
};

template <typename FUNCTYPE, typename PARAMTYPE>
class CStaticClassFunctor : public rho::common::IRhoRunnable
{
public:

    CStaticClassFunctor( FUNCTYPE pFunc, PARAMTYPE param ): m_pFunc(pFunc), m_param(param){}
    virtual void runObject()
    { 
        m_pFunc(m_param); 
    }

    ~CStaticClassFunctor()
    {
    }

private:
    FUNCTYPE m_pFunc;
    PARAMTYPE m_param;

};

template <typename PARAMTYPE>
class CInstanceClassFunctorBase : public rho::common::IRhoRunnable
{
public:

    CInstanceClassFunctorBase( PARAMTYPE param ): m_param(param){}

    PARAMTYPE& getLastParam(){ return m_param; }
protected:
    PARAMTYPE m_param;
};

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE>
class CInstanceClassFunctor1 : public CInstanceClassFunctorBase<PARAMTYPE>
{
public:

    CInstanceClassFunctor1( OBJTYPE obj, FUNCTYPE pFunc, PARAMTYPE param ): CInstanceClassFunctorBase<PARAMTYPE>(param), m_obj(obj), m_pFunc(pFunc){}
    virtual void runObject()
    { 
        (m_obj->*m_pFunc)(CInstanceClassFunctorBase<PARAMTYPE>::m_param); 
    }

    ~CInstanceClassFunctor1()
    {
    }

protected:
    OBJTYPE   m_obj;
    FUNCTYPE m_pFunc;
};

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE>
CInstanceClassFunctorBase<PARAMTYPE>* rho_makeInstanceClassFunctor1( OBJTYPE obj,FUNCTYPE pFunc, PARAMTYPE param )
{
    return new rho::common::CInstanceClassFunctor1<OBJTYPE,FUNCTYPE,PARAMTYPE>(obj, pFunc, param);
}

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2 >
class CInstanceClassFunctor2 : public CInstanceClassFunctorBase<PARAMTYPE2>
{
public:

    CInstanceClassFunctor2( OBJTYPE obj, FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2 ): CInstanceClassFunctorBase<PARAMTYPE2>(param2), m_obj(obj), m_pFunc(pFunc), m_param1(param1){}
    virtual void runObject()
    { 
        (m_obj->*m_pFunc)(m_param1, CInstanceClassFunctorBase<PARAMTYPE2>::m_param); 
    }

    ~CInstanceClassFunctor2()
    {
    }

protected:
    OBJTYPE   m_obj;
    FUNCTYPE m_pFunc;
    PARAMTYPE1 m_param1;
};

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2>
CInstanceClassFunctorBase<PARAMTYPE2>* rho_makeInstanceClassFunctor2( OBJTYPE obj,FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2 )
{
    return new rho::common::CInstanceClassFunctor2<OBJTYPE, FUNCTYPE,PARAMTYPE1, PARAMTYPE2>(obj, pFunc, param1, param2);
}

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2, typename PARAMTYPE3 >
class CInstanceClassFunctor3 : public CInstanceClassFunctorBase<PARAMTYPE3>
{
public:

    CInstanceClassFunctor3( OBJTYPE obj, FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2, PARAMTYPE3 param3 ): CInstanceClassFunctorBase<PARAMTYPE3>(param3), m_obj(obj), m_pFunc(pFunc), m_param1(param1), m_param2(param2){}
    virtual void runObject()
    { 
        (m_obj->*m_pFunc)(m_param1, m_param2, CInstanceClassFunctorBase<PARAMTYPE3>::m_param); 
    }

    ~CInstanceClassFunctor3()
    {
    }

protected:
    OBJTYPE   m_obj;
    FUNCTYPE m_pFunc;
    PARAMTYPE1 m_param1;
    PARAMTYPE2 m_param2;
};

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2, typename PARAMTYPE3>
CInstanceClassFunctorBase<PARAMTYPE3>* rho_makeInstanceClassFunctor3( OBJTYPE obj,FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2, PARAMTYPE3 param3 )
{
    return new rho::common::CInstanceClassFunctor3<OBJTYPE, FUNCTYPE,PARAMTYPE1, PARAMTYPE2, PARAMTYPE3>(obj, pFunc, param1, param2, param3);
}

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2, typename PARAMTYPE3, typename PARAMTYPE4 >
class CInstanceClassFunctor4 : public CInstanceClassFunctorBase<PARAMTYPE4>
{
public:

    CInstanceClassFunctor4( OBJTYPE obj, FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2, PARAMTYPE3 param3, PARAMTYPE4 param4 ): CInstanceClassFunctorBase<PARAMTYPE4>(param4), m_obj(obj), m_pFunc(pFunc), m_param1(param1), m_param2(param2), m_param3(param3){}
    virtual void runObject()
    { 
        (m_obj->*m_pFunc)(m_param1, m_param2, m_param3, CInstanceClassFunctorBase<PARAMTYPE4>::m_param); 
    }

    ~CInstanceClassFunctor4()
    {
    }

protected:
    OBJTYPE   m_obj;
    FUNCTYPE m_pFunc;
    PARAMTYPE1 m_param1;
    PARAMTYPE2 m_param2;
    PARAMTYPE3 m_param3;
};

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2, typename PARAMTYPE3, typename PARAMTYPE4>
CInstanceClassFunctorBase<PARAMTYPE4>* rho_makeInstanceClassFunctor4( OBJTYPE obj,FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2, PARAMTYPE3 param3, PARAMTYPE4 param4 )
{
    return new rho::common::CInstanceClassFunctor4<OBJTYPE, FUNCTYPE,PARAMTYPE1, PARAMTYPE2, PARAMTYPE3, PARAMTYPE4>(obj, pFunc, param1, param2, param3, param4);
}

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2, typename PARAMTYPE3, typename PARAMTYPE4, typename PARAMTYPE5 >
class CInstanceClassFunctor5 : public CInstanceClassFunctorBase<PARAMTYPE5>
{
public:

    CInstanceClassFunctor5( OBJTYPE obj, FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2, PARAMTYPE3 param3, PARAMTYPE4 param4, PARAMTYPE5 param5 ): CInstanceClassFunctorBase<PARAMTYPE5>(param5), m_obj(obj), m_pFunc(pFunc), m_param1(param1), m_param2(param2), m_param3(param3), m_param4(param4){}
    virtual void runObject()
    { 
        (m_obj->*m_pFunc)(m_param1, m_param2, m_param3, m_param4, CInstanceClassFunctorBase<PARAMTYPE5>::m_param); 
    }

    ~CInstanceClassFunctor5()
    {
    }

protected:
    OBJTYPE   m_obj;
    FUNCTYPE m_pFunc;
    PARAMTYPE1 m_param1;
    PARAMTYPE2 m_param2;
    PARAMTYPE3 m_param3;
    PARAMTYPE4 m_param4;
};

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2, typename PARAMTYPE3, typename PARAMTYPE4, typename PARAMTYPE5>
CInstanceClassFunctorBase<PARAMTYPE5>* rho_makeInstanceClassFunctor5( OBJTYPE obj,FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2, PARAMTYPE3 param3, PARAMTYPE4 param4, PARAMTYPE5 param5  )
{
    return new rho::common::CInstanceClassFunctor5<OBJTYPE, FUNCTYPE,PARAMTYPE1, PARAMTYPE2, PARAMTYPE3, PARAMTYPE4, PARAMTYPE5>(obj, pFunc, param1, param2, param3, param4, param5);
}

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2, typename PARAMTYPE3, typename PARAMTYPE4, typename PARAMTYPE5, typename PARAMTYPE6 >
class CInstanceClassFunctor6 : public CInstanceClassFunctorBase<PARAMTYPE6>
{
public:

    CInstanceClassFunctor6( OBJTYPE obj, FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2, PARAMTYPE3 param3, PARAMTYPE4 param4, PARAMTYPE5 param5, PARAMTYPE6 param6 ): CInstanceClassFunctorBase<PARAMTYPE6>(param6), m_obj(obj), m_pFunc(pFunc), m_param1(param1), m_param2(param2), m_param3(param3), m_param4(param4), m_param5(param5){}
    virtual void runObject()
    { 
        (m_obj->*m_pFunc)(m_param1, m_param2, m_param3, m_param4, m_param5, CInstanceClassFunctorBase<PARAMTYPE6>::m_param); 
    }

    ~CInstanceClassFunctor6()
    {
    }

protected:
    OBJTYPE   m_obj;
    FUNCTYPE m_pFunc;
    PARAMTYPE1 m_param1;
    PARAMTYPE2 m_param2;
    PARAMTYPE3 m_param3;
    PARAMTYPE4 m_param4;
    PARAMTYPE5 m_param5;
};

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2, typename PARAMTYPE3, typename PARAMTYPE4, typename PARAMTYPE5, typename PARAMTYPE6>
CInstanceClassFunctorBase<PARAMTYPE6>* rho_makeInstanceClassFunctor6( OBJTYPE obj,FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2, PARAMTYPE3 param3, PARAMTYPE4 param4, PARAMTYPE5 param5, PARAMTYPE6 param6  )
{
    return new rho::common::CInstanceClassFunctor6<OBJTYPE, FUNCTYPE,PARAMTYPE1, PARAMTYPE2, PARAMTYPE3, PARAMTYPE4, PARAMTYPE5, PARAMTYPE6>(obj, pFunc, param1, param2, param3, param4, param5, param6);
}

}
}
