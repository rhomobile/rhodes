// Signature.cpp: implementation of the CSignature class.
//
//////////////////////////////////////////////////////////////////////

#include "Signature.h"
#include <sstream>


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
CSignature::CSignature(HINSTANCE hInstance,HWND hParent, int iInstanceID, bool bBuiltAsRhodesExtension)
{
	_tcscpy(WcName,WIN_CLASSNAME);
	m_iInstanceID	= iInstanceID;
	m_hInstance		= hInstance;
	m_hParent		= hParent;
	m_hSigBox		= NULL;	
	m_Head			= NULL;
	m_pCurrentPos	= NULL;	
	m_pPrevPos		= NULL;
	m_lpSzFilename	= NULL;
	//set the defaults 
	
	m_iSigBoxHeight	= HEIGHT_SIG;	//height of the signature capture area
	m_iSigBoxWidth	= WIDTH_SIG;		//width of the signature capture area

//	m_iSigBoxHeight=GetSystemMetrics(SM_CYSCREEN);	
//	m_iSigBoxWidth=GetSystemMetrics(SM_CXSCREEN);

	m_iCoordX		= XCOORD;				//screen coordinates for the placement of the signature capture area
	m_iCoordY		= YCOORD;

//	m_iCoordX		= 0;				//screen coordinates for the placement of the signature capture area
//	m_iCoordY		= 0;
	m_showBorder	= WS_BORDER;
	m_BorderPadding = 0;
	m_crBgColor		= WebToColorRef(BG_COLOR);
	m_iLineWidth	= PEN_WIDTH;
	SetName			(DEFAULT_FILENAME);
	m_pCurrentVector = NULL;
	m_DoVectors		= FALSE;
	m_bSigning = FALSE;
	m_bHidden  = TRUE;
	m_crLineColor	= WebToColorRef(PEN_COLOR);
	m_bBuiltAsRhodesExtension = bBuiltAsRhodesExtension;
	m_szSignatureCaptureURI = NULL;
	m_pLastSignatureAsBytes = NULL;
	m_iSigBytesCount = 0;
	m_hwndClear = 0;
	m_hwndCapture = 0;
	m_hwndCancel = 0;
	m_pCallback = 0;

	m_bOutputHex = false;
	LPCTSTR pSettingValue = L"0";	// GD force to 0 temporarily
		/*m_pModule->GetAppSettingPtr(m_iInstanceID, L"SignatureCapture\\OutputHexValues", L"Value");*/
	if (pSettingValue)
	{
		if (pSettingValue && pSettingValue[0] != L'0')
			m_bOutputHex = true;
		else
			m_bOutputHex = false;
	}

}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
CSignature::~CSignature()
{
	DWORD err = 0;
	
	DestroyWindow(m_hSigBox);	// Will destroy any child buttons also
	UnregisterClass(WcName,m_hInstance);
	
	DeleteSigMem();
	delete m_Head;
	delete [] m_lpSzFilename;
	delete[] m_szSignatureCaptureURI;
	delete[] m_pLastSignatureAsBytes;
}

