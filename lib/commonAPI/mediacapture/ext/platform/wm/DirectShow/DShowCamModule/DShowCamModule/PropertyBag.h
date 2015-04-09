#ifndef PROPERTYBAG_H 
#define PROPERTYBAG_H

struct VAR_LIST
{
    VARIANT var;
    VAR_LIST *pNext;
    BSTR pBSTRName;
};


class CPropertyBag : public IPropertyBag
{  
public:
    CPropertyBag();
    ~CPropertyBag();
    
    HRESULT STDMETHODCALLTYPE
    Read(
        LPCOLESTR pszPropName, 
        VARIANT *pVar, 
        IErrorLog *pErrorLog
        );
    
    
    HRESULT STDMETHODCALLTYPE
    Write(
        LPCOLESTR pszPropName, 
        VARIANT *pVar
        );
        
    ULONG STDMETHODCALLTYPE AddRef();        
    ULONG STDMETHODCALLTYPE Release();        
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);   

private:
     ULONG _refCount;
     VAR_LIST *pVar;
};

#endif
