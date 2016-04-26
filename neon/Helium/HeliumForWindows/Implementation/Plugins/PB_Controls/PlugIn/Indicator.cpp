/******************************************************************************/
#include <windows.h>
#include "ControlsModule.h"
#include "Indicator.h"
#include "WLAN.h"

#include "RcmCAPI.h"

#define WRITELOG(format, ...) pControlsModule->WriteLog (_T(__FUNCTION__), __LINE__, format, __VA_ARGS__)

/// points on the battery shapesdepending on the iconPosition
sBatteryShapes CBattery::batteryShapes[4]= 
					{ 					
						//LEFT
						{
						 {{4,1},{4,6},{1,6},{1,16},{4,16},{4,21},{31,21},{31,1},{3,1}}, //outline
						 {{{3,0},{32,22}},{{0,5},{3,17}}}, //region
						 {{24,3,29,19}, //bars
						  {18,3,23,19},
						  {12,3,17,19},
						  {6,3,11,19}},
						 {{27,14},{14,4},{14,11},{5,8},{16,17},{16,10}} //chargeShape
						 },
					  //TOP
						{
						 {{1,4},{6,4},{6,1},{16,1},{16,4},{21,4},{21,31},{1,31},{1,3}}, //outline
						 {{{0,3},{22,32}},{{5,0},{17,3}}}, //region
						 {{3,24,19,29}, //bars
						  {3,18,19,23},
						  {3,12,19,17},
						  {3,6,19,11}},
						 {{7,27},{17,14},{10,14},{13,5},{4,16},{11,16}} //chargeShape
						},
						//RIGHT	
						{
						{{1,1},{28,1},{28,6},{31,6},{31,16},{28,16},{28,21},{1,21},{1,1}}, //outline
						{{{0,0},{29,22}},{{29,5},{32,17}}}, //region
						{{3,3,8,19}, // bars 
						 {9,3,14,19}, 
						 {15,3,20,19}, 
						 {21,3,26,19}}, 
						{{4,7},{17,17},{17,10},{26,13},{15,4},{15,11}} //chargeShape
						},
					 //BOTTOM
						{
						 {{1,1},{21,1},{21,28},{16,28},{16,31},{6,31},{6,28},{1,28},{1,1}}, //outline
						 {{{0,0},{22,29}},{{5,29},{17,32}}}, //region
						 {{3,3,19,8},  //bars
						  {3,9,19,14}, 
						  {3,15,19,20},  
						  {3,21,19,26}}, 
						 {{14,4},{4,17},{11,17},{8,26},{17,15},{10,15}} //chargeShape
				 		 }
					};
				
/// points on the battery shapesdepending on the iconPosition
sSignalShapes CSignal::signalShapes[4]= 
					{ 					
						//LEFT
						{
							{	//wifi box
								9,10,30,0
							},
							{	//wifi lines
								{{11,2},{13,7}},//W
								{{13,7},{15,1}},
								{{14,2},{16,7}},
								{{16,7},{19,1}},
								{{20,7},{20,4}},//I
								{{20,3},{20,2}},
								{{22,7},{22,1}},//F
								{{23,2},{26,2}},
								{{23,4},{25,4}},
								{{27,7},{27,4}},//I
								{{27,3},{27,2}}
							},
							{	//barRegions
								{{25,22},{30,16}},
								{{19,22},{24,13}},
								{{13,22},{18,11}},
								{{7,22},{12,11}},
								{{7,11},{8,5}},
								{{1,22},{6,2}}
							},
							{	//barLines
								{{25,16},{29,16},{29,21},{25,21},{25,16}},
								{{19,13},{23,13},{23,21},{19,21},{19,13}},
								{{13,11},{17,11},{17,21},{13,21},{13,11}},
								{{7,5},{7,21},{11,21},{11,5},{11,5}},
								{{1,2},{5,2},{5,21},{1,21},{1,2}}
							},
							{	{26,21,29,17},  //bars
								{20,21,23,14}, 
								{14,21,17,12},  
								{8,21,11,6},  
								{2,21,5,3}
							}
							
						},
					  //TOP
						{
							{	//wifi box
								12,9,22,30
							},
							{	//wifi lines
								{{19,11},{14,13}},//W
								{{14,13},{20,15}},
								{{19,14},{14,16}},
								{{14,16},{20,19}},
								{{14,20},{17,20}},//I
								{{18,20},{19,20}},
								{{14,22},{20,22}},//F
								{{19,23},{19,26}},
								{{17,23},{17,25}},
								{{14,27},{17,27}},//I
								{{18,27},{19,27}}
							},
							{	//barRegions
								{{0,30},{6,25}},
								{{0,24},{9,19}},
								{{0,18},{11,13}},
								{{0,7},{11,12}},
								{{11,7},{18,8}},
								{{0,6},{20,1}}
							},
							{	//barLines
								{{0,25},{5,25},{5,29},{0,29},{0,25}},
								{{0,19},{8,19},{8,23},{0,23},{0,19}},
								{{0,13},{10,13},{10,17},{0,17},{0,13}},
								{{10,11},{0,11},{0,7},{18,7},{18,7}},
								{{0,1},{19,1},{19,5},{0,5},{0,1}}
							},
							{	{1,29,5,26},  //bars
								{1,23,8,20}, 
								{1,17,10,14},  
								{1,8,17,11},  
								{1,2,19,5}
							}
						},
						//RIGHT	
						{
							{	//wifi box
								1,10,22,0
							},
							{	//wifi lines
								{{3,2},{5,7}},//W
								{{5,7},{7,1}},
								{{6,2},{8,7}},
								{{8,7},{11,1}},
								{{12,7},{12,4}},//I
								{{12,3},{12,2}},
								{{14,7},{14,1}},//F
								{{15,2},{18,2}},
								{{15,4},{17,4}},
								{{19,7},{19,4}},//I
								{{19,3},{19,2}}
							},
							{	//barRegions
								{{1,22},{6,16}},
								{{7,22},{12,13}},
								{{13,22},{18,11}},
								{{19,22},{24,11}},
								{{23,11},{24,5}},
								{{25,22},{30,2}}
							},
							{	//barLines
								{{1,21},{5,21},{5,16},{1,16},{1,21}},
								{{7,21},{11,21},{11,13},{7,13},{7,22}},
								{{13,21},{17,21},{17,11},{13,11},{13,21}},
								{{23,5},{23,21},{19,21},{19,10},{19,10}},
								{{25,21},{29,21},{29,2},{25,2},{25,22}}
							},
							{	{2,21,5,17},  //bars
								{8,21,11,14}, 
								{14,21,17,12},  
								{20,21,23,6},  
								{26,21,29,3}
							}
						},
					 //BOTTOM
						{
							{	//wifi box
								12,1,22,22
							},
							{	//wifi lines
								{{19,3},{14,5}},//W
								{{14,5},{20,7}},
								{{19,6},{14,8}},
								{{14,8},{20,11}},
								{{14,12},{17,12}},//I
								{{18,12},{19,12}},
								{{14,14},{20,14}},//F
								{{19,15},{19,18}},
								{{17,15},{17,17}},
								{{14,19},{17,19}},//I
								{{18,19},{19,19}}
							},
							{	//barRegions
								{{0,6},{6,1}},
								{{0,12},{9,7}},
								{{0,18},{11,13}},
								{{0,19},{11,24}},
								{{11,23},{18,24}},
								{{0,30},{20,25}}
							},
							{	//barLines
								{{0,1},{5,1},{5,5},{0,5},{0,1}},
								{{0,11},{8,11},{8,7},{0,7},{0,11}},
								{{0,13},{10,13},{10,17},{0,17},{0,13}},
								{{17,23},{0,23},{0,19},{11,19},{11,19}},
								{{0,25},{19,25},{19,29},{0,29},{0,25}}
							},
							{	{1,5,5,2},  //bars
								{1,11,8,8}, 
								{1,17,10,14},  
								{1,23,17,20},  
								{1,29,19,26}
							}
						}
					};

