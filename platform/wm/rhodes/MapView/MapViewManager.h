
#pragma once

#include <windows.h>

#include "stdafx.h"

#include "Graphics.h"
#include <string>
#include "ext/rho/rhoruby.h"
#include "common/StringConverter.h"

#include "resource.h"


using namespace rho::common;
using namespace rho::common::map;
using namespace rho;
using namespace stdext;

/*
class MapAnnotation {
public:
	MapAnnotation(String _title, String _subtitle, String _adress, String _url, double _longitude, double _latitude, bool adressBased) {
		mTitle = _title;
		mSubtitle = _subtitle;
		mAdress = _adress;
		mUrl = _url;
		mLongitude = _longitude;
		mLatitude = _latitude;
		mIsAdressBased = adressBased;
	}
	String const &title() {
		return mTitle;
	}
	String const &subtitle() {
		return mSubtitle;
	}
	String const &address() {
		return mAdress;
	}
	String const &url() {
		return mUrl;
	}
	double const latitude() {
		return mLatitude;
	}
	double const longitude() {
		return mLongitude;
	}
	bool const isAdressBased() {
		return mIsAdressBased;
	}
private:
	String mTitle;
	String mSubtitle;
	String mAdress;
	String mUrl;
	double mLongitude;
	double mLatitude;
	bool mIsAdressBased;
};
*/

/*
class IMapView
{
public:
	virtual ~IMapView(){}

	virtual void setSize(int width, int height) = 0;
	virtual int width() = 0;
	virtual int height() = 0;

	virtual void setMapType(String const &type) = 0;
	virtual String const mapType() = 0;

	virtual void setZoom(int zoom) = 0;
	virtual void setZoom(double latDelta, double lonDelta) = 0;
	virtual int zoom() = 0;
	virtual int minZoom() = 0;
	virtual int maxZoom() = 0;

	virtual void moveTo(double latitude, double longitude) = 0;
	virtual void moveTo(String geoCode) = 0;
	virtual void move(int dx, int dy) = 0;
	virtual double latitude() = 0;
	virtual double longitude() = 0;

	virtual void addAnnotation(Annotation* ann) = 0;

	// Return true if this MapView need to be closed
	virtual bool handleClick(int x, int y) = 0;

	virtual void paint(IDrawingContext* context) = 0;

	virtual void setGraphicsDevice(IDrawingDevice* device) = 0;
};
*/

class CRhoMapViewDlg : public //CDialog
#if defined(OS_WINDOWS)
	CDialogImpl<CRhoMapViewDlg>
#else
	CDialogImpl <CRhoMapViewDlg>
#endif	
{
    HBRUSH m_hBrush;
    HBITMAP m_hMemBitmap;
public:
	CRhoMapViewDlg  ();
	~CRhoMapViewDlg ();

	enum { IDD = IDD_MAPVIEW };

	BEGIN_MSG_MAP(CRhoBluetoothDiscoverDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_PAINT, OnDraw)
		MESSAGE_HANDLER(WM_VSCROLL, OnSliderScroll)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_BUTTON_ZOOM_IN, OnZoomIn)
		COMMAND_ID_HANDLER(IDC_BUTTON_ZOOM_OUT, OnZoomOut)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnTouch)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnUntouch)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnDrag)
        MESSAGE_HANDLER(WM_CTLCOLORSTATIC,OnCtlColor)
	END_MSG_MAP()

public:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDraw(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSliderScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnTouch(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnUntouch(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDrag(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnZoomIn(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnZoomOut(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCtlColor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void doOpen();
	void doClose();

	void requestRedraw();

	int mLastX;
	int mLastY;
	int mInitialX;
	int mInitialY;
	bool mIsPossibleClick;
	rho_param* mParams;
};