#pragma once

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
class CBaseAutoPointer : public CNonCopyable
{
public:
    CBaseAutoPointer() { m_ptr = NULL; }

    PTRTYPE* operator &(){ Close(); return &m_ptr; }
    void Set( PTRTYPE ptr ){ Close(); m_ptr = ptr; }

    PTRTYPE operator ->(){ return m_ptr; }
    operator PTRTYPE(){ return m_ptr; }

    virtual void FreePtr() = 0;
    void Close()
    {
        if ( m_ptr ) 
        { 
            FreePtr(); 
            m_ptr = NULL;
        }
    }

protected:
    PTRTYPE m_ptr;
};

template <typename TYPE, typename PTRTYPE=TYPE*>
class CAutoPtr : public CBaseAutoPointer<PTRTYPE>
{
public:
    CAutoPtr( PTRTYPE ptr ){ Set(ptr); }
    ~CAutoPtr(){ Close(); }

    virtual void FreePtr()
    { 
        delete m_ptr;
    }
};

template <typename PTRTYPE, class BaseClass>
class CAutoPointer1 : public CBaseAutoPointer<PTRTYPE>, public BaseClass
{
public:
    ~CAutoPointer1(){ Close(); }

    virtual void FreePtr()
    { 
        (*((BaseClass*)this))(m_ptr); 
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
    ~CAutoPointer2(){ Close(); }

    virtual void FreePtr()
    { 
        if(pFunc) 
            (*pFunc)(m_ptr); 
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
        m_ptr = NULL; 
        m_pDeleter = &oDeleter;
    }
    ~CAutoPointer(){ Close(); }

    virtual void FreePtr()
    { 
        if(m_pDeleter) 
            m_pDeleter->Delete(m_ptr); 
    }
private:
    CBaseDeleter* m_pDeleter;
};

}
}
