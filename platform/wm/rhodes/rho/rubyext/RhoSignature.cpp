#include "StdAfx.h"

#include "RhoSignature.h"
#include "common/RhodesApp.h"
#include "common/RhoTime.h"
#include "common/RhoFilePath.h"
#include "common/app_build_capabilities.h"

#if defined(_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
#include <msinkaut.h>
#include <msinkaut_i.c>
#endif

#if defined(_WIN32_WCE)
#include <imaging.h>
#endif

#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
#include "RhoElements\Plugins\PB_Signature_PLG\Signature.h"
#endif

extern "C" HWND getMainWnd();

namespace rho {
using namespace common;

CRhoSignatureWindow* CRhoSignature::m_pSigWindow = 0;
CRhoSignature::CRhoSignature(void)
{
}

CRhoSignature::~CRhoSignature(void)
{
}

/*static*/ void CRhoSignature::takeSignature(CParams* params)
{
    CModalParams& oSigParams = *((CModalParams*)params);
    CRhoSignatureWindow oSigWnd(params);
    int retVal = oSigWnd.DoModal(getMainWnd());
    oSigWnd.m_hWnd = 0;

    String strStatus = retVal != IDOK ? "" : ((oSigWnd.getLatestHResult() != S_OK && oSigWnd.getLatestHResult() != S_FALSE ? "Error" : ""));
    CFilePath oFilePath(oSigParams.getFilePath());

    RHODESAPP().callSignatureCallback( oSigParams.m_callback_url, oFilePath.getBaseName(), strStatus, retVal != IDOK );
}

/*static*/ void CRhoSignature::showSignature(CParams* params)
{
    CNonModalParams& oSigParams = *((CNonModalParams*)params);
    if (oSigParams.m_bVisible)
    {
        m_pSigWindow = new CRhoSignatureWindow(params);
        m_pSigWindow->Create(getMainWnd());
        m_pSigWindow->ShowWindow(SW_SHOW);

    }else
        hideSignature();
}

/*static*/ void CRhoSignature::saveSignature(CParams* params)
{
    CModalParams& oSigParams = *((CModalParams*)params);

    if (m_pSigWindow)
    {
        m_pSigWindow->saveImage();

        String strStatus = m_pSigWindow->getLatestHResult() != S_OK && m_pSigWindow->getLatestHResult() != S_FALSE ? "Error" : "";
        CFilePath oFilePath(m_pSigWindow->getParams().getFilePath());

        RHODESAPP().callSignatureCallback( oSigParams.m_callback_url, oFilePath.getBaseName(), strStatus, false );
    }else
        RHODESAPP().callSignatureCallback( oSigParams.m_callback_url, "", "Error", false );
}

/*static*/ void CRhoSignature::clearSignature(int )
{
    if (m_pSigWindow)
        m_pSigWindow->clearImage();
}

/*static*/ void CRhoSignature::hideSignature()
{
    if (m_pSigWindow)
    {
        m_pSigWindow->DestroyWindow();
        m_pSigWindow->m_hWnd = 0;
        delete m_pSigWindow;
        m_pSigWindow = 0;
    }
}

LRESULT CRhoSignatureWindow::OnDestroyDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER

    if (m_pInkImpl != NULL )
    {
        CSignature* pRESig = (CSignature*)m_pInkImpl;
        delete pRESig;
    }
#else

#if defined(_WIN32_WCE) && !defined( OS_PLATFORM_MOTCE )
	if (m_pInkImpl != NULL) 
    {
        ((IInkOverlay*)m_pInkImpl)->Release();
	}
#endif

#endif

    if ( m_hWndCommandBar )
        ::DestroyWindow(m_hWndCommandBar);

    m_hWndCommandBar = 0;

	m_pInkImpl = NULL;
	return FALSE;
}