BOOL IsLandScape(){
  DEVMODE devmode;
  ZeroMemory(&devmode, sizeof(DEVMODE));
  devmode.dmSize = sizeof(DEVMODE);
  devmode.dmDisplayOrientation = DMDO_0;
  devmode.dmFields = DM_DISPLAYORIENTATION;
  ChangeDisplaySettingsEx(NULL, &devmode, NULL, CDS_TEST, NULL);

  return ((devmode.dmDisplayOrientation == DMDO_90) || (devmode.dmDisplayOrientation == DMDO_270));
}
BOOL IsPortrait(){
  DEVMODE devmode;
  ZeroMemory(&devmode, sizeof(DEVMODE));
  devmode.dmSize = sizeof(DEVMODE);
  devmode.dmDisplayOrientation = DMDO_0;
  devmode.dmFields = DM_DISPLAYORIENTATION;
  ChangeDisplaySettingsEx(NULL, &devmode, NULL, CDS_TEST, NULL);

  return ((devmode.dmDisplayOrientation == DMDO_0) || (devmode.dmDisplayOrientation == DMDO_180));
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::Initialise(bool buttons, IButtonNotification *pcallback)
{
	WNDCLASS	wc;
	int iErr;
	this->buttons=buttons;
	// Remember interface to handle callbacks
	m_pCallback = pcallback;
	
	//set the current tree position
	m_pCurrentPos	= m_Head;
	
	if(m_hSigBox)
		return TRUE;
	wc.style			= CS_PARENTDC;//CS_HREDRAW | CS_VREDRAW ;
	wc.lpfnWndProc		= (WNDPROC) SigProc; 
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
	wc.hIcon			= NULL;
	wc.hCursor			= LoadCursor(NULL,IDC_CROSS);
	wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= WcName;

	if(RegisterClass(&wc) == 0){
		iErr = GetLastError();
		if(iErr != ERROR_CLASS_ALREADY_EXISTS){
			wcscpy(m_szError,ERR_CLASS);
			return FALSE;
		}		
	}
	if(IsLandScape() && buttons){
		//m_iSigBoxHeight= 536;
		//m_iSigBoxWidth= 480;
		m_iSigBoxHeight= (int)(m_iSigBoxHeight*1.425);
		m_iSigBoxWidth= (int)(m_iSigBoxWidth*0.75);

		int mx=m_iSigBoxHeight+(int)(0.17*m_iSigBoxHeight);
		int my=m_iSigBoxWidth-(int)(0.145*m_iSigBoxWidth);

		m_hSigBox = CreateWindowEx(WS_EX_NOANIMATION,WcName,NULL,WS_POPUP | WS_VISIBLE | m_showBorder,
			m_iCoordX, m_iCoordY, mx,my , m_hParent,NULL, m_hInstance,NULL);

		if(!m_hSigBox){
			wcscpy(m_szError,ERR_MEM);
			return FALSE;
		}
	}
	//	if(IsPortrait())
	else{
		m_hSigBox = CreateWindowEx(WS_EX_NOANIMATION,WcName,NULL,WS_POPUP | WS_VISIBLE | m_showBorder,
			m_iCoordX, m_iCoordY, m_iSigBoxWidth, m_iSigBoxHeight, m_hParent,NULL, m_hInstance,NULL);

		if(!m_hSigBox){
			wcscpy(m_szError,ERR_MEM);
			return FALSE;
		}
	}

	// Create child buttons if needed
	if (buttons){
		RECT rect;
		int width = 128;
		int height = 64;
		int top, left;
		int screenwidth,screenheight;

		// Remember interface to handle callbacks
		//m_pCallback = pcallback;

		// Get the size of the text in the buttons
		HWND hwnd = CreateWindow (L"BUTTON", L"", WS_CHILD, 0, 0, 0, 0, m_hSigBox, (HMENU) 0, m_hInstance, NULL);
		HDC hdc = GetDC (hwnd);

		SIZE size;
		GetTextExtentPoint32 (hdc, L"Capture", 7, &size);

		ReleaseDC (hwnd, hdc);
		DestroyWindow (hwnd);

		DEBUGMSG (TRUE, (L"Button size %d x %d\r\n", size.cx, size.cy));

		width = (size.cx + 18);
		height = size.cy+3;

		// Position buttons near the bottom of the sig cap window
		GetClientRect (m_hSigBox, &rect);

		screenwidth=GetSystemMetrics(SM_CXSCREEN);
		screenheight=GetSystemMetrics(SM_CYSCREEN);
		if(IsLandScape())
		{
			if( rect.bottom <= screenheight )
			{
				top = rect.bottom - height - 16;
			}
			else
			{
				top = screenheight - height - 30;
			}			
		}
		else
		{
			top = rect.bottom - height - 16;
		}
		left = (int)(screenwidth*0.10);
		m_hwndClear = CreateWindow (L"BUTTON", L"Clear", WS_CHILD | WS_VISIBLE,
			left, top, width, height, m_hSigBox, (HMENU) IButtonNotification::ID_CLEAR, m_hInstance, NULL);
		
		left = (int)(screenwidth*0.40);

		m_hwndCapture=CreateWindow (L"BUTTON", L"Capture", WS_CHILD | WS_VISIBLE,
			left, top, width, height, m_hSigBox, (HMENU) IButtonNotification::ID_CAPTURE, m_hInstance, NULL);
		
		left = (int)(screenwidth*0.70);
		m_hwndCancel=CreateWindow (L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE,
			left, top, width, height, m_hSigBox, (HMENU) IButtonNotification::ID_CANCEL, m_hInstance, NULL);
		left += (width + 16);
	}
	return (BOOL) SetWindowLong(m_hSigBox,GWL_USERDATA,(long) this);
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CALLBACK CSignature::SigProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	
	P_COORDXY		pPoint;
	CSignature		*pObj;
	HDC				hdc;
	PAINTSTRUCT		ps;
	static			bool bOffScreen;

	switch(message)
	{
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			// One of the buttons has been pressed
			pObj = (CSignature *)GetWindowLong(hWnd, GWL_USERDATA);

			if (pObj->m_pCallback)
				pObj->m_pCallback->OnClick (LOWORD(wParam));
		}

		break;
	
	case WM_LBUTTONDOWN:
		//grab a pointer to the object members
		pObj = (CSignature *)GetWindowLong(hWnd, GWL_USERDATA);
		if(LOWORD(lParam) && HIWORD(lParam) ){
			pObj->m_pPrevPos = pObj->AddCoord(LOWORD(lParam),HIWORD(lParam),true);// the 'true' depicts a start line
			pObj->m_pCurrentVector = pObj->m_pCurrentPos;
		}
		pObj->m_bSigning = TRUE;
		break;
	
	case WM_LBUTTONUP:
		pObj = (CSignature *)GetWindowLong(hWnd, GWL_USERDATA);
		if (pObj->m_pCallback) {
			pObj->m_pCallback->MUP();
		}
		//  Allows signature capture to save the vectors and send them using 'VectorNavigate' 
		pObj->SendVectors();
		pObj->m_bSigning = FALSE;
		break;
		
	case WM_MOUSEOUT:
		pObj = (CSignature *)GetWindowLong(hWnd, GWL_USERDATA);
		//  Only send vectors if we are still signing otherwise we send the 
		//  last recorded vectors whenever the user clicks outside the Window.
		if (pObj->m_bSigning)
		{
			pObj->m_bSigning = FALSE;
			if (pObj->m_pCallback) {
				pObj->m_pCallback->MUP();
			}
			pObj->SendVectors();
		}
		break;

	case WM_MOUSEMOVE:
		pObj = (CSignature *)GetWindowLong(hWnd, GWL_USERDATA);
		if(pObj->m_bSigning == FALSE){ //if we went offscreen
			pObj->m_pPrevPos = pObj->AddCoord(LOWORD(lParam),HIWORD(lParam),true);// the 'true' depicts a start line
			pObj->m_bSigning = TRUE;
		}
		
		//otherwise it is a valid onscreen coordinate
		pPoint = pObj->AddCoord(LOWORD(lParam),HIWORD(lParam),pObj->m_bNewLine);
		
		//check to make sure that we have a line to draw
		if(pPoint == NULL || pObj->m_pPrevPos == NULL)
			break;
		
		if(pPoint->bNewLine)
		{
			pObj->m_pPrevPos = 	pPoint;
			break;
		}

		//Draw the line
		hdc = GetDC(hWnd);
		//select the correct coloured pen
		SelectObject(hdc,CreatePen(PS_SOLID,pObj->m_iLineWidth,pObj->m_crLineColor));
				
		MoveToEx(hdc,pObj->m_pPrevPos->XPos,pObj->m_pPrevPos->YPos,NULL);
		LineTo(hdc,LOWORD(lParam),HIWORD(lParam));
		DeleteObject(SelectObject(hdc,GetStockObject(BLACK_PEN)));
		ReleaseDC(hWnd,hdc);
		pObj->m_pPrevPos = pPoint;
		break;	
			
	case WM_PAINT:
		pObj = (CSignature *)GetWindowLong(hWnd, GWL_USERDATA);
		hdc = BeginPaint(hWnd,&ps);
		pObj->RePaintBox(hdc,TRUE,pObj->buttons);// second param paint background
		EndPaint(hWnd, &ps);
		break;
	
	case WM_DESTROY:
		pObj = (CSignature *)GetWindowLong(hWnd, GWL_USERDATA);
		pObj->DeleteSigMem();
		break;
	case WM_SETTINGCHANGE:
		 pObj = (CSignature *)GetWindowLong(hWnd, GWL_USERDATA);
		 if (IsLandScape() && pObj->buttons )
		  {
			int my1=GetSystemMetrics(SM_CYSCREEN);
			int mx1=GetSystemMetrics(SM_CXSCREEN);
			int mx2=(pObj->m_iSigBoxHeight)+(int)(0.17 * (pObj->m_iSigBoxHeight));
			int my2=(pObj->m_iSigBoxWidth)-(int)(0.145 * (pObj->m_iSigBoxWidth));
				
	        // Create child buttons if needed
			if(pObj->buttons)
			{
				RECT rect;
				int width = 128;
				int height = 64;
				int top, left;
				int screenwidth,screenheight;

				// Remember interface to handle callbacks
				//m_pCallback = pcallback;

				// Get the size of the text in the buttons
				HWND hwnd1 = CreateWindow (L"BUTTON", L"", WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU) 0, pObj->m_hInstance, NULL);
				HDC hdc = GetDC (hwnd1);

				SIZE size;
				GetTextExtentPoint32 (hdc, L"Capture", 7, &size);

				ReleaseDC (hwnd1, hdc);
				DestroyWindow (hwnd1);

				DEBUGMSG (TRUE, (L"Button size %d x %d\r\n", size.cx, size.cy));

				width = (size.cx + 18);
				height = size.cy+3;

				// Position buttons near the bottom of the sig cap window
				GetClientRect (hWnd, &rect);
				screenwidth= GetSystemMetrics(SM_CXSCREEN);
				top = rect.bottom - height - 16;
				
				left = (int)(screenwidth*0.10);
				DestroyWindow (pObj->m_hwndClear);

				pObj->m_hwndClear= CreateWindow (L"BUTTON", L"Clear", WS_CHILD | WS_VISIBLE,
					 left, top, width, height, hWnd, (HMENU) IButtonNotification::ID_CLEAR, pObj->m_hInstance, NULL);
				
				left = (int)(screenwidth*0.40);
				DestroyWindow (pObj->m_hwndCapture);

				pObj->m_hwndCapture=CreateWindow (L"BUTTON", L"Capture", WS_CHILD | WS_VISIBLE,
					left, top, width, height, hWnd, (HMENU) IButtonNotification::ID_CAPTURE,pObj->m_hInstance, NULL);
				
				left = (int)(screenwidth*0.70);

				DestroyWindow (pObj->m_hwndCancel);
			
				pObj->m_hwndCancel=CreateWindow (L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE,
					left, top, width, height, hWnd, (HMENU) IButtonNotification::ID_CANCEL, pObj->m_hInstance, NULL);
				left += (width + 16);

			}
			SetWindowPos(hWnd,NULL,pObj->m_iCoordX,pObj->m_iCoordY,mx2,my2,SWP_SHOWWINDOW);
		 }
		 else if(IsPortrait() && pObj->buttons)
		 {
			// Create child buttons if needed
			if (pObj->buttons)
			{
				RECT rect;
				int width = 128;
				int height = 64;
				int top, left;
				int screenwidth,screenheight;

				// Remember interface to handle callbacks
				//m_pCallback = pcallback;

				// Get the size of the text in the buttons
				HWND hwnd1 = CreateWindow (L"BUTTON", L"", WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU) 0, pObj->m_hInstance, NULL);
				HDC hdc = GetDC (hwnd1);

				SIZE size;
				GetTextExtentPoint32 (hdc, L"Capture", 7, &size);

				ReleaseDC (hwnd1, hdc);
				DestroyWindow (hwnd1);

				DEBUGMSG (TRUE, (L"Button size %d x %d\r\n", size.cx, size.cy));

				width = (size.cx + 18);
				height = size.cy+3;

				// Position buttons near the bottom of the sig cap window
				GetClientRect (hWnd, &rect);
				screenheight=GetSystemMetrics(SM_CYSCREEN);
				screenwidth= GetSystemMetrics(SM_CXSCREEN);
				if( rect.bottom <= screenheight ){
					top = rect.bottom - height - 16;
				}
				else{
					top = screenheight - height - 16;
				}
				
				left = (int)(screenwidth*0.10);
				DestroyWindow (pObj->m_hwndClear);


				pObj->m_hwndClear= CreateWindow (L"BUTTON", L"Clear", WS_CHILD | WS_VISIBLE,
					 left, top, width, height, hWnd, (HMENU) IButtonNotification::ID_CLEAR, pObj->m_hInstance, NULL);
				
				left = (int)(screenwidth*0.40);
				DestroyWindow (pObj->m_hwndCapture);

				pObj->m_hwndCapture=CreateWindow (L"BUTTON", L"Capture", WS_CHILD | WS_VISIBLE,
					left, top, width, height, hWnd, (HMENU) IButtonNotification::ID_CAPTURE,pObj->m_hInstance, NULL);
				
				left = (int)(screenwidth*0.70);

				DestroyWindow (pObj->m_hwndCancel);
			
				pObj->m_hwndCancel=CreateWindow (L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE,
					left, top, width, height, hWnd, (HMENU) IButtonNotification::ID_CANCEL, pObj->m_hInstance, NULL);
				left += (width + 16);
			}
			SetWindowPos(hWnd,NULL,pObj->m_iCoordX,pObj->m_iCoordY,pObj->m_iSigBoxWidth,pObj->m_iSigBoxHeight,SWP_SHOWWINDOW);
		  }
		  else{
			  wprintf(L"NOTHING THIS IS");
		  }
		  break;	
	 default:
		return DefWindowProc(hWnd,message, wParam, lParam);		
  }
  return TRUE;
}

