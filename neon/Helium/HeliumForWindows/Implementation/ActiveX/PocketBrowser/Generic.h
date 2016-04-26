// Generic.h : Declaration of the CGeneric

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

#include "PocketBrowser.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CGeneric


class ATL_NO_VTABLE CGeneric :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGeneric, &CLSID_Generic>,
	public IDispatchImpl<IGeneric, &IID_IGeneric, &LIBID_PocketBrowserLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IObjectSafetyImpl<CGeneric, INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>,
	public ISupportErrorInfoImpl<&IID_IGeneric>
{
public:
	CGeneric()
	{
	}

#ifndef _CE_DCOM
DECLARE_REGISTRY_RESOURCEID(IDR_GENERIC)
#endif


BEGIN_COM_MAP(CGeneric)
	COM_INTERFACE_ENTRY(IGeneric)
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

private:
	bool GetPPC_UUID (WCHAR *puuid);
	bool GetCE_UUID (WCHAR *puuid);
	void BytesToHexStr(LPTSTR lpHexStr, LPBYTE lpBytes, int nSize);

public:
	STDMETHOD(PlayWave)(BSTR strSound, LONG iFlags);
	STDMETHOD(PlaySound)(BSTR strWaveFile);
	STDMETHOD(InvokeMETAFunction)(BSTR strHTTPEquiv, BSTR strContent, LONG* iReturn);
	STDMETHOD(HideHourglass)(void);
	STDMETHOD(ShowHourglass)(void);
	STDMETHOD(SetRegistrySetting)(LONG lKey, LONG lType, BSTR sSubKey, BSTR sSetting, BSTR psData);
	STDMETHOD(GetRegistrySetting)(LONG lKey, BSTR sSubKey, BSTR sSetting, BSTR* psData);
	STDMETHOD(get_UUID)(BSTR* pVal);
	STDMETHOD(LaunchProcess)(BSTR strImageName, BSTR strCommandLine, LONG* iReturn);
	STDMETHOD(LaunchProcessNonBlocking)(BSTR strImageName, BSTR strCommandLine, LONG* iReturn);
	STDMETHOD(GetProcessExitCode)(LONG iProcess, LONG* iReturn);
	STDMETHOD(CloseProcess)(LONG iProcess);
	STDMETHOD(WaitProcess)(LONG iProcess, LONG iTimeout, VARIANT_BOOL* Finished);
	STDMETHOD(Log)(BSTR strComment, LONG nSeverity, VARIANT_BOOL* pOK);
	STDMETHOD(SPBLog)(BSTR strComment, BSTR strSeverity, int *iReturn);
	STDMETHOD(WriteUserSetting)(BSTR strSetting, BSTR strValue, VARIANT_BOOL* pOK);
	STDMETHOD(ReadUserSetting)(BSTR strSetting, BSTR *pstrValue);
	STDMETHOD(WriteConfigSetting)(BSTR strSetting, BSTR strValue, BSTR strApp, VARIANT_BOOL* pOK);
	STDMETHOD(ReadConfigSetting)(BSTR strSetting, BSTR strApp, BSTR *pstrValue);
	STDMETHOD(get_ConnectionStatus)(BSTR strEntry, /*[out, retval]*/ long *pVal);
	STDMETHOD(RasDisconnect)(/*[in]*/ BSTR strEntry);
	STDMETHOD(RasConnect)(/*[in]*/ BSTR strEntry, /*[in]*/ BSTR strUser, /*[in]*/ BSTR strPwd, /*[in]*/ BSTR strDomain);
	STDMETHOD(get_OEMInfo)(BSTR* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(Generic), CGeneric)