LRESULT CRhoSignatureWindow::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    if ( getParams().hasBorder() )
        SetWindowLong(GWL_STYLE,(long)WS_BORDER);

    CRect rcWnd = getParams().getWndRect();
    MoveWindow(rcWnd);

    if (getParams().m_eType == CRhoSignature::esModal)
    {

    #if defined(_WIN32_WCE) && !defined( OS_PLATFORM_MOTCE )
	    SetWindowText(_T("Take signature"));

	    SHINITDLGINFO shidi = { SHIDIM_FLAGS, m_hWnd, SHIDIF_SIZEDLGFULLSCREEN };
	    RHO_ASSERT(SHInitDialog(&shidi));

	    SHMENUBARINFO mbi = { sizeof(mbi), 0 };
	    mbi.hwndParent = m_hWnd;
	    mbi.nToolBarId = IDR_GETURL_MENUBAR;
	    mbi.hInstRes = _AtlBaseModule.GetResourceInstance();
	    RHO_ASSERT(SHCreateMenuBar(&mbi));
    #elif defined( OS_PLATFORM_MOTCE )
        m_hWndCommandBar = CommandBar_Create(_AtlBaseModule.GetResourceInstance(), m_hWnd, 1);
        TBBUTTON oBtn = {0};
        oBtn.idCommand = IDM_CLEAR;
        oBtn.fsState = TBSTATE_ENABLED;
        oBtn.iString = (int)L"Clear";

        CommandBar_InsertButton(m_hWndCommandBar, 0, &oBtn);
        CommandBar_AddAdornments(m_hWndCommandBar, CMDBAR_OK, 0 );

        CommandBar_Show(m_hWndCommandBar, TRUE);
    #endif

    }else
    {

    }

    #ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
        CSignature* pRESig = new CSignature(_AtlBaseModule.GetResourceInstance(), getMainWnd(), 0, 0);
        pRESig->RhoInitialise(m_hWnd, rcWnd, getParams().getBgColor(), getParams().getPenColor(), getParams().getPenWidth());
        m_pInkImpl = pRESig;
    #elif defined(_WIN32_WCE)

	    HRESULT hr = S_OK;
	    HRESULT co_init_result = CoInitializeEx(NULL, 0); //COINIT_APARTMENTTHREADED
	    if ( (co_init_result == S_OK) || (co_init_result == S_FALSE)  ) 
        {
            IInkOverlay* pInkOverlay = 0;
		    hr = ::CoCreateInstance(CLSID_InkOverlay,
								    NULL,
								    CLSCTX_INPROC_SERVER,
								    IID_IInkOverlay,
								    (void **)&pInkOverlay);
		    if (pInkOverlay != NULL) 
            {
                m_pInkImpl = pInkOverlay;

                CComPtr<IInkDrawingAttributes> spIInkDrawAttrs = NULL;
                hr = pInkOverlay->get_DefaultDrawingAttributes(&spIInkDrawAttrs);
                if (SUCCEEDED(hr))
                {
                    spIInkDrawAttrs->put_Color(getParams().getPenColor());      
                    //HIMETRIC = (PIXEL * 2540) / 96
                    spIInkDrawAttrs->put_Width( ((float)getParams().getPenWidth()*2540)/96 );

                    // Set the new drawing attributes
                    pInkOverlay->putref_DefaultDrawingAttributes(spIInkDrawAttrs);
                }

                // Attach the inkoverlay object to the window and enable it to start collecting ink
			    pInkOverlay->put_hWnd((long)m_hWnd);
			    hr = pInkOverlay->put_Enabled(VARIANT_TRUE);
		    }
		    else {
			    RAWLOG_ERROR("ERROR: Can not get Ink Overlay in Signature Capture !");
		    }
	    }
	    else {
		    RAWLOG_ERROR("ERROR: Can not Signature CoInitialize !");
	    }
    #endif //!APP_BUILD_CAPABILITY_WEBKIT_BROWSER

	return FALSE;
}

LRESULT CRhoSignatureWindow::OnClearCommand(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    clearImage();
    return FALSE;
}

LRESULT CRhoSignatureWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
    CSignature* pRESig = (CSignature*)m_pInkImpl;
    return pRESig->SigProc( m_hWnd, uMsg, wParam, lParam);
#else
    bHandled = FALSE;
#endif
    return TRUE;
}

LRESULT CRhoSignatureWindow::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
    CSignature* pRESig = (CSignature*)m_pInkImpl;
    return pRESig->SigProc( m_hWnd, uMsg, wParam, lParam);
#else
    bHandled = FALSE;
#endif
    return TRUE;
}

LRESULT CRhoSignatureWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
    CSignature* pRESig = (CSignature*)m_pInkImpl;
    return pRESig->SigProc( m_hWnd, uMsg, wParam, lParam);
#else
    bHandled = FALSE;
