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

#include "common/RhoFilePath.h"

//#include "ESRIMapProvider.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WM MapView"


extern "C" HWND getMainWnd();



static IMapView* ourMapView = NULL;
static CRhoMapViewDlg ourMapViewDlg;


class RhoMapViewRedrawViewCommand : public RhoNativeViewRunnable {
public:
	RhoMapViewRedrawViewCommand() {}
	virtual void run() {
		ourMapViewDlg.requestRedraw();
	}
};


class DrawingDeviceMapViewImpl : public DrawingDeviceImpl {
	virtual void requestRedraw(){
		RHO_MAP_TRACE("MapView requested redraw -> post command to UI thread");
		RhoMapViewRedrawViewCommand* command = new RhoMapViewRedrawViewCommand();
		RhoNativeViewUtil::executeInUIThread_WM(command);
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

	RHO_MAP_TRACE2("execute rho_map_create( w = %d,  h = %d )", r.right - r.left, r.bottom - r.top);
	ourMapView = rho_map_create(mParams, &ourDrawingDevice, r.right - r.left, r.bottom - r.top);
	rho_param_free(mParams);
	mParams = NULL;


	if (ourMapView != NULL) {
		int minz = ourMapView->minZoom();
		int maxz = ourMapView->maxZoom();
		RHO_MAP_TRACE2("request Zoom limits: minZoom = %d,  maxZoom = %d", minz, maxz);
		::SendMessage(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, TBM_SETRANGEMIN, FALSE, minz); 
		::SendMessage(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, TBM_SETRANGEMAX, FALSE, maxz); 
		int dwPos = ourMapView->zoom();
		dwPos = ourMapView->maxZoom() - (dwPos - ourMapView->minZoom());
		::SendMessage(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, TBM_SETPOS, TRUE, dwPos); 

		String strImagePath = "lib/res/blue_pushpin.png";
		String fullImagePath = CFilePath::join( RHODESAPP().getRhoRootPath(), strImagePath);
		IDrawingImage* pinImg = ourDrawingDevice.createImage(fullImagePath, true);
		ourMapView->setPinImage(pinImg, -10, -35);
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
			RHO_MAP_TRACE1("MapView->setZoom( %d)", dwPos);
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
		int nz = ourMapView->zoom()+1;
		RHO_MAP_TRACE1("MapView->setZoom( %d)", nz);
		ourMapView->setZoom(nz);
		int dwPos = ourMapView->zoom();
		dwPos = ourMapView->maxZoom() - (dwPos - ourMapView->minZoom());
		::SendMessage(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, TBM_SETPOS, TRUE, dwPos); 
		requestRedraw();
	}
	return 0;
}

LRESULT CRhoMapViewDlg::OnZoomOut(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	if (ourMapView != NULL	) {
		int nz = ourMapView->zoom()-1;
		RHO_MAP_TRACE1("MapView->setZoom( %d)", nz);
		ourMapView->setZoom(nz);
		int dwPos = ourMapView->zoom();
		dwPos = ourMapView->maxZoom() - (dwPos - ourMapView->minZoom());
		::SendMessage(GetDlgItem(IDC_SLIDER_ZOOM).m_hWnd, TBM_SETPOS, TRUE, dwPos); 
		requestRedraw();
	}
	return 0;
}


LRESULT CRhoMapViewDlg::OnDraw(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{	
	RHO_MAP_TRACE("WM_PAINT start");

	PAINTSTRUCT ps;
	HDC hDC;

	hDC = ::BeginPaint(m_hWnd,&ps);

	RECT rect;
	::GetClientRect(m_hWnd,&rect);

	DrawingContextImpl* context = new DrawingContextImpl(hDC, rect.right - rect.left, rect.bottom - rect.top);

	// call MapView draw !!!
	if (ourMapView != NULL) {
		RHO_MAP_TRACE("execute MapView paint");
		ourMapView->paint(context);
	}

	::EndPaint(m_hWnd,&ps);
	
	RHO_MAP_TRACE("WM_PAINT finish");
	return 0;
}

LRESULT CRhoMapViewDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if (ourMapView != NULL) {
		rho_map_destroy(ourMapView);
		ourMapView = NULL;
	}
	return 0;
}


void CRhoMapViewDlg::doOpen() {

}

void CRhoMapViewDlg::doClose() {
	EndDialog(0);
}

void CRhoMapViewDlg::requestRedraw() {
	RHO_MAP_TRACE("redraw requested !");
	InvalidateRect(NULL, FALSE);
}

LRESULT CRhoMapViewDlg::OnTouch(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	int xPos = LOWORD(lParam); 
	int yPos = HIWORD(lParam);
	mLastX = xPos;
	mLastY = yPos;
	mInitialX = xPos;
	mInitialY = yPos;
	mIsPossibleClick = true;
	return 0;
}

#define MABS(x) ((x)>=0)?(x):-(x)

LRESULT CRhoMapViewDlg::OnUntouch(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if ( (MABS(mInitialX - mLastX) < 8) && (MABS(mInitialY - mLastY) < 8) && mIsPossibleClick) {
		if (ourMapView != NULL) {
			RHO_MAP_TRACE2("MapView->handleClick( %d, %d)", mLastX, mLastY);
			if (ourMapView->handleClick(mLastX, mLastY)) {
				EndDialog(0);
			}
		}
	}	
	return 0;
}

LRESULT CRhoMapViewDlg::OnDrag(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	int xPos = LOWORD(lParam); 
	int yPos = HIWORD(lParam);
	if (ourMapView != NULL) {
		RHO_MAP_TRACE2("MapView->move( %d, %d)", mLastX-xPos, mLastY-yPos);
		ourMapView->move(mLastX-xPos, mLastY-yPos);
		requestRedraw();
	}
	mLastX = xPos;
	mLastY = yPos;
	return 0;
}




class RhoMapViewOpenViewCommand : public RhoNativeViewRunnable {
public:
	RhoMapViewOpenViewCommand() {}
	virtual void run() {
		ourMapViewDlg.DoModal(getMainWnd());
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
	RHO_MAP_TRACE("MapView create executed");
	RhoMapViewOpenViewCommand* command = new RhoMapViewOpenViewCommand();
	ourMapViewDlg.mParams = rho_param_dup(p);

	RhoNativeViewUtil::executeInUIThread_WM(command);
}

extern "C" void mapview_close() {
	RHO_MAP_TRACE("MapView close executed");
	RhoMapViewCloseViewCommand* command = new RhoMapViewCloseViewCommand();
	RhoNativeViewUtil::executeInUIThread_WM(command);
}

extern "C" VALUE mapview_state_started() {
	return (ourMapView != NULL);
}

extern "C" double mapview_state_center_lat() {
	if (ourMapView != NULL) {
		return ourMapView->latitude();
	}
	return 0;
}

extern "C" double mapview_state_center_lon() {
	if (ourMapView != NULL) {
		return ourMapView->longitude();
	}
	return 0;
}