/******************************************************************************/
extern CControlsModule *pControlsModule;

/******************************************************************************/
#define INDICATOR_CLASS_NAME		L"PB_INDICATOR"

/******************************************************************************/
BOOL CIndicator::bRegistered = FALSE;

/******************************************************************************/
CIndicator::CIndicator (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CControl (instance_id, hparent)
{
	if (!bRegistered)
	{
		WNDCLASS wndclass;
		memset (&wndclass, 0, sizeof wndclass);
		wndclass.lpfnWndProc = StaticWindowProc;
		wndclass.hInstance = hinstance;
		wndclass.lpszClassName = INDICATOR_CLASS_NAME;
		RegisterClass (&wndclass);

		bRegistered = TRUE;
	}



	// Actual position and layout will be set in derived class
	nLeft = 0;
	nTop = 0;

	// Disable timer initially
	uiTimer = 0;

	nLevel = 0;

	// Create the window with zero size initially, will be resize during layout
	hWindow = CreateWindowEx (WS_EX_NOACTIVATE, INDICATOR_CLASS_NAME, NULL, WS_POPUP, 
		nLeft, nTop, 0, 0, hparent, (HMENU) -1, hinstance, NULL);

	// Add a pointer to ourselves to the window
	SetWindowLong (hWindow, GWL_USERDATA, (LONG) this);

	// Make sure it's on top of other controls
	SetWindowPos (hWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

/******************************************************************************/
CIndicator::~CIndicator ()
{
	KillTimer (hWindow, uiTimer);
	DeleteObject (hPen);
	DeleteObject (hBrush);
	DeleteObject(hEmptyBrush);
}
/******************************************************************************/
void CIndicator::drawPolyLine(HDC hdc, const int points[][2], int numPoints)
{
	SelectObject (hdc, hPen);
	if (numPoints < 1) return;

	// move position to the first point
	MoveToEx (hdc, points[0][0], points[0][1], NULL);
	
	for (int i = 1;i<numPoints;i++)
	{
		LineTo (hdc, points[i][0], points[i][1]);
	}
}

/******************************************************************************/
LRESULT CALLBACK CIndicator::StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	CIndicator *pthis;
	HDC hdc;
	PAINTSTRUCT ps;

	// Get a pointer to ourselves
	pthis = (CIndicator*) GetWindowLong (hwnd, GWL_USERDATA);

	switch (message)
	{
		case WM_CREATE:
			return 0;

		case WM_ERASEBKGND:
			return 0;

		case WM_PAINT:
			// Paint region using object
			hdc = BeginPaint (hwnd, &ps);
			pthis->Paint (hdc);
			EndPaint (hwnd, &ps);

			return 0;

		case WM_TIMER:
			pthis->Refresh ();
			return 0;
	}

	return DefWindowProc (hwnd, message, wparam, lparam);
}

/******************************************************************************/
void CIndicator::SetTimer (UINT interval)
{
	if (uiTimer)
		KillTimer (hWindow, uiTimer);

	uiTimer = ::SetTimer (hWindow, 1, interval, NULL);
}




/******************************************************************************/
BOOL CIndicator::SetLeft (int left)
{
	HRGN region;
	BOOL result;

	// Do a boundary check	
	if (left > GetSystemMetrics (SM_CXSCREEN))
	{
		return false;
	}

	// Take a copy of the window region before moving, remove it and restore it afterwards
	region = CreateRectRgn (0, 0, 0, 0);
	GetWindowRgn (hWindow, region);
	SetWindowRgn (hWindow, NULL, FALSE);
	result = CControl::SetLeft (left);
	SetWindowRgn (hWindow, region, FALSE);

	return result;
}

/******************************************************************************/
BOOL CIndicator::SetTop (int top)
{
	HRGN region;
	BOOL result;

	// Do a boundary check	
	if (top > GetSystemMetrics (SM_CYSCREEN))
	{
		return false;
	}

	// Take a copy of the window region before moving, remove it and restore it afterwards
	region = CreateRectRgn (0, 0, 0, 0);
	GetWindowRgn (hWindow, region);
	SetWindowRgn (hWindow, NULL, FALSE);
	result = CControl::SetTop (top);
	SetWindowRgn (hWindow, region, FALSE);

	return result;
}

/******************************************************************************/
BOOL CIndicator::SetColour (LPCWSTR colour)
{

	if (!StringToColour (colour, &cRgb))
		return FALSE;

	DeleteObject (hPen);
	hPen = CreatePen (PS_SOLID, nPenWidth, cRgb);

	DeleteObject (hBrush);
	hBrush = CreateSolidBrush (cRgb);

	InvalidateRect (hWindow, NULL, FALSE);
	return TRUE;
}
/******************************************************************************/
BOOL CIndicator::SetIconPosition (LPCWSTR position)
{

	WRITELOG(L"SetIconPosition for Battery/Signal has been deprecated. See help file for more info. ");
	return TRUE;
}

/******************************************************************************/
BOOL CIndicator::SetGraphDirection (LPCWSTR direction)
{
	WRITELOG(L"SetGraphPosition for Battery/Signal has been deprecated. See help file for more info. ");

	if (!wcsicmp (direction, L"left"))
		nPendingLayout = LAYOUT_LEFT;
	else if (!wcsicmp (direction, L"top"))
		nPendingLayout = LAYOUT_UP;
	else if (!wcsicmp (direction, L"right"))
		nPendingLayout = LAYOUT_RIGHT;
	else if (!wcsicmp (direction, L"bottom"))
		nPendingLayout = LAYOUT_DOWN;
	else
	{
		//invalid setting, do nothing
		return FALSE;
	}


	UsePending ();
	return TRUE;
}
/******************************************************************************/
BOOL CIndicator::SetLayout (LPCWSTR direction)
{

	if (!wcsicmp (direction, L"left"))
		nPendingLayout = LAYOUT_LEFT;
	else if (!wcsicmp (direction, L"up"))
		nPendingLayout = LAYOUT_UP;
	else if (!wcsicmp (direction, L"right"))
		nPendingLayout = LAYOUT_RIGHT;
	else if (!wcsicmp (direction, L"down"))
		nPendingLayout = LAYOUT_DOWN;
	else
	{
		//invalid setting, do nothing
		return FALSE;
	}


	UsePending ();
	return TRUE;
}

/******************************************************************************/
void CIndicator::UsePending (void)
{

	
	nLayout = nPendingLayout;

	CreateLayout ();
	InvalidateRect (hWindow, NULL, FALSE);
}

/******************************************************************************/
/******************************************************************************/
CBattery::CBattery (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CIndicator (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Battery API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Battery API for future releases", _T(__FUNCTION__), __LINE__);

	LPCWSTR pvalue;

	psNavigate = NULL;
	psSmartNavigate = NULL;
	m_pDiagnosticsNavigate = NULL;

	bUserRequest			= FALSE;
	bBatteryThreadRunning	= FALSE;
	hBatteryThread			= INVALID_HANDLE_VALUE;
	hBatteryThreadQuit		= INVALID_HANDLE_VALUE;	
	hBatteryThreadQueue		= INVALID_HANDLE_VALUE;
	hBatteryNotification	= INVALID_HANDLE_VALUE;
	
	// Create drawing resources
	nPenWidth = 2 ;

	cRgb = RGB (0, 0, 0);

	hPen = CreatePen (PS_SOLID, nPenWidth, cRgb);
	hBrush = CreateSolidBrush (cRgb);
	hEmptyBrush = CreateSolidBrush (RGB (0xff, 0xff, 0xff));

	//used to draw charging shape
	hShapePen = CreatePen (PS_SOLID, 1 , cRgb);

	nPendingIcon = nIconPosition = ICON_BOTTOM;
	nPendingGraph = nGraphDirection = GRAPH_TO_RIGHT;
	
	nPendingLayout = nLayout = LAYOUT_RIGHT;

	
	nLeft = 1;
	nTop = 1;
	MoveControl ();
	CreateLayout ();

	// Force initial refresh
	Refresh ();

	// Get BatteryRefresh from config
	pvalue = (*CControlsModule::PBCoreStruct.pGetAppSettingPtr) (nInstanceID, L"GUI\\BatteryRefresh", L"Value");
	if (pvalue)
		uInterval = (UINT) _wtoi (pvalue);
	else
		uInterval = 5000;
	
	m_pRcm = new CRcmLoader();
	if (m_pRcm && m_pRcm->LoadRcmDLL())
		m_bRcmLoaded = true;
	else
		m_bRcmLoaded = false;
		
	if (uInterval == 0)
	{		
		//Do not set the timer
	}
	else
	{
		SetTimer (uInterval);
	}
	
	m_pDiagnostics = new CDiagnostics();
	if(!m_pDiagnostics->Initialise()){
		//No DiagAPI32.dll exists
		delete m_pDiagnostics;
		m_pDiagnostics = NULL;
	}
}

/******************************************************************************/
CBattery::~CBattery ()
{
	if (psNavigate)
		free (psNavigate);

	if (psSmartNavigate)
		free (psSmartNavigate);

	DeleteObject(hShapePen);
	delete m_pRcm;
	m_pRcm = NULL;
	
	delete [] m_pDiagnosticsNavigate;
	delete m_pDiagnostics;
}


/******************************************************************************/
void CBattery::SetLevel (int level)
{
	if (nLevel == level) 
	{	//nothing to do, early return
		return;
	};
	
	nLevel=level;

	if (nLevel < 0)
		nLevel = 0;
	if (nLevel > 4)
		nLevel = 4;

	InvalidateRect (hWindow, NULL, FALSE);
}

/******************************************************************************/
void CBattery::OnBeforeNavigate (void)
{
	if (psNavigate)
	{
		free (psNavigate);
		psNavigate = NULL;
	}

	if (psSmartNavigate)
	{
		free (psSmartNavigate);
		psSmartNavigate = NULL;
	}
	
	delete [] m_pDiagnosticsNavigate;
	m_pDiagnosticsNavigate = NULL;
}

/******************************************************************************/
HRGN CBattery::GetIconRegion (void)
{
	HRGN region, temp_region;



	region = CreateRectRgn (batteryShapes[nLayout-1].region[0][0][0],
		batteryShapes[nLayout-1].region[0][0][1], 
		batteryShapes[nLayout-1].region[0][1][0], 
		batteryShapes[nLayout-1].region[0][1][1]);

	temp_region = CreateRectRgn (batteryShapes[nLayout-1].region[1][0][0], 
		batteryShapes[nLayout-1].region[1][0][1], 
		batteryShapes[nLayout-1].region[1][1][0], 
		batteryShapes[nLayout-1].region[1][1][1]);
	CombineRgn (region, region, temp_region, RGN_OR);
	DeleteObject (temp_region);

	return region;
}

/******************************************************************************/
BOOL CBattery::SetColour (LPCWSTR colour)
{

	if (!StringToColour (colour, &cRgb))
		return FALSE;

	DeleteObject (hPen);
	hPen = CreatePen (PS_SOLID, nPenWidth, cRgb);

	DeleteObject (hShapePen);
	hShapePen = CreatePen (PS_SOLID, 1, cRgb);

	DeleteObject (hBrush);
	hBrush = CreateSolidBrush (cRgb);


	InvalidateRect (hWindow, NULL, FALSE);
	return TRUE;
}
/******************************************************************************/
void CBattery::CreateLayout (void)
{


	// Remove existing region so window can be resized safely
	// Region handle will be released by Windows itself
	SetWindowRgn (hWindow, NULL, FALSE);

	// Set window size according to overall orientation ??
	nWidth = BATTERY_MAX_DIMENSION;
	nHeight = BATTERY_MAX_DIMENSION;

	MoveControl ();

	// Determine drawing parameters
	nIconX = 0; nIconY = 0;


	SetWindowRgn (hWindow, GetIconRegion (), FALSE);
}

/******************************************************************************/
void CBattery::Paint (HDC hdc)
{

	
	// Fill the icon area with white - only the defined region will get drawn on
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = BATTERY_MAX_DIMENSION;
	rect.bottom = BATTERY_MAX_DIMENSION;
	FillRect (hdc, &rect, (HBRUSH) GetStockObject (WHITE_BRUSH));
	
	// Paint the battery icon outline
	SelectObject (hdc, hPen);
	

	drawPolyLine(hdc,batteryShapes[nLayout-1].outline, 9);

	SelectObject (hdc, hShapePen);
	SelectObject (hdc, hBrush);
	// Paint the charging zigzag if necessary
	if (bIsCharging)
	{
		Polygon(hdc,batteryShapes[nLayout-1].chargeShape, 6);
	}
	else 
	{ 
		DrawGraph (hdc);
	}
}
/******************************************************************************/
void CBattery::DrawGraph (HDC hdc)
{
	int n;

	SelectObject (hdc, hPen);

	for (n = 0; n < 4; n++)
	{
		if (nLevel > n)
			FillRect (hdc, &(batteryShapes[nLayout-1].bars[n]), hBrush);
		else
		{
			FillRect (hdc, &(batteryShapes[nLayout-1].bars[n]), hEmptyBrush);
		}
	}
}
/******************************************************************************/
void CBattery::Refresh (void)
{
	SYSTEM_POWER_STATUS_EX2 status;
	WCHAR battery [8], backup [8], source[32], batteryKnown[12], backupBatteryKnown[12];

	// Get battery status, REPAINT, FIRE EVENT
	memset (&status, 0, sizeof status);
	if (!GetSystemPowerStatusEx2 (&status, sizeof status, TRUE))
		return;

	bIsCharging = (status.ACLineStatus == AC_LINE_ONLINE);

	// SetLevel expects value 0 - 4, so convert from percentage
	if (status.BatteryLifePercent == BATTERY_PERCENTAGE_UNKNOWN)
		SetLevel (0);
	else
		SetLevel ( status.BatteryLifePercent / 25);
	
	if (psNavigate && psNavigate[0])
	{
		wsprintf (battery, L"%d", status.BatteryLifePercent == BATTERY_PERCENTAGE_UNKNOWN ? 0 : status.BatteryLifePercent);
		wsprintf (backup, L"%d", status.BackupBatteryLifePercent == BATTERY_PERCENTAGE_UNKNOWN ? 0 : status.BackupBatteryLifePercent);
		if(status.BatteryLifePercent == BATTERY_PERCENTAGE_UNKNOWN)
		{
			wsprintf (batteryKnown, L"false");
		}
		else
		{
			wsprintf (batteryKnown, L"true");
		}

		if(status.BackupBatteryLifePercent == BATTERY_PERCENTAGE_UNKNOWN)
		{
			wsprintf (backupBatteryKnown, L"false");
		}
		else
		{
			wsprintf (backupBatteryKnown, L"true");
		}

		if (bUserRequest)
		{
			wsprintf (source, L"User Request");
		}
		else
		{
			wsprintf (source, L"Timed Interval");
		}

		pControlsModule->SendPBNavigate (tcBatteryEventNames, nInstanceID, psNavigate,
			bIsCharging ? L"1" : L"0", battery, backup, source, batteryKnown, backupBatteryKnown, NULL);
	}
	InvalidateRect (hWindow, NULL, FALSE);
}


/******************************************************************************/
BOOL CBattery::SetMiscValue (LPCWSTR pparameter, LPCWSTR pvalue)
{
	if (!wcsicmp (pparameter, L"batteryevent"))
	{
		if (psNavigate)
		{
			free (psNavigate);
			psNavigate = NULL;
		}

		if (pvalue)
			psNavigate = _wcsdup (pvalue);

		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"batterydiagnosticevent"))
	{
		if (m_pDiagnosticsNavigate)
		{
			delete [] m_pDiagnosticsNavigate;
			m_pDiagnosticsNavigate = NULL;
		}
		if (pvalue){
			try{
				m_pDiagnosticsNavigate = new WCHAR[wcslen(pvalue)+1];
				wcscpy(m_pDiagnosticsNavigate,pvalue);
				return TRUE;
			}
			catch(...){

			}
		}
		
	}
	else if (!wcsicmp (pparameter, L"shiftduration"))
	{
		if(m_pDiagnostics){
			m_pDiagnostics->SetTripDuration( _wtoi(pvalue));
			return TRUE;
		}
		
	}
	else if (!wcsicmp (pparameter, L"AverageCurrentConsumption"))
	{
		if(m_pDiagnostics){
			m_pDiagnostics->SetAverageCurrent( _wtoi(pvalue));
			return TRUE;
		}
		
	}
	else if (!wcsicmp (pparameter, L"smartbatteryevent"))
	{
		if (psSmartNavigate)
		{
			free (psSmartNavigate);
			psSmartNavigate = NULL;
		}

		if (pvalue)
			psSmartNavigate = _wcsdup (pvalue);

		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"getsmartbatterystatus"))
	{
		GetSmartBatteryStatus ();
		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"getbatterystatus"))
	{
		GetBatteryStatus ();
		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"getbatterydiagnostics"))
	{
		GetBatteryDiagnostics();
		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"iconposition"))
	{
		SetIconPosition (pvalue);
		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"graphposition"))
	{
		SetGraphDirection (pvalue);
		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"layout"))
	{
		SetLayout (pvalue);
		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"setSystemNotifications"))
	{
		SetNotification (pvalue);
		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"sampleInterval"))
	{
		SetInterval (pvalue);
		return TRUE;
	}
	else
		return FALSE;
}

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		April 2012 (HP: Initial Creation)
*/
void CBattery::EnableBatteryNotifications (void)
{
	MSGQUEUEOPTIONS options;
	memset (&options, 0, sizeof options);
	options.dwSize = sizeof options;
	options.cbMaxMessage = sizeof(POWER_BROADCAST) + (MAX_PATH * sizeof TCHAR);
	options.bReadAccess = TRUE;
	hBatteryThreadQueue = CreateMsgQueue (NULL, &options);

	hBatteryNotification = RequestPowerNotifications (hBatteryThreadQueue, PBT_POWERINFOCHANGE);	
}