#endif
    return TRUE;
}

LRESULT CRhoSignatureWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
    CSignature* pRESig = (CSignature*)m_pInkImpl;
    return pRESig->SigProc( m_hWnd, uMsg, wParam, lParam);
#else
    CPaintDC oPaintDC(m_hWnd);
    CRect rcClient;
    GetClientRect(rcClient);
    oPaintDC.FillSolidRect(rcClient, getParams().getBgColor());
    bHandled = TRUE;
#endif
    return TRUE;
}

void CRhoSignatureWindow::saveImage()
{
	HBITMAP hBitmap = getScreenBitmap();
    m_hResult = saveBitmapToFile( hBitmap, convertToStringW(getParams().getFilePath()).c_str(), convertToStringW(getParams().getFileFormat()).c_str() );
	DeleteObject(hBitmap);
}

void CRhoSignatureWindow::clearImage()
{
#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
	if (m_pInkImpl != NULL) 
    {
        CSignature* pRESig = (CSignature*)m_pInkImpl;
        pRESig->ClearScreen();
    }
#else

#if defined(_WIN32_WCE) && !defined( OS_PLATFORM_MOTCE )
	if (m_pInkImpl != NULL) 
    {
        CComPtr<IInkDisp> pDisp;
        HRESULT hRes = ((IInkOverlay*)m_pInkImpl)->get_Ink(&pDisp);
        if ( SUCCEEDED(hRes) && pDisp )
        {
            pDisp->DeleteStrokes();

            InvalidateRect(NULL,TRUE);
        }
	}
#endif

#endif
}

LRESULT CRhoSignatureWindow::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND hwnd, BOOL& /*bHandled*/)
{
    saveImage();

    if (m_bModal)
	    EndDialog(wID);

	return 0;
}

LRESULT CRhoSignatureWindow::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (m_bModal)
	    EndDialog(wID);

	return 0;
}

HBITMAP CRhoSignatureWindow::getScreenBitmap() 
{
	// get screen rectangle 
	CRect windowRect; 
	GetWindowRect(&windowRect); 

    int nTopHeader = 0;
    if ( m_hWndCommandBar )
    {
	    CRect rc; 
        ::GetWindowRect(m_hWndCommandBar, &rc); 
        nTopHeader = rc.Height();
    }

    boolean hasBorder = getParams().hasBorder();
    if (hasBorder)
        windowRect.DeflateRect(1,1);

    windowRect.top += nTopHeader;

	// bitmap dimensions 
	int bitmap_dx = windowRect.right - windowRect.left; 
	int bitmap_dy = windowRect.bottom - windowRect.top; 

	// create bitmap info header 
    BITMAPINFOHEADER infoHeader = {0}; 
	infoHeader.biSize          = sizeof(infoHeader); 
	infoHeader.biWidth         = bitmap_dx; 
	infoHeader.biHeight        = bitmap_dy; 
	infoHeader.biPlanes        = 1; 
	infoHeader.biBitCount      = 24;
	infoHeader.biCompression   = BI_RGB; 

	// dibsection information 
	BITMAPINFO info; 
	info.bmiHeader = infoHeader; 
	HDC winDC = GetWindowDC(); 
	HDC memDC = CreateCompatibleDC(winDC); 
	BYTE* memory = 0; 
	HBITMAP bitmap = CreateDIBSection(winDC, &info, DIB_RGB_COLORS, (void**)&memory, 0, 0); 
	HBITMAP old_selected = (HBITMAP)SelectObject(memDC, bitmap); 
	// Copies screen upside down (as it is already upside down) - if need normal layout, change to BitBlt function call
    StretchBlt(memDC, 0, 0, bitmap_dx, bitmap_dy, winDC, hasBorder ? 1 : 0, bitmap_dy+(hasBorder ? 1 : 0)+nTopHeader, bitmap_dx, bitmap_dy * -1, SRCCOPY); 
	SelectObject(memDC, old_selected);
	DeleteDC(memDC); 
	ReleaseDC(winDC); 

	return bitmap;
}

