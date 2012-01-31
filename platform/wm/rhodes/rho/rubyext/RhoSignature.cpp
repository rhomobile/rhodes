#include "StdAfx.h"

#include "RhoSignature.h"
#include "common/RhodesApp.h"
#include "common/RhoTime.h"
#include "common/RhoFilePath.h"
#include "common/app_build_capabilities.h"

//#if defined(_WIN32_WCE)
//#include <imaging.h>
//#endif

#ifdef APP_BUILD_CAPABILITY_WEBKIT_BROWSER
extern void *rho_wmimpl_createSignatureInstance(HINSTANCE hInst, HWND hWnd, RECT rcWnd, int bgColor, int penColor, int penWidth);
extern void rho_wmimpl_deleteSignature(void *inkImpl);
extern LRESULT rho_wmimpl_signatureSigProcCall(void *inkImpl, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern void rho_wmimpl_signatureClearScreenCall(void *inkImpl);
extern BOOL rho_wmimpl_saveSignature(void *inkImpl, LPCTSTR szFilePathName);
#endif

extern "C" HWND getMainWnd();
extern "C" HWND getWebViewWnd(int index);

namespace rho 
{
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
        m_pSigWindow->Create(getWebViewWnd(0));
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
    if ( m_hWndCommandBar )
        ::DestroyWindow(m_hWndCommandBar);

    m_hWndCommandBar = 0;

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
	    SHCreateMenuBar(&mbi);
    #elif defined( OS_PLATFORM_MOTCE )
        m_hWndCommandBar = CommandBar_Create(_AtlBaseModule.GetResourceInstance(), m_hWnd, 1);
        TBBUTTON oBtn = {0};
        oBtn.iBitmap = -1;
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

	return FALSE;
}

LRESULT CRhoSignatureWindow::OnClearCommand(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    clearImage();
    return FALSE;
}

void CRhoSignatureWindow::addNewPoint(int x, int y, bool bNewLine)
{
    m_vecPoints.addElement( CSigPoint( x, y, bNewLine ) );
}

void CRhoSignatureWindow::sendVectors()
{
	if ( !m_bDoVectors )
        return;
#if 0
	TCHAR		szTarget[MAXURL + 1];
	LPTSTR		pIndex;
	int			iTotalLen,iStrLen,iErr = 1;
	CSignature	*pObj = (CSignature *)lParam;
	P_COORDXY	pPoint = pObj->m_pCurrentVector;
		
	while(pPoint)	// we need at least 2 points for a line
	{
		wcscpy(szTarget,L"new Array(");
		iTotalLen = 10;
		pIndex = szTarget + 10;
		//set our pointer past 'new Array('  
		while(pPoint && iTotalLen < ((MAXLEN_VECTORARR / 2) - 13))
		{
			if(pPoint->bNewLine){
				wcscpy(pIndex,L"0xFFFF,0xFFFF,");
				iTotalLen	+= 14;
				pIndex		+= 14;

			}
			
			iStrLen		= wsprintf(pIndex,L"0x%X,0x%X,",pPoint->XPos,pPoint->YPos);
			iTotalLen	+= iStrLen;
			pIndex		+= iStrLen;
			
			pPoint = pPoint->pPoint;
			
		}
		
		
		if(pPoint){				// if we haven't finished outputting this batch of points
			--pIndex;			//get rid of the last seperator
			*pIndex = L')';		//add the bracket
			pIndex++;			
			*pIndex = NULL;		//finally NULL terminate
			
		}
		else{					
			*pIndex = NULL;		//we have reached the end of a batch send
			wcscat(szTarget,L"0xFFFF,0xFFFF)");//add the last point indicator
		}
		
		//navigate
		if(pObj && pObj->m_hParent && *pObj->m_tcVectorNavigateURI != NULL)
		{
            //RHO
            //TODO: SendPBNavigate
			//pObj->m_pModule->SendPBNavigate(tcVectorEventNames, pObj->m_iInstanceID, pObj->m_tcVectorNavigateURI, szTarget, NULL); 
            //RHO
			iErr = 0;
		}
	}
#endif
}

LRESULT CRhoSignatureWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //LOG(INFO) + "DOWN: " + (int)LOWORD(lParam) + "," + (int)HIWORD(lParam);

    addNewPoint( LOWORD(lParam), HIWORD(lParam), true );

    SetCapture();
    m_bCapture = true;
    m_bOutOfSignature = false;
    m_ptLast = CPoint(LOWORD(lParam), HIWORD(lParam));

    return TRUE;
}