/**
* \author	Harish Pulimi (HP, PXB743)
* \date		April 2012 (HP: Initial Creation)
*/
void CBattery::DisableBatteryNotifications (void)
{
	StopPowerNotifications (hBatteryNotification);
	hBatteryNotification = INVALID_HANDLE_VALUE;

	CloseMsgQueue (hBatteryThreadQueue);
	hBatteryThreadQueue = INVALID_HANDLE_VALUE;
}

/******************************************************************************/
void CBattery::SetNotification (LPCWSTR lpNotify)
{
	if (!wcsicmp(lpNotify, L"Enabled"))
	{
		//  Create the thread		
		if (!bBatteryThreadRunning)
		{
			EnableBatteryNotifications();

			hBatteryThreadQuit = CreateEvent (NULL, TRUE, FALSE, NULL);
			hBatteryThread = CreateThread (NULL, 0, BatteryThreadProc, this, 0, NULL);
			
			bBatteryThreadRunning = TRUE;			
		}	
		else
		{
			//Already running, log a warning message
			pControlsModule->WriteLog (	_T(__FUNCTION__), __LINE__,
				L"The system notification thread is already running");
		}
	}
	else if (!wcsicmp(lpNotify, L"Disabled"))
	{
		//  Stop the thread
		if (bBatteryThreadRunning)
		{
			SetEvent (hBatteryThreadQuit);
			WaitForSingleObject (hBatteryThread, 5000);

			DisableBatteryNotifications();

			CloseHandle (hBatteryThreadQuit);
			hBatteryThreadQuit = INVALID_HANDLE_VALUE;
			CloseHandle (hBatteryThread);
			hBatteryThread = INVALID_HANDLE_VALUE;

			bBatteryThreadRunning = FALSE;
		}
		else
		{
			//Not running, log a warning message
			pControlsModule->WriteLog (	_T(__FUNCTION__), __LINE__,
				L"The system notification thread is not running");
		}
	}
	else
	{
		//Log Error
		pControlsModule->WriteLog (	_T(__FUNCTION__), __LINE__,
				L"Unknown parameter for setSystemNotification in battery module");
	}	
}

