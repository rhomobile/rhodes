#include "stdafx.h"

#include "Winuser.h"

#include "MainWindow.h"

#include "common/RhodesApp.h"
#include "common/StringConverter.h"
#include "common/rhoparams.h"
#include "rho/rubyext/GeoLocationImpl.h"
#include "ruby/ext/rho/rhoruby.h"
#include "net/NetRequestImpl.h"
#include "sync/ClientRegister.h"


#include "MapViewManager.h"
#include "common/RhoNativeViewManager.h"

//#include "ESRIMapProvider.h"


extern "C" HWND getMainWnd();



static IMapView* ourMapView = NULL;
static CRhoMapViewDlg ourMapViewDlg;


class DrawingDeviceMapViewImpl : public DrawingDeviceImpl {
	virtual void requestRedraw(){
		ourMapViewDlg.requestRedraw();
	}
};

static DrawingDeviceMapViewImpl ourDrawingDevice;


CRhoMapViewDlg::CRhoMapViewDlg ()
{
}

CRhoMapViewDlg::~CRhoMapViewDlg ()
{
}

LRESULT CRhoMapViewDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowText(_T("MapView"));

#if defined(_WIN32_WCE)

	SHINITDLGINFO shidi = { SHIDIM_FLAGS, m_hWnd, SHIDIF_SIZEDLGFULLSCREEN };
	RHO_ASSERT(SHInitDialog(&shidi));


	SHMENUBARINFO mbi = { sizeof(mbi), 0 };
	mbi.hwndParent = m_hWnd;
	mbi.nToolBarId = IDR_GETURL_MENUBAR;//IDR_MAPVIEW;
	mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
	
	SHCreateMenuBar(&mbi);
	//RHO_ASSERT(SHCreateMenuBar(&mbi));

	//::SetWindowLong(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, 
	//	GWL_EXSTYLE,
	//	::GetWindowLong(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);


	RECT r;
	::GetClientRect(m_hWnd, &r);

	ourMapView = rho_map_create(mParams, &ourDrawingDevice, r.right - r.left, r.bottom - r.top);
	rho_param_free(mParams);
	mParams = NULL;


	if (ourMapView != NULL) {
		::SendMessage(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, TBM_SETRANGEMIN, FALSE, ourMapView->minZoom()); 
		::SendMessage(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, TBM_SETRANGEMAX, FALSE, ourMapView->maxZoom()); 
		int dwPos = ourMapView->zoom();
		dwPos = ourMapView->maxZoom() - (dwPos - ourMapView->minZoom());
		::SendMessage(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, TBM_SETPOS, TRUE, dwPos); 
	}

#else 

	CreateButtons();
	GotoDlgCtrl(m_btnOk);

#endif

	requestRedraw();

	return FALSE;
}

LRESULT CRhoMapViewDlg::OnSliderScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	//if (wParam == TB_THUMBTRACK) {
		
		int dwPos = ::SendMessage(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, TBM_GETPOS, 0, 0); 
		if (ourMapView != NULL) {
			dwPos = ourMapView->maxZoom() - (dwPos - ourMapView->minZoom());
			ourMapView->setZoom(dwPos);
		}
		requestRedraw();

	//}
	return 0;
}


LRESULT CRhoMapViewDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND hwnd, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}


LRESULT CRhoMapViewDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CRhoMapViewDlg::OnZoomIn(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	if (ourMapView != NULL	) {
		ourMapView->setZoom(ourMapView->zoom()+1);
		int dwPos = ourMapView->zoom();
		dwPos = ourMapView->maxZoom() - (dwPos - ourMapView->minZoom());
		::SendMessage(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, TBM_SETPOS, TRUE, dwPos); 
		requestRedraw();
	}
	return 0;
}

LRESULT CRhoMapViewDlg::OnZoomOut(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	if (ourMapView != NULL	) {
		ourMapView->setZoom(ourMapView->zoom()-1);
		int dwPos = ourMapView->zoom();
		dwPos = ourMapView->maxZoom() - (dwPos - ourMapView->minZoom());
		::SendMessage(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, TBM_SETPOS, TRUE, dwPos); 
		requestRedraw();
	}
	return 0;
}


LRESULT CRhoMapViewDlg::OnDraw(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{	
	PAINTSTRUCT ps;
	HDC hDC;

	hDC = ::BeginPaint(m_hWnd,&ps);

	RECT rect;
	::GetClientRect(m_hWnd,&rect);

	DrawingContextImpl* context = new DrawingContextImpl(hDC, rect.right - rect.left, rect.bottom - rect.top);

	// call MapView draw !!!
	if (ourMapView != NULL) {
		ourMapView->paint(context);
	}

	::EndPaint(m_hWnd,&ps);
	
	return 0;
}

void CRhoMapViewDlg::doOpen() {

}

void CRhoMapViewDlg::doClose() {
	EndDialog(0);
}

void CRhoMapViewDlg::requestRedraw() {
	InvalidateRect(NULL, FALSE);
}




class RhoMapViewOpenViewCommand : public RhoNativeViewRunnable {
public:
	RhoMapViewOpenViewCommand() {}
	virtual void run() {


		//ourMapView = new MapViewESRI();
		//ourMapView->setGraphicsDevice(&ourDrawingDevice);

		ourMapViewDlg.DoModal(getMainWnd());

		rho_map_destroy(ourMapView);
		ourMapView = NULL;
	}

private:
};

class RhoMapViewCloseViewCommand : public RhoNativeViewRunnable {
public:
	RhoMapViewCloseViewCommand() {}
	virtual void run() {
		ourMapViewDlg.doClose();
	}
};





extern "C" void mapview_create(rho_param *p) {
	RhoMapViewOpenViewCommand* command = new RhoMapViewOpenViewCommand();
	ourMapViewDlg.mParams = rho_param_dup(p);

	RhoNativeViewUtil::executeInUIThread_WM(command);
}

extern "C" void mapview_close() {
	RhoMapViewCloseViewCommand* command = new RhoMapViewCloseViewCommand();
	RhoNativeViewUtil::executeInUIThread_WM(command);
}

extern "C" VALUE mapview_state_started() {
	//TODO: mapview_state_started
	return 0;
}

extern "C" double mapview_state_center_lat() {
	//TODO:
	return 0;
}

extern "C" double mapview_state_center_lon() {
	//TODO:
	return 0;
}