HRESULT CRhoSignatureWindow::saveBitmapToFile( HBITMAP hBitmap, LPCTSTR filename, LPCTSTR format)
{
	HRESULT res = S_OK;
#if defined(_WIN32_WCE) //&& !defined( OS_PLATFORM_MOTCE )
	IImagingFactory* factory=NULL;
	if (CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&factory) == S_OK) {
		UINT count;
		ImageCodecInfo* imageCodecInfo=NULL;
		if (factory->GetInstalledEncoders(&count, &imageCodecInfo) == S_OK) {
			// Get the particular encoder to use
			LPTSTR formatString;
			if (wcscmp(format, L"png") == 0) {
				formatString = _T("image/png");
			} else if (wcscmp(format, L"jpg") == 0) {
				formatString = _T("image/jpeg");
			} else if (wcscmp(format, L"gif") == 0) {
				formatString = _T("image/gif");
			} else if (wcscmp(format, L"bmp") == 0) {
				formatString = _T("image/bmp");
			} else {
				factory->Release();
				CoUninitialize();
				return S_FALSE;
			}
			CLSID encoderClassId;
			if (count == 0) {
				factory->Release();
				return S_FALSE;
			}
			for(int i=0; i < (int)count; i++) {
  				if (wcscmp(imageCodecInfo[i].MimeType, formatString) == 0) {
     				encoderClassId= imageCodecInfo[i].Clsid;
     				free(imageCodecInfo);
     				break;
				} else {
					continue;
				}
				factory->Release();
				return S_FALSE;
  			} 
			IImageEncoder* imageEncoder=NULL;
			if (factory->CreateImageEncoderToFile(&encoderClassId, filename, &imageEncoder) == S_OK) {
				IImageSink* imageSink = NULL;
				res = imageEncoder->GetEncodeSink(&imageSink);
				
				if (res != S_OK) {
					imageEncoder->TerminateEncoder();
					imageEncoder->Release();
					factory->Release();
					return res;
				}

				BITMAP bm;
				GetObject (hBitmap, sizeof(BITMAP), &bm);
				PixelFormatID pixelFormat;
				switch (bm.bmBitsPixel) {
					case 1: {
						pixelFormat = PixelFormat1bppIndexed;
						break;
					}
					case 4: {
						pixelFormat = PixelFormat4bppIndexed;
						break;
					}
					case 8: {
						pixelFormat = PixelFormat8bppIndexed;
						break;
					}
					case 24: {
						pixelFormat = PixelFormat24bppRGB;
						break;
					}
					default: {
						pixelFormat = PixelFormat32bppARGB;
						break;
					}
				}
						
				BitmapData* bmData = new BitmapData();
				bmData->Height = bm.bmHeight;
				bmData->Width = bm.bmWidth;
				bmData->Scan0 = bm.bmBits;
				bmData->PixelFormat = pixelFormat;

				UINT bitsPerLine = bm.bmWidth * bm.bmBitsPixel;
				UINT bitAlignment = sizeof(LONG) * 8;
				UINT bitStride = bitAlignment * (bitsPerLine / bitAlignment);	// The image buffer is always padded to LONG boundaries
				if ((bitsPerLine % bitAlignment) != 0) bitStride += bitAlignment; // Add a bit more for the leftover values
				bmData->Stride = (bitStride / 8);

				IBitmapImage* pBitmap;
				factory->CreateBitmapFromBuffer(bmData, &pBitmap);
				IImage* pImage;
				pBitmap->QueryInterface(IID_IImage, (void**)&pImage); 
				res = pImage->PushIntoSink(imageSink);
				if (res != S_OK) {
					delete bmData;
					pBitmap->Release();
					pImage->Release();
					if (imageSink != NULL) {
						imageSink->Release();
					}
					imageEncoder->TerminateEncoder();
					imageEncoder->Release();
					factory->Release();
					return res;
				}
				
				delete bmData;
				pBitmap->Release();
				pImage->Release();
				imageSink->Release();
				imageEncoder->TerminateEncoder();
				imageEncoder->Release();
			}
		}
	}
	else {
		RAWLOG_ERROR("ERROR: Can not Signature Bitmap preparing get Image Factory !");
	}
	factory->Release();
#endif

	return res;
}

static void readIntFromParam(rho_param* params, const char* szName, long& res)
{
    rho_param* p = rho_param_hash_get(params, szName);
    if ( p )
        convertFromStringA( p->v.string, res );
}