/******************************************************************************/
void CBattery::SetInterval (LPCWSTR lpInterval)
{
	//If the timer is already set then kill the timer
	if (uiTimer)
		KillTimer (hWindow, uiTimer);

	if (lpInterval)
		uInterval = (UINT) _wtoi (lpInterval);
	else
		uInterval = 5000;

	if (uInterval == 0)
	{	
		//Do not set the timer		
	}
	else
	{
		//Set the timer with the new value
		SetTimer (uInterval);
	}
}

/******************************************************************************/
DWORD CBattery::BatteryThreadProc(LPVOID pParam)
{
	CBattery *pBattery = (CBattery *)pParam;
	pBattery->ProcessBatteryThread();

	return 0;
}

/******************************************************************************/
void CBattery::ProcessBatteryThread (void)
{
	HANDLE wait_handles [2];
	BYTE power_buffer [QUEUE_SIZE];
	POWER_BROADCAST *ppower;
	POWER_BROADCAST_POWER_INFO *pbattery;
	DWORD count, flags;
	WCHAR battery [8], backup [8], source[32], batteryKnown[12], backupBatteryKnown[12];

	wait_handles [0] = hBatteryThreadQuit;		// Signalled when thread should quit
	wait_handles [1] = hBatteryThreadQueue;		// Signalled on battery status change

	while (1)
	{
		if (WaitForMultipleObjects (2, wait_handles, FALSE, INFINITE) == WAIT_OBJECT_0)
			break;

		// Must be battery notification, get notification message
		if (!ReadMsgQueue (hBatteryThreadQueue, power_buffer, QUEUE_SIZE, &count, 0, &flags))
			continue;

		// Get power state as reported by notification		
		ppower = (POWER_BROADCAST*) power_buffer;
		pbattery = (POWER_BROADCAST_POWER_INFO*)(ppower->SystemPowerState);

		if (pbattery)
		{			
			if (psNavigate)
			{
				wsprintf (battery, L"%d", pbattery->bBatteryLifePercent == BATTERY_PERCENTAGE_UNKNOWN ? 0 : pbattery->bBatteryLifePercent);
				wsprintf (backup, L"%d", pbattery->bBackupBatteryLifePercent == BATTERY_PERCENTAGE_UNKNOWN ? 0 : pbattery->bBackupBatteryLifePercent);
				if(pbattery->bBatteryLifePercent == BATTERY_PERCENTAGE_UNKNOWN)
				{
					wsprintf (batteryKnown, L"false");
				}
				else
				{
					wsprintf (batteryKnown, L"true");
				}

				if(pbattery->bBackupBatteryLifePercent == BATTERY_PERCENTAGE_UNKNOWN)
				{
					wsprintf (backupBatteryKnown, L"false");
				}
				else
				{
					wsprintf (backupBatteryKnown, L"true");
				}

				if (pbattery->bBatteryFlag == 2)
				{
					wsprintf (source, L"Low Battery"); 					
				}
				else if (pbattery->bBatteryFlag == 4)
				{
					wsprintf (source, L"Critical Battery"); 					
				}
				else if (pbattery->bACLineStatus == 1)
				{
					bIsCharging = true;
					wsprintf (source, L"AC Connect"); 
				}
				else if (pbattery->bACLineStatus == 0)
				{
					bIsCharging = false;
					wsprintf (source, L"AC Disconnect"); 
				}
				else
					return;

				pControlsModule->SendPBNavigate (tcBatteryEventNames, nInstanceID, psNavigate,
						bIsCharging ? L"1" : L"0", battery, backup, source, batteryKnown, backupBatteryKnown, NULL);
				
			}
		}		
	}
}


