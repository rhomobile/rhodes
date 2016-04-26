// AirBEAMSmart.h : Declaration of the CAirBEAMSmart
#pragma once


#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif



#include <atlctl.h>
#include "PocketBrowser.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// CAirBEAMSmart
class ATL_NO_VTABLE CAirBEAMSmart :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IAirBEAMSmart, &IID_IAirBEAMSmart, &LIBID_PocketBrowserLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CAirBEAMSmart>,
	public IOleControlImpl<CAirBEAMSmart>,
	public IOleObjectImpl<CAirBEAMSmart>,
	public IOleInPlaceActiveObjectImpl<CAirBEAMSmart>,
	public IViewObjectExImpl<CAirBEAMSmart>,
	public IOleInPlaceObjectWindowlessImpl<CAirBEAMSmart>,
	public ISupportErrorInfo,
	public IPersistStorageImpl<CAirBEAMSmart>,
	public ISpecifyPropertyPagesImpl<CAirBEAMSmart>,
	public IQuickActivateImpl<CAirBEAMSmart>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CAirBEAMSmart>,
#endif
	public IProvideClassInfo2Impl<&CLSID_AirBEAMSmart, NULL, &LIBID_PocketBrowserLib>,
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	public IObjectSafetyImpl<CAirBEAMSmart, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
#endif
	public CComCoClass<CAirBEAMSmart, &CLSID_AirBEAMSmart>,
	public CComControl<CAirBEAMSmart>
{
public:


	CAirBEAMSmart()
	{
		m_lAutoLoadMode = 1;
		m_bRAMManagement = FALSE;
		m_iPackageCount = 0;
		m_bAutoReboot = TRUE;
		m_bstrUploadPackage = L"";
		m_bTestMode = FALSE;
		m_bstrDeletePackage = L"";
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

#ifndef _CE_DCOM
DECLARE_REGISTRY_RESOURCEID(IDR_AIRBEAMSMART)
#endif


BEGIN_COM_MAP(CAirBEAMSmart)
	COM_INTERFACE_ENTRY(IAirBEAMSmart)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
#endif
END_COM_MAP()

BEGIN_PROP_MAP(CAirBEAMSmart)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY_TYPE("Property Name", dispid, clsid, vtType)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CAirBEAMSmart)
	CHAIN_MSG_MAP(CComControl<CAirBEAMSmart>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] =
		{
			&IID_IAirBEAMSmart,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(0)

// IAirBEAMSmart
public:
		/////////////////////////////////////////////////////////////////////////
	//  Functions exported from CGeneric, see PB Help file for more 
	//  information
	////////////////////////////////////////////////////////////////////////
	STDMETHOD(get_DeletePackage)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_DeletePackage)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_UploadPackage)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_UploadPackage)(/*[in]*/ BSTR newVal);
	STDMETHOD(Run)(/*[out,retval]*/ int* iRetVal);
	STDMETHOD(get_TestMode)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_TestMode)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_AutoReboot)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_AutoReboot)(/*[in]*/ BOOL newVal);
	STDMETHOD(AddPackage)(/*[in]*/ BSTR strPackageName, /*[out,retval]*/ int* iRetVal);
	STDMETHOD(get_RAMManagement)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_RAMManagement)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_AutoLoadMode)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_AutoLoadMode)(/*[in]*/ long newVal);

		long m_lAutoLoadMode;
	BOOL m_bRAMManagement;
	CComBSTR m_bstrPackages[4];
	int m_iPackageCount;
	BOOL m_bAutoReboot;
	CComBSTR m_bstrUploadPackage;
	BOOL m_bTestMode;
	CComBSTR m_bstrDeletePackage;

		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
		RECT& rc = *(RECT*)di.prcBounds;
		// Set Clip region to the rectangle specified by di.prcBounds
		HRGN hRgnOld = NULL;
		if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
			hRgnOld = NULL;
		bool bSelectOldRgn = false;

		HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

		if (hRgnNew != NULL)
		{
			bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
		}

		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("ATL 8.0 : AirBEAMSmart");
#ifndef _WIN32_WCE
		TextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			pszText,
			lstrlen(pszText));
#else
		ExtTextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			ETO_OPAQUE,
			NULL,
			pszText,
			ATL::lstrlen(pszText),
			NULL);
#endif

		if (bSelectOldRgn)
			SelectClipRgn(di.hdcDraw, hRgnOld);

		return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
};

OBJECT_ENTRY_AUTO(__uuidof(AirBEAMSmart), CAirBEAMSmart)
