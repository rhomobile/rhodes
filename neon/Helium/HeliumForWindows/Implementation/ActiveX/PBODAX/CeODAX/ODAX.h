// ODAX.h : Declaration of the CODAX

#pragma once
#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif
#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif
#ifdef SMARTPHONE2003_UI_MODEL
#include "resourcesp.h"
#endif
#ifdef AYGSHELL_UI_MODEL
#include "resourceayg.h"
#endif

#include "CeODAX.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CODAX

class ATL_NO_VTABLE CODAX :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CODAX, &CLSID_ODAX>,
	public IDispatchImpl<IODAX, &IID_IODAX, &LIBID_CeODAXLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IObjectSafetyImpl<CODAX, INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
public:
	CODAX()
	{
	}

#ifndef _CE_DCOM
DECLARE_REGISTRY_RESOURCEID(IDR_ODAX)
#endif


BEGIN_COM_MAP(CODAX)
	COM_INTERFACE_ENTRY(IODAX)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(FileExists)(BSTR filename, VARIANT_BOOL* ret);
	STDMETHOD(Copy)(BSTR srcFileName, BSTR destFileName, LONG* status);
	STDMETHOD(Delete)(BSTR FileName, LONG* status);
	STDMETHOD(Move)(BSTR srcFileName, BSTR destFileName, LONG* status);
	STDMETHOD(Execute)(BSTR sql_statement, int format, BSTR delimiter, VARIANT_BOOL firstrow, int* pcount);
	STDMETHOD(GetLastErrorString)(BSTR* perror);
	STDMETHOD(Select)(BSTR sql_statement, BSTR delimiter, VARIANT_BOOL firstrow, int* pcount);
	STDMETHOD(MoveFirst)(int* presult);
	STDMETHOD(MoveLast)(int* presult);
	STDMETHOD(MoveNext)(int* presult);
	STDMETHOD(MovePrev)(int* presult);
	STDMETHOD(Get)(BSTR column_name, BSTR* pvar);
	STDMETHOD(Clear)(void);

};

OBJECT_ENTRY_AUTO(__uuidof(ODAX), CODAX)