CRect CRhoSignature::CParams::getWndRect()
{
    CRect rcDefault, rcWnd(-1,-1,-1,-1);
    if (m_params)
    {
        readIntFromParam(m_params, "left", rcWnd.left);
        readIntFromParam(m_params, "top", rcWnd.top);
        readIntFromParam(m_params, "width", rcWnd.right);
        rcWnd.right += rcWnd.left > 0 ? rcWnd.left : 0;
        readIntFromParam(m_params, "height", rcWnd.bottom);
        rcWnd.bottom += rcWnd.top > 0 ? rcWnd.top : 0;
    }

    if ( m_eType == esModal )
    {
        ::GetWindowRect(getMainWnd(), rcDefault);
    }else
    {
        rcDefault = CRect(15, 60, 15 + 200, 60 + 150);
    }

    if (rcWnd.left == -1)
        rcWnd.left = rcDefault.left;
    if (rcWnd.top == -1)
        rcWnd.top = rcDefault.top;
    if (rcWnd.right == -1)
        rcWnd.right = rcDefault.right;
    if (rcWnd.bottom == -1)
        rcWnd.bottom = rcDefault.bottom;

    return rcWnd;
}

const String& CRhoSignature::CParams::getFilePath()
{
    if ( m_strFilePath.length() > 0 )
        return m_strFilePath;

    if (m_params)
    {
        rho_param* pFP = rho_param_hash_get(m_params, "filePath");
        if ( pFP )
            m_strFilePath = pFP->v.string;
    }

    if ( m_strFilePath.length() > 0 )
        return m_strFilePath;

    m_strFilePath = RHODESAPPBASE().getBlobsDirPath() + "/Image_" + CLocalTime().toString(true,true) + "." + getFileFormat();

    return m_strFilePath;
}

const String& CRhoSignature::CParams::getFileFormat()
{
    if ( m_strFileFormat.length() > 0 )
        return m_strFileFormat;

    if (m_params)
    {
        rho_param* pFF = rho_param_hash_get(m_params, "imageFormat");
        if ( pFF )
            m_strFileFormat = pFF->v.string;
    }

    if ( m_strFileFormat.length() > 0 )
        return m_strFileFormat;

    m_strFileFormat = "png";

    return m_strFileFormat;
}

static int getColorFromString(const char* szColor)
{
    if ( !szColor || !*szColor )
        return RGB(0, 0, 0);

	int c = atoi(szColor);

	int cR = (c & 0xFF0000) >> 16;
	int cG = (c & 0xFF00) >> 8;
	int cB = (c & 0xFF);

    return RGB(cR, cG, cB);
}

int CRhoSignature::CParams::getPenColor()
{
    COLORREF color = RGB(0,0,0);

    if (m_params)
    {
        rho_param* p = rho_param_hash_get(m_params, "penColor");
        if ( p )
            color = getColorFromString(p->v.string);
    }

    return color;
}

int CRhoSignature::CParams::getBgColor()
{
    COLORREF color = RGB(255,255,255);

    if (m_params)
    {
        rho_param* p = rho_param_hash_get(m_params, "bgColor");
        if ( p )
            color = getColorFromString(p->v.string);
    }

    return color;
}

int CRhoSignature::CParams::getPenWidth()
{
    long nw = 1;

    if (m_params)
        readIntFromParam(m_params, "penWidth", nw);

    return (int)nw;
}

boolean CRhoSignature::CParams::hasBorder()
{
    long nw = 0;

    if (m_params)
    {
        rho_param* p = rho_param_hash_get(m_params, "border");
        if ( p && (strcasecmp(p->v.string, "true") == 0 || strcasecmp(p->v.string, "1") == 0 ) )
            nw = 1;
    }

    return nw != 0;
}

}

extern "C"
{

using namespace rho;
using namespace common;

void rho_signature_take(char* callback_url, rho_param* p) 
{
    rho_callInUIThread(CRhoSignature::takeSignature, new CRhoSignature::CModalParams(callback_url, p));
}

void rho_signature_visible(bool b, rho_param* p)
{
    rho_callInUIThread(CRhoSignature::showSignature, new CRhoSignature::CNonModalParams(b, p));
}

void rho_signature_capture(const char* callback_url) 
{
    rho_callInUIThread(CRhoSignature::saveSignature, new CRhoSignature::CModalParams(callback_url, 0));
}

void rho_signature_clear() 
{
    rho_callInUIThread(CRhoSignature::clearSignature, 0);
}

}