LRESULT CRhoSignatureWindow::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //LOG(INFO) + "UP: " + (int)LOWORD(lParam) + "," + (int)HIWORD(lParam);
    ReleaseCapture();
    m_bCapture = false;
    m_ptLast = CPoint(-1, -1);
    //sendVectors();

    return TRUE;
}

LRESULT CRhoSignatureWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //LOG(INFO) + "MouseMove: " + (int)LOWORD(lParam) + "," + (int)HIWORD(lParam);

    if (!m_bCapture)
        return TRUE;

    if ( !m_bOutOfSignature && m_ptLast.x >= 0 )
    {
        int xDelta = abs(LOWORD(lParam)-m_ptLast.x);
        int yDelta = abs(HIWORD(lParam)-m_ptLast.y);

        if ( xDelta > 30 || yDelta >30 )
        {
            LOG(ERROR) + "Sporadic stroke!";
            return TRUE;
        }
    }

    m_ptLast = CPoint(LOWORD(lParam), HIWORD(lParam));
    CRect rcClient;
    GetClientRect(rcClient);
    if ( !rcClient.PtInRect( CPoint( LOWORD(lParam), HIWORD(lParam) ) ) )
    {
        m_bOutOfSignature = true;
        return TRUE;
    }

    if ( m_bOutOfSignature )
    {
        addNewPoint( LOWORD(lParam), HIWORD(lParam), true );
        m_bOutOfSignature = false;
    }else
    {

        addNewPoint( LOWORD(lParam), HIWORD(lParam), false );
    }

    drawLastStroke();

    return TRUE;
}

void CRhoSignatureWindow::drawLastStroke()
{
    CDC oDC(GetDC());

    CPen oPen;
    oPen.CreatePen( PS_SOLID, getParams().getPenWidth(), getParams().getPenColor() );
	HPEN hOldPen = oDC.SelectPen( oPen );

    int nLastPos = m_vecPoints.size()-1;
    if ( nLastPos > 0 && !m_vecPoints[nLastPos].m_bNewLine )
    {
	    oDC.MoveTo( m_vecPoints[nLastPos-1].m_xPos, m_vecPoints[nLastPos-1].m_yPos );
	    oDC.LineTo( m_vecPoints[nLastPos].m_xPos, m_vecPoints[nLastPos].m_yPos );
    }

    oDC.SelectPen( hOldPen );
}

LRESULT CRhoSignatureWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CPaintDC oPaintDC(m_hWnd);

    CRect rcClient;
    GetClientRect(rcClient);

    drawSignature( oPaintDC, rcClient );

    return TRUE;
}

void CRhoSignatureWindow::drawSignature( CDC& oDC, CRect& rcDraw )
{
    oDC.FillSolidRect(rcDraw, getParams().getBgColor() );

    CPen oPen;
    oPen.CreatePen( PS_SOLID, getParams().getPenWidth(), getParams().getPenColor()  );
	HPEN hOldPen = oDC.SelectPen( oPen );

    for( int i = 0; i < (int)m_vecPoints.size()-1; i++ )
    {
        if ( m_vecPoints[i+1].m_bNewLine )
            continue;

		oDC.MoveTo( m_vecPoints[i].m_xPos, m_vecPoints[i].m_yPos );
		oDC.LineTo( m_vecPoints[i+1].m_xPos, m_vecPoints[i+1].m_yPos );
    }

    oDC.SelectPen( hOldPen );

}

void CRhoSignatureWindow::clearImage()
{
    m_vecPoints.removeAllElements();
    InvalidateRect(NULL,TRUE);
}

LRESULT CRhoSignatureWindow::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND hwnd, BOOL& /*bHandled*/)
{
    saveImage();

    if (getParams().m_eType == CRhoSignature::esModal)
	    EndDialog(wID);

	return 0;
}

LRESULT CRhoSignatureWindow::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (getParams().m_eType == CRhoSignature::esModal)
	    EndDialog(wID);

	return 0;
}

void RGBFromColour (RGBQUAD *prgb, COLORREF colour)
{
	prgb->rgbBlue = (BYTE) ((colour & 0x00FF0000) >> 16);
	prgb->rgbGreen = (BYTE) ((colour & 0x0000FF00) >> 8);
	prgb->rgbRed = (BYTE) (colour & 0x000000FF);
	prgb->rgbReserved = 0;
}	