BOOL CBattery::GetBatteryDiagnostics (void)
{
	DWORD dwRet = DIAG_RET_FAILURE;
	if(m_pDiagnostics && m_pDiagnosticsNavigate){
		if((dwRet = m_pDiagnostics->DoDiagnosticsCall())!= DIAG_RET_FAILURE){
			WCHAR tcHealth[10];
			WCHAR tcPercentage[10];
			WCHAR tcMinutesCapacity[10];
			WCHAR tcExpireMonths[10];
			WCHAR tcDaysSinceBatteryReplaced[10];
			WCHAR tcMinsSinceColdBoot[10];
			WCHAR tcMinsRequiredChargeTime[10];
			WCHAR tcTimElapsedChargingMins[10];
			
			WCHAR tcUndefined[]={L"Undefined"}; 
			
			
			dwRet ^ DIAG_BATT_HEALTH_PERCENT ? _ltow(m_pDiagnostics->m_diagData.dwBattHlthPer,tcHealth,10):wcscpy(tcHealth,tcUndefined);
			dwRet ^ DIAG_BATT_CAPACITY_PERCENT ? _ltow(m_pDiagnostics->m_diagData.dwBattCapInPercent,tcPercentage,10):wcscpy(tcPercentage,tcUndefined);
			dwRet ^ DIAG_BATT_CAPACITY_MINS  ? _ltow(m_pDiagnostics->m_diagData.dwBattCapInMins,tcMinutesCapacity,10):wcscpy(tcMinutesCapacity,tcUndefined);
			dwRet ^ DIAG_BATT_APPROX_REMAINING_LIFE_MONTHS ? _ltow(m_pDiagnostics->m_diagData.dwBattRemainingLifeMnths,tcExpireMonths,10):wcscpy(tcExpireMonths,tcUndefined);
			dwRet ^ DIAG_BATT_TIME_SINCE_LAST_BATT_REPLACE_DAYS ? _ltow(m_pDiagnostics->m_diagData.dwTimSincLstBattReplDays,tcDaysSinceBatteryReplaced,10):wcscpy(tcDaysSinceBatteryReplaced,tcUndefined);
			dwRet ^ DIAG_GEN_TIME_SINCE_LAST_REBOOT_MINS ? _ltow(m_pDiagnostics->m_diagData.dwTimSincLstRebootMins,tcMinsSinceColdBoot,10):wcscpy(tcMinsSinceColdBoot,tcUndefined);
			dwRet ^ DIAG_BATT_REQ_TIME_IN_CRADLE_MINS ? _ltow(m_pDiagnostics->m_diagData.dwBattReqTimeinCradleMins,tcMinsRequiredChargeTime,10):wcscpy(tcMinsRequiredChargeTime,tcUndefined);
			dwRet ^ DIAG_BATT_TIME_ELAPSED_CHARGING_MINS ? _ltow(m_pDiagnostics->m_diagData.dwTimElapsedChargingMins,tcTimElapsedChargingMins,10):wcscpy(tcTimElapsedChargingMins,tcUndefined);
			
			
			//_ltow(m_pDiagnostics->m_diagData.dwBattHlthPer,tcHealth,10);
			//_ltow(m_pDiagnostics->m_diagData.dwBattHlthPer,tcHealth,10);
			//_ltow(m_pDiagnostics->m_diagData.dwBattCapInPercent,tcPercentage,10);
			//ltow(m_pDiagnostics->m_diagData.dwBattCapInMins,tcMinutesCapacity,10);
			//_ltow(m_pDiagnostics->m_diagData.dwBattRemainingLifeMnths,tcExpireMonths,10);
			//_ltow(m_pDiagnostics->m_diagData.dwTimSincLstBattReplDays,tcDaysSinceBatteryReplaced,10);
			//_ltow(m_pDiagnostics->m_diagData.dwTimSincLstRebootMins,tcMinsSinceColdBoot,10);
			//_ltow(m_pDiagnostics->m_diagData.dwBattReqTimeinCradleMins,tcMinsRequiredChargeTime,10);
			//_ltow(m_pDiagnostics->m_diagData.dwTimElapsedChargingMins,tcTimElapsedChargingMins,10);

			
			return pControlsModule->SendPBNavigate (
					tcBatteryDiagnosticsEventNames,
					nInstanceID,
					m_pDiagnosticsNavigate,
					tcHealth,
					tcPercentage,
					tcMinutesCapacity,
					tcExpireMonths,
					tcDaysSinceBatteryReplaced,
					tcMinsSinceColdBoot,
					tcMinsRequiredChargeTime,
					tcTimElapsedChargingMins,NULL);
			
					
		}
	}
	pControlsModule->WriteLog (
			_T(__FUNCTION__), __LINE__,
			L"Unable to retrieve diagnostic battery data, does this device support battery diagnostics?");
	
	return FALSE;
}