void RGBFromColour (RGBQUAD *prgb, COLORREF colour)
{
	prgb->rgbBlue = (BYTE) ((colour & 0x00FF0000) >> 16);
	prgb->rgbGreen = (BYTE) ((colour & 0x0000FF00) >> 8);
	prgb->rgbRed = (BYTE) (colour & 0x000000FF);
	prgb->rgbReserved = 0;
}	

/**
* \author	Geoff Day (GD, XFH386)
* \date		August 2010 - Fix for colour independence, GD
*/

BOOL CSignature::CreateDIBitmap(LPTSTR szFilePathName)
{
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER *pbih;
	RGBQUAD *pcolour;
	BYTE *pbuffer, *pbits;
	HBITMAP hbitmap;
	HDC hdc;
	FILE *pfile;
	int width, height, bytes_per_line;
	RECT rect;
	P_COORDXY pPoint;

	// Get internal size of image
	width = m_iSigBoxWidth - m_BorderPadding;
	height = m_iSigBoxHeight - m_BorderPadding;
	bytes_per_line = ((width + 31) & (~31)) / 8;

	// Allocate space for BITMAPINFOHEADER plus 2 colours
	pbuffer = new BYTE [sizeof (BITMAPINFOHEADER) + 2 * sizeof (RGBQUAD)];
	memset (pbuffer, 0, sizeof (BITMAPINFOHEADER) + 2 * sizeof (RGBQUAD));

	// Fill in header
	pbih = (BITMAPINFOHEADER*) pbuffer;
	pbih->biSize = sizeof (BITMAPINFOHEADER);
	pbih->biWidth = width;
	pbih->biHeight = height;
	pbih->biPlanes = 1;
	pbih->biBitCount = 1;

	// Fill in colour table using black and white (replaced with actual colours later)
	pcolour = (RGBQUAD*) (pbuffer + sizeof (BITMAPINFOHEADER));
	RGBFromColour (pcolour, RGB (0, 0, 0));
	RGBFromColour (pcolour + 1, RGB (255, 255, 255));

	// Create a bitmap to draw on
	hbitmap = CreateDIBSection (NULL, (BITMAPINFO*) pbuffer, DIB_RGB_COLORS, (void**) &pbits, NULL, 0);

	// Create a device context to draw into and select bitmap into it
	hdc = CreateCompatibleDC (NULL);
	SelectObject (hdc, hbitmap);

	// Fill area with black (i.e. zero bytes = background)
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	FillRect (hdc, &rect, (HBRUSH) GetStockObject (BLACK_BRUSH));

	// Draw pattern with white (i.e. foreground)
	SelectObject (hdc, CreatePen (PS_SOLID, m_iLineWidth, RGB (255, 255, 255)));

	// Drawing code by PH
	pPoint = m_Head;
	if(pPoint != NULL && pPoint->pPoint != NULL)
	{
		for(;pPoint && pPoint->pPoint;pPoint = pPoint->pPoint)// we need at least 2 points to draw any lines
		{	
			if(pPoint->pPoint->bNewLine) //ensures that the new line is drawn
				continue;
		
			MoveToEx(hdc,pPoint->XPos,pPoint->YPos,NULL);
			LineTo(hdc,pPoint->pPoint->XPos,pPoint->pPoint->YPos);
		}	
	}	

	// Clean up
	DeleteObject (SelectObject (hdc, (HPEN) GetStockObject (BLACK_PEN)));
	DeleteDC (hdc);

	// Fill in file header
	memset (&bfh, 0, sizeof bfh);
	bfh.bfType = 0x4D42;
	bfh.bfOffBits = sizeof bfh + sizeof (BITMAPINFOHEADER) + 2 * sizeof (RGBQUAD);
	bfh.bfSize = bfh.bfOffBits + (bytes_per_line * height);

	// Insert actual colours into colour table
	RGBFromColour (pcolour, m_crBgColor);
	RGBFromColour (pcolour + 1, m_crLineColor);

	// Write file
	m_iSigBytesCount = sizeof(BITMAPFILEHEADER) + 
		sizeof(BITMAPINFOHEADER) + (2 * sizeof(RGBQUAD)) + 
		(bytes_per_line * height);
	delete[] m_pLastSignatureAsBytes;
	m_pLastSignatureAsBytes = new BYTE[m_iSigBytesCount];

	//  Copy the bitmap into the byte array, this same array will be used to write 
	//  the file out to the disk (RE1 behaviour) and also for the Data URI
	//  if the user requests that.
	memcpy(m_pLastSignatureAsBytes
		, &bfh, sizeof (BITMAPFILEHEADER));
	memcpy(m_pLastSignatureAsBytes + sizeof (BITMAPFILEHEADER)
		, pbih, sizeof (BITMAPINFOHEADER));
	memcpy(m_pLastSignatureAsBytes + sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER)
		, pcolour, (2 * sizeof (RGBQUAD)));
	memcpy(m_pLastSignatureAsBytes + sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER) + sizeof (RGBQUAD) + sizeof (RGBQUAD)
		, pbits, bytes_per_line * height);

	pfile = _wfopen (szFilePathName, L"wb");
	if (pfile)
	{
		fwrite(m_pLastSignatureAsBytes, m_iSigBytesCount, 1, pfile);
		fclose (pfile);
	}

	DeleteObject (hbitmap);
	delete [] pbuffer;

	return TRUE;
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
/*BOOL CSignature::CreateDIBitmap(LPTSTR szFilePathName)
{

	if(m_hSigBox == NULL){
		return FALSE;
	}
	
	DWORD iWidth,iHeight;
	BOOL bRet = FALSE; 
	
	RGBQUAD Colors2[2];
	
	//set the only two colours 
	Colors2[0] = CRefToRGB(m_crLineColor);	//Line colour 
	Colors2[1] = CRefToRGB(m_crBgColor);	//Backgorund colour	
	

	iWidth		= m_iSigBoxWidth - m_BorderPadding;
	iHeight		= m_iSigBoxHeight - m_BorderPadding;
	//to Create a bitmap file '???.bmp', it must have this structure
	//BITMAPFILEHEADER   
	//BITMAPINFOHEADER
	//RGBQUADs 
	//Bitmapped image data 
		
	//depends on how many colours are used
	DWORD				dwBitmapInfoSize	= sizeof(BITMAPINFO)  * sizeof(RGBQUAD); 
	BYTE*				lpBuf				= new BYTE[dwBitmapInfoSize];
	if(lpBuf ==NULL)
		return FALSE;
	
	BITMAPFILEHEADER	hdr;
	BITMAPINFOHEADER&	bih					= *(BITMAPINFOHEADER*)lpBuf;
	BITMAPINFO*			lpbi				= (BITMAPINFO*)lpBuf; 
	int					iBytesPerLine		= ((iWidth+ 31) & (~31)) / 8; 
	
	DWORD				dwFileHeaderSize	= dwBitmapInfoSize + sizeof(BITMAPFILEHEADER); 
	
	HBITMAP           hBmp; 
	VOID              *ppvBits; 
	
	//Build the BITMAPINFO 
	memset(&bih, 0, sizeof(BITMAPINFOHEADER)); 
	bih.biSize					= sizeof(BITMAPINFOHEADER); 
	bih.biWidth					= iWidth; 
	bih.biHeight				= iHeight; 
	bih.biPlanes				= 1; 
	bih.biBitCount				= 1; 
	bih.biCompression			= BI_RGB; 
	bih.biSizeImage				= iBytesPerLine * iHeight; 
	bih.biClrUsed				= 2;

	RGBQUAD* pColors			= NULL; 
	
	pColors = &Colors2[0];
	for (int i = 0; i < 2; i++) 
			memcpy(&lpbi->bmiColors[i], &pColors[i], sizeof(RGBQUAD)); 
	
	// Fill in the fields of the file header 
	hdr.bfType					= ((WORD) ('M' << 8) | 'B'); 
	hdr.bfSize					= dwFileHeaderSize + bih.biSizeImage; 
	hdr.bfReserved1				= 0; 
	hdr.bfReserved2				= 0; 
	hdr.bfOffBits				= dwFileHeaderSize; 
	
	
	HDC hDCScreen				= GetDC(m_hSigBox); 
	RePaintBox(hDCScreen,FALSE);
	hBmp = CreateDIBSection(hDCScreen, lpbi, DIB_RGB_COLORS, &ppvBits, NULL, 0); 
	//create a device context in memory; same dimensions and format as the signature capture box
	HDC hMemDC = CreateCompatibleDC(hDCScreen); 
	if (!hMemDC)
	{
		DeleteObject(hBmp); 
		ReleaseDC(NULL,hDCScreen); 
		delete [] lpBuf;
		return FALSE; 
	}
	//select our BMP into the memory device context
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp); 
	
	//draw on the memory context that now uses our bitmap 
	RePaintBox(hMemDC,FALSE);//change the screen to Black & White
	bRet = BitBlt( hDCScreen, 0,0, iWidth, iHeight,hMemDC, 0,0, SRCCOPY); 
	if(!bRet)
	{	//couldn't create the DIB section
			DeleteObject(hBmp); 
			ReleaseDC(NULL,hDCScreen); 
			delete [] lpBuf;
			return FALSE; 
	}
	
	ReleaseDC(NULL,hDCScreen);   
	//reselect the old bmp(memory) into the memory device context	
	hBmp = (HBITMAP)SelectObject(hMemDC, hOldBmp); 
	DeleteDC(hMemDC); 
	 
	HANDLE hFile = CreateFile(szFilePathName, 
                   GENERIC_WRITE, 
                   FILE_SHARE_READ, 
                   NULL, 
                   CREATE_ALWAYS, 
                   FILE_ATTRIBUTE_NORMAL, 
                   NULL); 
    if (hFile != INVALID_HANDLE_VALUE) 
    { 
		DWORD dwNumBytes; 
        WriteFile(hFile, &hdr, sizeof(hdr), &dwNumBytes, NULL);			// Write the file header 
        WriteFile(hFile, lpbi, dwBitmapInfoSize, &dwNumBytes, NULL);	// Write the DIB header 
        WriteFile(hFile, ppvBits, bih.biSizeImage, &dwNumBytes, NULL);  // Write DIB bits 
        CloseHandle(hFile); 
    } 
   
	delete [] lpBuf; 
	DeleteObject(hBmp); 
	return bRet;
}*/

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::CreateDIBitmap()
{
	return CreateDIBitmap(m_lpSzFilename);
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
void CSignature::DrawRect(HWND hWnd, HDC hDc, RECT rt, COLORREF crColor, BOOL bFilled) {
	HRGN hRgn;
	HBRUSH hBrush;
		
	// Create a rectangular region.
	hRgn = CreateRectRgn (rt.left, rt.top, rt.right, rt.bottom);
		
	if (!bFilled)
		hBrush = CreateSolidBrush (RGB(255,255,255)); 
	else 
		hBrush = CreateSolidBrush (crColor); 

	// Fill the region out with the created brush.
	FillRgn (hDc, hRgn, hBrush);
	// Delete the rectangular region. 
	DeleteObject (hRgn);
	// Delete the brush object and free all resources associated with it.
	DeleteObject (hBrush);
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::Show()
{
	
	m_bHidden  = FALSE;
	if(m_hSigBox){
		ShowWindow(m_hSigBox,SW_SHOW);
		SetForegroundWindow(m_hSigBox);
		return TRUE;
	}
	return FALSE;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::Hide()
{
	m_bHidden  = TRUE;
	return ShowWindow(m_hSigBox,SW_HIDE);
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
P_COORDXY CSignature::AddCoord(int xCoord, int yCoord,bool bNewLine)
{
	
	if(!m_Head)
	{
		m_Head				= new Point;
		if(!m_Head)
		{
			wcscpy(m_szError,ERR_MEM);
			return NULL;

		}
		m_pCurrentPos = m_pCurrentVector	= m_Head;
		m_pCurrentPos->bNewLine = bNewLine;
		m_pCurrentPos->XPos		= xCoord;
		m_pCurrentPos->YPos		= yCoord;
		return	m_pCurrentPos;
	}
	
	m_pCurrentPos->pPoint = new Point;
	if(!m_pCurrentPos->pPoint )
	{
		wcscpy(m_szError,ERR_MEM);
		return NULL;	

	}
	m_pCurrentPos->pPoint->pPoint = NULL;
	m_pCurrentPos = m_pCurrentPos->pPoint;
	
	m_pCurrentPos->bNewLine = bNewLine;
	m_pCurrentPos->XPos = xCoord;
	m_pCurrentPos->YPos = yCoord;
	
	//Mustn't forget to reset m_bNewLine
	m_bNewLine = 0;
	return m_pCurrentPos;
}	


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::ClearScreen()
{
	if(m_hSigBox == NULL)
		return FALSE;
	DeleteSigMem();
	if(InvalidateRect(m_hSigBox,NULL,TRUE)){
		if(UpdateWindow(m_hSigBox))
			return TRUE;
	}
	return FALSE;
	//SendMessage(m_hSigBox,WM_ERASEBKGROUND,0,0);
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
void CSignature::DoVectors(BOOL bDoVectors)
{
	m_DoVectors = bDoVectors;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2008 - Initial Creation, PH
*/
void CSignature::SendVectors()
{
	//if(m_DoVectors)
	//	sendVectorsThread(this);
}

rho::Vector<rho::String> CSignature::ReturnVectors()
{
	P_COORDXY pPoint = m_pCurrentVector;
	rho::Vector<rho::String> arPoints;
	std::stringstream stream;
	//stream.setf(std::ios::basefield);
	//stream.setf(std::ios::showbase);

	while (pPoint)
	{
		{
			stream.str(""); stream.clear();
			//stream << std::hex << pPoint->XPos;
			stream << pPoint->XPos;

			arPoints.addElement(stream.str());

			stream.str(""); stream.clear();
			//stream << std::hex << pPoint->YPos;
			stream << pPoint->YPos;
			arPoints.addElement(stream.str());

		}
		pPoint = pPoint->pPoint;
	}
	
	arPoints.addElement("0xFFFF");
	arPoints.addElement("0xFFFF");

	return arPoints;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2008 - Initial Creation, PH
*/
DWORD WINAPI CSignature::sendVectorsThread(LPVOID lParam)
{
	
	TCHAR		szTarget[MAX_URL + 1];
	LPTSTR		pIndex;
	int			iTotalLen,iStrLen,iErr = 1;
	CSignature	*pObj = (CSignature *)lParam;
	P_COORDXY	pPoint = pObj->m_pCurrentVector;
	
	// For new version we don't need to worry about a string object, we can send the m_pCurrentVector through.
	// And process that directly.

	while(pPoint)	// we need at least 2 points for a line
	{
		wcscpy(szTarget,L"new Array(");
		iTotalLen = 10;
		pIndex = szTarget + 10;
		//set our pointer past 'new Array('  
		while(pPoint && iTotalLen < ((MAXLEN_VECTORARR / 2) - 13))
		{
			if(pPoint->bNewLine)
			{
				if (pObj->m_bOutputHex)
				{
					wcscpy(pIndex,L"0xFFFF,0xFFFF,");
					iTotalLen	+= 14;
					pIndex		+= 14;
				}
				else
				{
					wcscpy(pIndex,L"65535,65535,");
					iTotalLen	+= 12;
					pIndex		+= 12;
				}
			}

			if (pObj->m_bOutputHex)
				iStrLen		= wsprintf(pIndex,L"0x%X,0x%X,",pPoint->XPos,pPoint->YPos);
			else
				iStrLen		= wsprintf(pIndex,L"%u,%u,",pPoint->XPos,pPoint->YPos);
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
			if (pObj->m_bOutputHex)
				wcscat(szTarget,L"0xFFFF,0xFFFF)");//add the last point indicator
			else
				wcscat(szTarget,L"65535,65535)");//add the last point indicator
		}
		
		//navigate
		if(pObj && pObj->m_hParent && *pObj->m_tcVectorNavigateURI != NULL)
		{
			//pObj->m_pModule->SendPBNavigate(tcVectorEventNames, pObj->m_iInstanceID, pObj->m_tcVectorNavigateURI, szTarget, NULL); 
			iErr = 0;
		}
	}
	return iErr;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
void CSignature:: DeleteSigMem()
{ 
	
	P_COORDXY temp = m_Head;
	m_pCurrentPos = m_Head;

	while(temp)
	{
		m_pCurrentPos = m_pCurrentPos->pPoint;
		delete temp;
		temp=m_pCurrentPos;
	}
	m_pCurrentVector = m_Head = NULL;
	m_pCurrentPos	= NULL;

	//then update the screen
	InvalidateRect(m_hSigBox,NULL,TRUE); 	
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
COLORREF CSignature::WebToColorRef(DWORD dWebColor)
{
		
	return ((dWebColor & 0xFF0000) >> 16) | (dWebColor & 0x00FF00) | ((dWebColor & 0x0000FF) << 16);
}	


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
RGBQUAD CSignature::CRefToRGB(COLORREF cColor)
{
	RGBQUAD rgbTemp;	
	
	rgbTemp.rgbBlue		= (BYTE)((cColor & 0x00FF0000) >>16);
	rgbTemp.rgbGreen	= (BYTE)((cColor & 0x0000FF00) >> 8);
	rgbTemp.rgbRed		= (BYTE)(cColor & 0x000000FF);
	
	return rgbTemp; 

}	


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
void CSignature::RePaintBox(HDC hDc,BOOL bFill,BOOL isTakeFullScreen)
{
	P_COORDXY pPoint = m_Head;
	
	
	m_Rect.top		= 0;
	m_Rect.left		= 0;
	m_Rect.bottom	= m_iSigBoxHeight;
	if(isTakeFullScreen && IsLandScape() )
	{
		//m_Rect.right	= m_iSigBoxWidth;
		int tempWidth=m_iSigBoxHeight+(int)(0.17*m_iSigBoxHeight);
		m_Rect.right=tempWidth;
	}
	else
	m_Rect.right	= m_iSigBoxWidth;

/*****************
	Used for debugging with the release build
	  TCHAR tempOut[100];

	wsprintf(tempOut,L"Colour: %d \nFill: %d",m_crBgColor,bFill);
	MessageBox(m_hParent,tempOut,L"Box Attributes",MB_OK);
******************/

	DrawRect(m_hSigBox,hDc,m_Rect,m_crBgColor,bFill);
	
	if(bFill)
		SelectObject(hDc,CreatePen(PS_SOLID,m_iLineWidth,m_crLineColor));
	else
		SelectObject(hDc,CreatePen(PS_SOLID,m_iLineWidth,RGB(0x00,0x00,0x00)));

	if(pPoint != NULL && pPoint->pPoint != NULL)
	{
		for(;pPoint && pPoint->pPoint;pPoint = pPoint->pPoint)// we need at least 2 points to draw any lines
		{	
			if(pPoint->pPoint->bNewLine) //ensures that the new line is drawn
				continue;
		
			MoveToEx(hDc,pPoint->XPos,pPoint->YPos,NULL);
			LineTo(hDc,pPoint->pPoint->XPos,pPoint->pPoint->YPos);
		
				
		}	
	}	
	DeleteObject(SelectObject(hDc,GetStockObject(BLACK_PEN))); 
	
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::SetWidth(int iWidth)
{
	m_iSigBoxWidth	=	iWidth;
	if(!m_hSigBox)
		return TRUE;	//just setting up the values before calling Initialise
	RECT Rect; 
	DeleteSigMem();
	
	if(GetWindowRect(m_hParent,&Rect) == FALSE)
		return FALSE;

	
	if(GetWindowRect(m_hSigBox,&Rect) == FALSE)
		return FALSE;
		
	m_iSigBoxHeight	=	Rect.bottom - Rect.top;
	m_iSigBoxWidth	=	iWidth;//Rect.right	- Rect.left;
	m_iCoordX		=	Rect.left;
	m_iCoordY		=	Rect.top;

	
	if(Validate(m_iCoordX,m_iCoordY, 
		m_iSigBoxWidth,m_iSigBoxHeight) == FALSE)
		return FALSE;

	return ReDraw();

}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::SetHeight(int iHeight)
{
	m_iSigBoxHeight	=	iHeight;
	if(!m_hSigBox)
		return TRUE;	//just setting up the values before calling Initialise
	RECT Rect; 
	DeleteSigMem();		
	if(GetWindowRect(m_hParent,&Rect) == FALSE)
		return FALSE;
	
	if(GetWindowRect(m_hSigBox,&Rect) == FALSE)
		return FALSE;
		
	
	m_iSigBoxWidth	=	Rect.right	- Rect.left;
	m_iCoordX		=	Rect.left;

	if(Validate(m_iCoordX,m_iCoordY, 
		m_iSigBoxWidth,m_iSigBoxHeight) == FALSE)
		return FALSE;
	
	return ReDraw();
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::SetXpos(int iX)
{
	m_iCoordX		=	iX;
	if(!m_hSigBox)
		return TRUE;	//just setting up the values before calling Initialise
	RECT	Rect; 
	
	if(GetWindowRect(m_hParent,&Rect) == FALSE)
		return FALSE;

	int iMenuHeight = Rect.top;
	

	if(GetWindowRect(m_hSigBox,&Rect) == FALSE)
		return FALSE;
		
	m_iSigBoxHeight	=	Rect.bottom  - Rect.top;
	m_iSigBoxWidth	=	Rect.right	- Rect.left;
	
	if (!m_bBuiltAsRhodesExtension)
		m_iCoordY		=	Rect.top  - iMenuHeight;
		
	return ReDraw();
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::ReDraw()// with the latest coords
{

#ifdef _SIG_DEBUG	
	WCHAR szInfo[100];

	wsprintf(szInfo,L"Width = %d Height = %d",m_iSigBoxWidth,m_iSigBoxHeight);
	m_pModule->Log(PB_LOG_INFO, szInfo, _T(__FUNCTION__), __LINE__);
#endif
	
	return MoveWindow( 
			m_hSigBox, 
			m_iCoordX, 
			m_iCoordY, 
			m_iSigBoxWidth, 
			m_iSigBoxHeight, 
			FALSE ); 
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::SetYpos(int iY)
{
	m_iCoordY		=	iY;
	if(!m_hSigBox)
		return TRUE;	//just setting up the values before calling Initialise
	RECT Rect; 
	
	

	if(GetWindowRect(m_hSigBox,&Rect) == FALSE)
		return FALSE;

	
	
	m_iSigBoxHeight	=	Rect.bottom - Rect.top;
	m_iSigBoxWidth	=	Rect.right	- Rect.left;
	m_iCoordX		=	Rect.left;
	
	return ReDraw();
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::Validate(int iCoordX, int iCoordY, int iWidth, int iHeight)
{
	return TRUE;
	RECT rect; 
	
	if(GetWindowRect(m_hParent,&rect) == FALSE)
		return FALSE;
	
	if(iCoordX < 0 || iCoordY < 0 ||iCoordX  > rect.right || iCoordY > rect.bottom)
	{
		wcscpy(m_szError,ERR_DIM_XY);
		return FALSE;
	}
		
	if(((iHeight + iCoordY) > rect.bottom) || ((iWidth + iCoordX) > rect.right))
	{
		wcscpy(m_szError,ERR_DIM_XY);
		return FALSE;
	}
	return TRUE;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::SetLineWidth(int iWidth)
{
	if(iWidth < 1)
		return FALSE;
	DEBUGMSG (TRUE, (L"CSignature::SetLineWidth (%d) called, replacing existing (%d)\r\n", iWidth, m_iLineWidth));
	m_iLineWidth = iWidth;
	return TRUE;

}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::SetLineColor(LPCTSTR lpSzLineColor)
{
	COLORREF crTemp;
	/* this is a temporary measure to solve a colour problem that only shos in release build   */
	Sleep(10);
	if(strToCR(lpSzLineColor,&crTemp)){
		m_crLineColor = WebToColorRef(crTemp);
		DeleteSigMem();
		return TRUE;
	}
	return FALSE;
	

}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::SetBgColor(LPCTSTR lpSzColor)
{
	COLORREF crTemp;
	/* this is a temporary measure to solve a colour problem that only shows in release build   */
	Sleep(10);
	
	
	if(strToCR(lpSzColor,&crTemp)){
		m_crBgColor = WebToColorRef(crTemp);
		DeleteSigMem();
		return TRUE;
	}
	return TRUE;
	

}

static DWORD getColorFromString(LPCTSTR szColor)
{
    if ( !szColor || !*szColor )
        return RGB(0, 0, 0);

	int c = _wtoi(szColor);

	int cR = (c & 0xFF0000) >> 16;
	int cG = (c & 0xFF00) >> 8;
	int cB = (c & 0xFF);

    return RGB(cR, cG, cB);
}

/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::strToCR(LPCTSTR lpSzColor,COLORREF* pCrColor)
{
		
	TCHAR	pStr[7];
	int		iVal;
	BYTE byCol[3];
		
	if(wcslen(lpSzColor)!= 7)
    {
        *pCrColor = getColorFromString(lpSzColor);
		return TRUE;
    }

	for(int loop = 0;loop < 3;loop++)
	{
		_tcscpy(pStr,lpSzColor +(loop * 2)+ 1);
		pStr[2] = '\0';
		
		iVal = CharHexToByte(*pStr);
		byCol[loop] = (iVal * 16) + CharHexToByte(*(pStr+1));

	}
	*pCrColor = 0;	//set the COLORREF to 0
	*pCrColor = (byCol[0] << 16)|(byCol[1] << 8)|(byCol[2]);
	
	return TRUE;


}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BYTE CSignature::CharHexToByte(TCHAR chHex)
{
	TCHAR ch =  chHex;
	ch = towupper(chHex);
	TCHAR conv[] = {L"0123456789ABCDEF"};
	int byIndex;
	for(byIndex = 0;ch != conv[byIndex];byIndex++);
	return byIndex;
	
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::SetBorder(BOOL bOn)
{
	
	m_showBorder = bOn ? WS_BORDER:0;
			
	if(m_hSigBox){
		DWORD style = (DWORD) GetWindowLong(m_hSigBox, GWL_STYLE);
		BOOL bIsBorderAlreadyOn = (style & WS_BORDER);
		
		if(bOn){
			if(bIsBorderAlreadyOn){
				return TRUE;
			}
			style = style | WS_BORDER;//border on
		}
		else{
			if(!bIsBorderAlreadyOn)//if border is already off
				return TRUE;	
			style = style ^ WS_BORDER;//border off	
		}
		
		
		SetWindowLong(m_hSigBox,GWL_STYLE,(long)style);
		if(InvalidateRect(m_hSigBox,NULL,TRUE))
			return TRUE;
	}
	
	//just setting the property since the signature window handle is NULL
	return TRUE;  
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CSignature::SetName(LPCTSTR lpSzFilename)
{
	int iLen;
	// Check to see if the filename has already been set
	if (m_lpSzFilename != NULL) {
		delete [] m_lpSzFilename;
	}

	//if the name was set to L"", then the default name will be selected
	if((iLen = wcslen(lpSzFilename))== 0){
		m_lpSzFilename = new TCHAR[wcslen(DEFAULT_FILENAME)+ 5];
		if(!m_lpSzFilename) return FALSE;
		wcscpy(m_lpSzFilename,DEFAULT_FILENAME);
		wcscat(m_lpSzFilename,L".bmp");
		

	}
	else{	
		m_lpSzFilename = new TCHAR[iLen + 5];	//  we must allow for the appending of the extention(".bmp")
												//  which is done in CreateDIBitmap()
		if(m_lpSzFilename == NULL){
			//error
			return FALSE;
		}
		wcscpy(m_lpSzFilename,lpSzFilename);
		wcscat(m_lpSzFilename,L".bmp");
	}
	
	return TRUE;
}

/**
* \author	Geoff Day (GD, xfh386)
*/

LPCTSTR CSignature::GetName ()
{
	return (LPCTSTR) m_lpSzFilename;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		December 2009 - Initial Creation, DCC
*/
BOOL CSignature::SetVectorNavigationURI(LPCWSTR tcNavURI)
{
	//  store the passed navigation URI
	if (wcslen(tcNavURI) < MAX_URL)
	{
		memset(m_tcVectorNavigateURI, 0, MAX_URL);
		wcsncpy(m_tcVectorNavigateURI, tcNavURI, MAX_URL);
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		June 2012 - Initial Creation, DCC
*/
BOOL CSignature::SetSignatureCaptureURI(LPCWSTR tcNavURI)
{
	//  store the passed navigation URI
	if (!tcNavURI)
		return FALSE;
	if (m_szSignatureCaptureURI)
		delete[] m_szSignatureCaptureURI;

	m_szSignatureCaptureURI = new WCHAR[(wcslen(tcNavURI) * sizeof(WCHAR)) + 1];
	wcscpy(m_szSignatureCaptureURI, tcNavURI);
	return TRUE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		June 2012 - Initial Creation, DCC
*/
BOOL CSignature::CaptureSignature()
{
	//  Called when the user captures the signature to invoke the 
	//  SignatureCaptureEvent (if registered) containing the DataURI 
	//  signature.  The last captured signature is stored in m_pLastSignatureAsBytes
	if (m_szSignatureCaptureURI && m_szSignatureCaptureURI[0])
	{
		//  Convert the Signature to base 64, this representation is about 1/3
		//  larger than the binary input
		char* szEncodedString = new char[m_iSigBytesCount * 2];
		memset(szEncodedString, 0, m_iSigBytesCount * 2);
		EncodeToBase64(m_pLastSignatureAsBytes, m_iSigBytesCount, szEncodedString);
		//  The DataURI is longer than the base 64 representation because of the header
		int iLengthOfReturnString = strlen(szEncodedString) + 30;
		WCHAR* szSignatureAsDataURI = new WCHAR[iLengthOfReturnString];
		if (szSignatureAsDataURI)
		{
			memset(szSignatureAsDataURI, 0, iLengthOfReturnString * sizeof(WCHAR));
			wcscpy(szSignatureAsDataURI, L"data:image/bmp;base64,");
			mbstowcs(szSignatureAsDataURI + wcslen(L"data:image/bmp;base64,"),
				szEncodedString, iLengthOfReturnString);

			// GD Needs re-implementing using the Common API callback method
			/*m_pModule->SendPBNavigate(tcSignatureCaptureEventNames, m_iInstanceID, 
				m_szSignatureCaptureURI, szSignatureAsDataURI, NULL); */
		}
		delete[] szSignatureAsDataURI;
		delete[] szEncodedString;
	}
	return TRUE;
}

// GD - Modification of CaptureSignature to return data as short chars in a Rho String
void CSignature::GetDataURI (rho::String& data)
{
	//  Convert the Signature to base 64, this representation is about 1/3
	//  larger than the binary input
	char* szEncodedString = new char[m_iSigBytesCount * 2 + 30];
	memset(szEncodedString, 0, m_iSigBytesCount * 2 + 30);

	// Start with the data header
	strcpy(szEncodedString, "data:image/bmp;base64,");

	// Now append the encoded data itself
	EncodeToBase64(m_pLastSignatureAsBytes, m_iSigBytesCount, szEncodedString + strlen (szEncodedString));

	// Copy it to the caller
	data = szEncodedString;

	// Clean up
	delete[] szEncodedString;
}