void CRhoSignatureWindow::saveImage() 
{
    StringW strFilePathW = convertToStringW( getParams().getFilePath() );

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
	infoHeader.biBitCount      = 1;
	infoHeader.biCompression   = BI_RGB; 

	// dibsection information 
	BITMAPINFO* pInfo = (BITMAPINFO*)(new BYTE [sizeof (BITMAPINFOHEADER) + 2 * sizeof (RGBQUAD)]); 
	pInfo->bmiHeader = infoHeader; 
    RGBFromColour( &(pInfo->bmiColors[0]), getParams().getPenColor() );
    RGBFromColour( &(pInfo->bmiColors[1]), getParams().getBgColor() );

    BYTE* memory = 0; 
    HBITMAP hBitmap = 0;
    {
	    CDC memDC = CreateCompatibleDC(NULL); 
	    hBitmap = CreateDIBSection( NULL, pInfo, DIB_RGB_COLORS, (void**)&memory, 0, 0 ); 
	    HBITMAP hBmpOld = (HBITMAP)SelectObject( memDC, hBitmap ); 
	    // Copies screen upside down (as it is already upside down) - if need normal layout, change to BitBlt function call
        //StretchBlt(memDC, 0, 0, bitmap_dx, bitmap_dy, winDC, hasBorder ? 1 : 0, bitmap_dy+(hasBorder ? 1 : 0)+nTopHeader, bitmap_dx, bitmap_dy /* * -1 */, SRCCOPY); 
        drawSignature( memDC, CRect(0,0, bitmap_dx, bitmap_dy ) );
	    SelectObject( memDC, hBmpOld );
    }

    //#if defined(_WIN32_WCE) && !defined( OS_PLATFORM_MOTCE )
    //    m_hResult = saveBitmapToFileByImageFactory( hBitmap, strFilePathW.c_str(), convertToStringW(getParams().getFileFormat()).c_str() );
    //#else
	    if ( getParams().getFileFormat().compare("bmp") != 0 )
        {
            LOG(ERROR) + "Windows support only bmp image format.";
            m_hResult = E_FAIL;
        }else
        {
            int bytes_per_line = ((bitmap_dx + 31) & (~31)) / 8;
            BITMAPFILEHEADER bfh = {0};
	        bfh.bfType = 0x4D42;
	        bfh.bfOffBits = sizeof( bfh ) + sizeof (BITMAPINFOHEADER) + 2 * sizeof (RGBQUAD);
	        bfh.bfSize = bfh.bfOffBits + (bytes_per_line * bitmap_dy);

	        //RGBFromColour( &(pInfo->bmiColors[0]), getParams().getPenColor() );
	        //RGBFromColour( &(pInfo->bmiColors[1]), getParams().getBgColor() );

	        // Write file
	        FILE *pfile = _wfopen( strFilePathW.c_str(), L"wb" );
	        if (pfile)
	        {
		        fwrite( &bfh, sizeof (BITMAPFILEHEADER), 1, pfile );
		        fwrite( &(pInfo->bmiHeader), sizeof (BITMAPINFOHEADER), 1, pfile );
		        fwrite( pInfo->bmiColors, sizeof (RGBQUAD), 2, pfile );
		        fwrite( memory, 1, bytes_per_line * bitmap_dy, pfile );
		        fclose( pfile );
	        }

            m_hResult = S_OK;
        }

    //#endif

    delete pInfo;
	DeleteObject(hBitmap);
}
/*
HRESULT CRhoSignatureWindow::saveBitmapToFileByImageFactory( HBITMAP hBitmap, LPCTSTR filename, LPCTSTR format)
{
	HRESULT res = S_OK;

#if defined(_WIN32_WCE) && !defined( OS_PLATFORM_MOTCE )
	IImagingFactory* factory=NULL;
	if (CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&factory) == S_OK) {
		UINT count;
		ImageCodecInfo* imageCodecInfo=NULL;
		if (factory->GetInstalledEncoders(&count, &imageCodecInfo) == S_OK) 
        {
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
			} else 
            {
                LOG(ERROR) + "Unsupported image format:" + format;

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
                bmData->Stride *= -1;

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
*/

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
        rcDefault = CRect(30, 100, 30 + 200, 100 + 150);
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

    m_strFileFormat = "bmp";

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
