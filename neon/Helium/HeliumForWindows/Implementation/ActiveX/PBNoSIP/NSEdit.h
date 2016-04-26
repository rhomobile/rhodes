// NSEdit.h : Declaration of the CNSEdit
#pragma once


#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif

#include <atlctl.h>
#include "PBNoSIP.h"
#include "_INSEditEvents_CP.h"

#define EDIT_ID		1

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// CNSEdit
class ATL_NO_VTABLE CNSEdit :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CStockPropImpl<CNSEdit, INSEdit>,
	public IPersistStreamInitImpl<CNSEdit>,
	public IOleControlImpl<CNSEdit>,
	public IOleObjectImpl<CNSEdit>,
	public IOleInPlaceActiveObjectImpl<CNSEdit>,
	public IViewObjectExImpl<CNSEdit>,
	public IOleInPlaceObjectWindowlessImpl<CNSEdit>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CNSEdit>,
	public CProxy_INSEditEvents<CNSEdit>,
	public IPersistStorageImpl<CNSEdit>,
	public ISpecifyPropertyPagesImpl<CNSEdit>,
	public IQuickActivateImpl<CNSEdit>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CNSEdit>,
#endif
	public IProvideClassInfo2Impl<&CLSID_NSEdit, &__uuidof(_INSEditEvents), &LIBID_PBNoSIPLib>,
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	public IObjectSafetyImpl<CNSEdit, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
#endif
	public CComCoClass<CNSEdit, &CLSID_NSEdit>,
	public CComControl<CNSEdit>
{
public:
	CNSEdit()
		: strAlignment (L"LEFT")
		, m_bstrFont (L"Arial")
		, m_bstrValue (L"")
	{
		m_bWindowOnly = TRUE;

		hEdit = 0;
		nMaxLength = 0;
		bPassword = 0;
		bBorder = true;

		m_bUnderLine = 0;
		m_bItalic = 0;
		m_bBold = false;
		m_bFixed = 0;
		m_iHeight = 16;
		m_iEnd = 0;
		m_iStart = 0;


		/*if((FindWindow(L"HTMLContainer", L"Symbol PocketBrowser")==NULL)&&(!ValidLicence(NULL, NULL)))
			::MessageBox(NULL, L"This is an unlicenced component of Symbol PocketBrowser.\r\n\r\nFor licence information email pso@uk.symbol.com, or call +44 118 945 7000\r\n\r\nCopyright© 2002\r\nSymbol Technologies Ltd."
				   , L"Symbol PocketBrowser"
				   , MB_SETFOREGROUND | MB_TOPMOST | MB_ICONINFORMATION);*/
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

#ifndef _CE_DCOM
DECLARE_REGISTRY_RESOURCEID(IDR_NSEDIT)
#endif


BEGIN_COM_MAP(CNSEdit)
	COM_INTERFACE_ENTRY(INSEdit)
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
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
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

BEGIN_PROP_MAP(CNSEdit)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	PROP_ENTRY_TYPE("Text", DISPID_TEXT, CLSID_NULL, VT_BSTR)
	// Example entries
	// PROP_ENTRY_TYPE("Property Name", dispid, clsid, vtType)
	// PROP_PAGE(CLSID_StockColorPage)
	PROP_ENTRY("Value", 1, CLSID_NULL)
	PROP_ENTRY("MaxLength", 2, CLSID_NULL)
	PROP_ENTRY("Password", 3, CLSID_NULL)
	PROP_ENTRY("FontBold", 4, CLSID_NULL)
	PROP_ENTRY("Alignment", 11, CLSID_NULL)
	PROP_ENTRY("FontFixedPitch", 6, CLSID_NULL)
	PROP_ENTRY("Border", 7, CLSID_NULL)
	PROP_ENTRY("Font", 8, CLSID_NULL)
	PROP_ENTRY("FontHeight", 9, CLSID_NULL)
	PROP_ENTRY("FontUnderline", 10, CLSID_NULL)
	PROP_ENTRY("FontItalic", 5, CLSID_NULL)
	PROP_ENTRY("FontName", 12, CLSID_NULL)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CNSEdit)
	CONNECTION_POINT_ENTRY(__uuidof(_INSEditEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CNSEdit)
	CHAIN_MSG_MAP(CComControl<CNSEdit>)
	DEFAULT_REFLECTION_HANDLER()
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	COMMAND_ID_HANDLER(EDIT_ID, HandleCommand)
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
			&IID_INSEdit,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// INSEdit
public:
	STDMETHOD(get_SelEndPos)(/*[out, retval]*/ int *pVal);
	STDMETHOD(get_SelStartPos)(/*[out, retval]*/ int *pVal);
	STDMETHOD(SetSelection)(/*[in]*/ int iStart, /*[in]*/ int iEnd);
	STDMETHOD(put_FontFixedPitch)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(put_FontName)(/*[in]*/ BSTR newVal);
	STDMETHOD(put_Font)(/*[in]*/ BSTR *newVal);
	STDMETHOD(put_FontItalic)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(put_FontUnderline)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(put_FontBold)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_FontBold)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_FontHeight)(/*[in]*/ int newVal);

	STDMETHOD(put_Border)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(ShowSIP)(VARIANT_BOOL bShow);
	STDMETHOD(get_Alignment)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Alignment)(/*[in]*/ BSTR newVal);
	STDMETHOD(SetFocus)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Password)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Password)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_MaxLength)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_MaxLength)(/*[in]*/ int newVal);
	STDMETHOD(get_Value)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Value)(/*[in]*/ BSTR newVal);

	CComBSTR m_bstrText;
	void OnTextChanged()
	{
		ATLTRACE(_T("OnTextChanged\n"));
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	HRESULT InPlaceActivate(LONG iVerb, const RECT* prcPosRect = NULL);

private:
	HWND hEdit;
	int nMaxLength, bPassword, bBorder, m_bUnderLine, m_bItalic, m_bFixed;
	int m_bBold, m_iHeight;
	int m_iStart, m_iEnd;
	CComBSTR strAlignment;
	CComBSTR m_bstrFont;
	CComBSTR m_bstrValue;

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCommand (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	void CreateEditWindow (void);

	WNDPROC pOldWndProc;
	static LRESULT CALLBACK NewWndProc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

OBJECT_ENTRY_AUTO(__uuidof(NSEdit), CNSEdit)