/*
BOOL CBattery::GetBatteryDiagnostics (void)
{
	if(m_pDiagnostics && m_pDiagnosticsNavigate){
		if(m_pDiagnostics->DoDiagnosticsCall()== DIAG_RET_SUCCESS){
			WCHAR tcHealth[10];
			WCHAR tcPercentage[10];
			WCHAR tcMinutesCapacity[10];
			WCHAR tcExpireMonths[10];
			WCHAR tcDaysSinceBatteryReplaced[10];
			WCHAR tcMinsSinceColdBoot[10];
			WCHAR tcMinsRequiredChargeTime[10];
			WCHAR tcTimElapsedChargingMins[10];
			
			_ltow(m_pDiagnostics->m_diagData.dwBattHlthPer,tcHealth,10);
			_ltow(m_pDiagnostics->m_diagData.dwBattCapInPercent,tcPercentage,10);
			_ltow(m_pDiagnostics->m_diagData.dwBattCapInMins,tcMinutesCapacity,10);
			_ltow(m_pDiagnostics->m_diagData.dwBattRemainingLifeMnths,tcExpireMonths,10);
			_ltow(m_pDiagnostics->m_diagData.dwTimSincLstBattReplDays,tcDaysSinceBatteryReplaced,10);
			_ltow(m_pDiagnostics->m_diagData.dwTimSincLstRebootMins,tcMinsSinceColdBoot,10);
			_ltow(m_pDiagnostics->m_diagData.dwBattReqTimeinCradleMins,tcMinsRequiredChargeTime,10);
			_ltow(m_pDiagnostics->m_diagData.dwTimElapsedChargingMins,tcTimElapsedChargingMins,10);

			
			return pControlsModule->SendPBNavigate (nInstanceID,
					m_pDiagnosticsNavigate,
					tcHealth,
					tcPercentage,
					tcMinutesCapacity,
					tcExpireMonths,
					tcDaysSinceBatteryReplaced,
					tcMinsSinceColdBoot,
					tcMinsRequiredChargeTime,
					tcTimElapsedChargingMins,NULL);
			
					
		}
	}
	pControlsModule->WriteLog (
			_T(__FUNCTION__), __LINE__,
			L"Unable to retrieve diagnostic battery data, does this device support battery diagnostics?");
	
	return FALSE;
}
*/

/******************************************************************************/
void CBattery::GetBatteryStatus (void)
{
	bUserRequest = true;
	// Refresh and send the current battery status
	Refresh ();
	bUserRequest = false;
}

/******************************************************************************/
void CBattery::GetSmartBatteryStatus (void)
{
	//  Only attempt to get the smart battery status if the RCM was loaded
	//  dynamically
	if (!m_bRcmLoaded)
	{
		pControlsModule->WriteLog (
			_T(__FUNCTION__), __LINE__,
			L"Smart Battery is not supported on this device");
		return;
	}

	if (!psSmartNavigate)
		return;

	//  Retrieve the Smart Battery Status from the device
	SMART_BATTERY_STATUS sms;
	memset(&sms, 0, sizeof(sms));
	SI_INIT(&sms);
	DWORD retVal = m_pRcm->lpfn_RCM_GetSmartBatteryStatus(&sms);
	//  Some devices (such as the ES400) use the RCM2 DLL which requires you to 
	//  open it before you use it (like a bag of crisps).  This is not required
	//  on all devices.
	if (retVal == E_RCM_NOTINITIALIZED)
	{
		retVal = m_pRcm->lpfn_RCM_Open();
		retVal = m_pRcm->lpfn_RCM_GetSmartBatteryStatus(&sms);
	}
	if (retVal == E_RCM_SUCCESS)
	{
		//  The Smart Battery Status is now loaded in sms, invoke the 
		//  navigate Function
		//  Retrieve the parameters from the Smart Battery Status
		//  Serial Number: return as String
		//  part Number: Return as String
		//  Battery Charge Cycles: Convert word to String
		TCHAR tcBatteryChargeCycles[64];
		_itow(sms.wBatteryChargeCycles, tcBatteryChargeCycles, 10);
		//  Battery Rated Capacity: Convert word to String
		TCHAR tcBatteryRatedCapacity[64];
		_itow(sms.wBatteryRatedCapacity, tcBatteryRatedCapacity, 10);
		//  Battery Manufactured Date: Date Structure to String (MM/DD/YYYY)
		TCHAR tcBatteryManufacturedDate[64];
		//  MFG_DATE structure appears to be returning Month as day and vice
		//  versa
		wsprintf(tcBatteryManufacturedDate, L"%.2u/%.2u/%.4u", 
			sms.BatteryMfgDate.byDay,
			sms.BatteryMfgDate.byMonth,
			sms.BatteryMfgDate.wYear);
		//  Battery Health, convert byte to string
		TCHAR tcBatteryHealth[16];
		if (sms.byStateOfHealth == 100)
			wcscpy(tcBatteryHealth, L"Healthy");
		else if (sms.byStateOfHealth == 254)
			wcscpy(tcBatteryHealth, L"Unhealthy");
		else
			wcscpy(tcBatteryHealth, L"Unknown");

		pControlsModule->SendPBNavigate (
				tcSmartBatteryEventNames,
				nInstanceID,
				psSmartNavigate,
				sms.szBatterySerialNumber,
				sms.szBatteryPartNumber,
				tcBatteryChargeCycles,
				tcBatteryRatedCapacity,
				tcBatteryManufacturedDate,
				tcBatteryHealth,
				NULL);	
	}
	else
	{
		//  Failed to retrieve Smart Battery Status, probably because the 
		//  device doesn't have one :/
		pControlsModule->WriteLog (
			_T(__FUNCTION__), __LINE__,
			L"Unable to retrieve Smart battery settings, does this device have a smart battery?");
	}
}

/******************************************************************************/
/******************************************************************************/
CWLAN *CSignal::pWLAN = NULL;
int CSignal::nWLANUsage = 0;

/******************************************************************************/
CSignal::CSignal (HINSTANCE hinstance, HWND hparent, int instance_id) :
	CIndicator (hinstance, hparent, instance_id)
{
	pControlsModule->Log(PB_LOG_WARNING, L"The Signal API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Signal API for future releases", _T(__FUNCTION__), __LINE__);

	LPCWSTR pvalue;
	UINT interval;

	// Create static WLAN interface (once only)
	if (nWLANUsage++ == 0)
		pWLAN = new CWLAN;

	psNavigate = NULL;
	
	// Create drawing resources
	nPenWidth = 1;
	hPen = CreatePen (PS_SOLID, nPenWidth , RGB (0, 0, 0));
	hTextPen = CreatePen (PS_SOLID, nPenWidth , RGB (0xff, 0xff, 0xff));
	
	hBrush = CreateSolidBrush (RGB (0, 0, 0));
	hEmptyBrush = CreateSolidBrush (RGB (0xff, 0xff, 0xff));

	// Default layout
	nPendingIcon = nIconPosition = ICON_BOTTOM;
	nPendingGraph = nGraphDirection = GRAPH_TO_LEFT;

	nPendingLayout = nLayout = LAYOUT_RIGHT;

	nLeft = CControlsModule::nScreenWidth - 32;
	nTop = 1;
	MoveControl ();
	CreateLayout ();

	// Force initial refresh
	Refresh ();

	// Get SignalRefresh from config
	pvalue = (*CControlsModule::PBCoreStruct.pGetAppSettingPtr) (nInstanceID, L"GUI\\SignalRefresh", L"Value");
	if (pvalue)
		interval = (UINT) _wtoi (pvalue);
	else
		interval = 5000;
	SetTimer (interval);
}

/******************************************************************************/
CSignal::~CSignal ()
{
	if (psNavigate)
		free (psNavigate);

	if (--nWLANUsage == 0)
		delete pWLAN;

	DeleteObject(hTextPen);
}
/******************************************************************************/
void CSignal::SetLevel (int level)
{
	if (nLevel == level) 
	{	//nothing to do, early return
		return;
	};

	nLevel= level;

	if (nLevel < 0)
		nLevel = 0;
	if (nLevel > 5)
		nLevel = 5;

	InvalidateRect (hWindow, NULL, FALSE);
}
/******************************************************************************/
void CSignal::OnBeforeNavigate (void)
{
	if (psNavigate)
	{
		free (psNavigate);
		psNavigate = NULL;
	}
}

/******************************************************************************/
BOOL CSignal::OpenWLAN ()
{
	return pWLAN->Open ();
}

/******************************************************************************/
HRGN CSignal::GetIconRegion (void)
{
	HRGN region, temp_region;

	region = CreateRectRgn (signalShapes[nLayout-1].wifiBox.left,
		signalShapes[nLayout-1].wifiBox.top, 
		signalShapes[nLayout-1].wifiBox.right, 
		signalShapes[nLayout-1].wifiBox.bottom);

	for (int i = 0;i<6;i++)
	{
		temp_region = CreateRectRgn (signalShapes[nLayout-1].barRegion[i][0][0], 
		signalShapes[nLayout-1].barRegion[i][0][1], 
		signalShapes[nLayout-1].barRegion[i][1][0], 
		signalShapes[nLayout-1].barRegion[i][1][1]);
		CombineRgn (region, region, temp_region, RGN_OR);
		DeleteObject (temp_region);
	}

	return region;
}
	
/******************************************************************************/
void CSignal::Paint (HDC hdc)
{


	DrawGraph (hdc);
	
	FillRect (hdc, &(signalShapes[nLayout-1].wifiBox), hBrush);
	// draw the "wifi" text 
	SelectObject (hdc, hTextPen);
	
	for (int i=0;i<13;i++)
	{
		MoveToEx (hdc, signalShapes[nLayout-1].wifiLines[i][0][0], signalShapes[nLayout-1].wifiLines[i][0][1], NULL);
		LineTo (hdc, signalShapes[nLayout-1].wifiLines[i][1][0], signalShapes[nLayout-1].wifiLines[i][1][1]);
	}

}

/******************************************************************************/
void CSignal::Refresh (void)
{
	ADAPTER_STATUS status;
	WCHAR signal [8], mac [18], dhcp [16], rssi [8];

	if (!pWLAN->GetAdapterStatus (&status))
	{
		// Status could not be found, probably because there is no WiFi connection
		memset (&status, 0, sizeof status);
	}

	// SetLevel expects value 0 - 5, so convert from percentage
	SetLevel (status.nSignalStrength / 20);

	if (psNavigate && psNavigate[0])
	{
		wsprintf (signal, L"%d", status.nSignalStrength);
		wsprintf (mac, L"%02X:%02X:%02X:%02X:%02X:%02X", status.bMacAddress [0], status.bMacAddress [1], status.bMacAddress [2], status.bMacAddress [3], status.bMacAddress [4], status.bMacAddress [5]);
		wcscpy (dhcp, status.bStatic ? L"Static" : L"DHCP");
		wsprintf (rssi, L"%d", status.nRSSI);

		pControlsModule->SendPBNavigate (tcSignalEventNames, nInstanceID, psNavigate,
			signal, status.szSSID, mac, status.szName, status.szDHCPAddress, dhcp, status.szGatewayAddress,
			status.szIpAddress, rssi, status.szSubnetMask, status.szWinsAddress,
			NULL);
	}
	InvalidateRect (hWindow, NULL, FALSE);
}

/******************************************************************************/
BOOL CSignal::SetMiscValue (LPCWSTR pparameter, LPCWSTR pvalue)
{
	if (!wcsicmp (pparameter, L"signalevent"))
	{
		if (psNavigate)
		{
			free (psNavigate);
			psNavigate = NULL;
		}

		if (pvalue)
			psNavigate = _wcsdup (pvalue);

		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"iconposition"))
	{
		SetIconPosition (pvalue);
		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"graphposition"))
	{
		SetGraphDirection (pvalue);
		return TRUE;
	}
	else if (!wcsicmp (pparameter, L"layout"))
	{
		SetLayout (pvalue);
		return TRUE;
	}
	else
		return FALSE;
}
/******************************************************************************/
void CSignal::CreateLayout (void)
{

	// Remove existing region so window can be resized safely
	// Region handle will be released by Windows itself
	SetWindowRgn (hWindow, NULL, FALSE);

	// Set window size according to overall orientation ??
	nWidth = SIGNAL_MAX_DIMENSION;
	nHeight = SIGNAL_MAX_DIMENSION;

	MoveControl ();

	// Determine drawing parameters
	nIconX = 0; nIconY = 0;


	SetWindowRgn (hWindow, GetIconRegion (), FALSE);
}

/******************************************************************************/
void CSignal::DrawGraph (HDC hdc)
{
	int n;

	
	SelectObject (hdc, hPen);

	for (n = 0; n < 5; n++)
	{	
		drawPolyLine(hdc,signalShapes[nLayout-1].barLines[n],5); 
		if (nLevel > n)
		{
			FillRect (hdc, &(signalShapes[nLayout-1].bars[n]), hBrush);
		}
		else
		{
			
			FillRect (hdc, &(signalShapes[nLayout-1].bars[n]), hEmptyBrush);
		}
	}
}

BOOL CSignal::SetColour (LPCWSTR colour)
{

	if (!StringToColour (colour, &cRgb))
		return FALSE;

	DeleteObject (hPen);
	hPen = CreatePen (PS_SOLID, nPenWidth, cRgb);

	DeleteObject (hBrush);
	hBrush = CreateSolidBrush (cRgb);

	// change text to black if colour is white
	DeleteObject (hTextPen);
	if (cRgb == RGB (0xff, 0xff, 0xff))
	{
		hTextPen = CreatePen (PS_SOLID, nPenWidth , RGB (0x0, 0x0, 0x0));
	
	}
	else
	{
		hTextPen = CreatePen (PS_SOLID, nPenWidth , RGB (0xff, 0xff, 0xff));
	
	}
	InvalidateRect (hWindow, NULL, FALSE);
	return TRUE;
}


