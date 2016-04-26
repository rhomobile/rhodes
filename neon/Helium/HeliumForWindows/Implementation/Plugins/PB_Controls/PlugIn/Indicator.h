/******************************************************************************/
#pragma once

/******************************************************************************/
#include "WLAN.h"
#include "Control.h"
#include "../../../common/Private/RcmLoader.h"
#include "Diagnostics.h"
#include <pm.h>

#ifndef QUEUE_ENTRIES 
#define QUEUE_ENTRIES       3 
#endif 
#ifndef MAX_NAMELEN 
#define MAX_NAMELEN         128 
#endif 
#ifndef QUEUE_SIZE 
#define QUEUE_SIZE          (QUEUE_ENTRIES * (sizeof(POWER_BROADCAST) + (MAX_NAMELEN * sizeof(TCHAR)))) 
#endif 


/******************************************************************************/
#define INVALID_ICON	0
#define	ICON_LEFT		1
#define	ICON_TOP		2
#define ICON_RIGHT		3
#define ICON_BOTTOM		4

#define GRAPH_TO_LEFT	1
#define GRAPH_TO_TOP	2
#define GRAPH_TO_RIGHT	3
#define GRAPH_TO_BOTTOM	4


#define LAYOUT_LEFT		1
#define LAYOUT_UP		2
#define LAYOUT_RIGHT	3
#define LAYOUT_DOWN		4

#define BATTERY_MAX_DIMENSION	42
#define SIGNAL_MAX_DIMENSION	32

/******************************************************************************/

static TCHAR* tcSignalEventNames[] = 
{
	L"signalStrength", 
	L"essid", 
	L"macAddress", 
	L"adapterName", 
	L"dhcpServer", 
	L"dhcpStatic", 
	L"gateway", 
	L"ipAddress", 
	L"rssi", 
	L"subnetMask", 
	L"wins", 
	NULL
};

static TCHAR* tcBatteryEventNames[] = 
{
	L"acLineStatus", 
	L"batteryLifePercent", 
	L"backupBatteryLifePercent", 
	L"eventSource",
	L"batteryLifeKnown",
	L"backupBatteryLifeKnown",
	NULL
};
static TCHAR* tcSmartBatteryEventNames[] = 
{
	L"serialNumber", 
	L"partNumber", 
	L"batteryChargeCycles", 
	L"ratedCapacity", 
	L"manufactureDate", 
	L"stateOfHealth", 
	NULL
};
static TCHAR* tcBatteryDiagnosticsEventNames[] = 
{
	L"stateOfHealthPercent",
	L"batteryCapacityPercent",
	L"batteryCapacityMinutes",
	L"batteryExpirationInMonths",
	L"previousBatteryReplacement",
	L"timeSinceLastColdBoot",
	L"requiredChargeTime",
	L"chargingTime",
	NULL
};

class CIndicator: public CControl
{
public:
	CIndicator (HINSTANCE hinstance, HWND hparent, int instance_id);
	virtual ~CIndicator ();

	// Overrides of virtual base class functions
	virtual BOOL SetLeft (int left);
	virtual BOOL SetTop (int top);
	virtual BOOL SetColour (LPCWSTR colour);

	virtual void SetLevel (int level) = 0;

private:
	static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	static BOOL bRegistered;
	

protected:

	void drawPolyLine(HDC hdc,const int points[][2],int numPoints);   

		
	int nLevel;			// 0 to 5
	UINT uiTimer;
	COLORREF cRgb; // colour of image
	HPEN hPen;			// For drawing icon and empty bar
	HBRUSH hBrush;		// For filling bar
	HBRUSH hEmptyBrush;		// For filling bar
	
	int nPenWidth;
	int nIconX, nIconY;	// Start point to icon drawing
	int nIconPosition, nPendingIcon;
	int nGraphDirection, nPendingGraph;
	int nLayout, nPendingLayout;
	RECT rectGraph [5];	
	
	void SetTimer (UINT interval);
	virtual	BOOL SetLayout (LPCWSTR direction);
	virtual BOOL SetIconPosition (LPCWSTR position);
	virtual BOOL SetGraphDirection (LPCWSTR direction);

	virtual void UsePending (void);
	virtual void CreateLayout (void) = 0;
	virtual void Paint (HDC hdc) = 0;
	virtual void Refresh (void) = 0;
	virtual void DrawGraph (HDC hdc) =0;
	virtual HRGN GetIconRegion (void) = 0;
};
struct sBatteryShapes
{
	int outline [9][2]; // coordinates of the battery outline
	int region[2][2][2]; //end points of the two regions that make up the battery
	RECT bars [4]; //coordinates of the bars
	POINT chargeShape [6]; //coordinates of the charging shape

};
struct sSignalShapes
{
		RECT wifiBox; //coordinates of the Wifi Box
		int wifiLines[13][2][2]; // coordinates of the letter lines
		int barRegion[6][2][2]; //coordinates of the bar regions
		int barLines[5][5][2]; //coordinates of the bar lines
		RECT bars [5]; //coordinates of the bars

};
/******************************************************************************/
class CBattery: public CIndicator
{
public:
	CBattery (HINSTANCE hinstance, HWND hparent, int instance_id);
	virtual ~CBattery ();

	BOOL SetMiscValue (LPCWSTR pparameter, LPCWSTR pvalue);
	void OnBeforeNavigate (void);
	virtual void SetLevel (int level);
	virtual BOOL SetColour (LPCWSTR colour);

private:

	BOOL bIsCharging;	
	LPWSTR psNavigate;
	LPWSTR psSmartNavigate;
	LPWSTR m_pDiagnosticsNavigate;
	void GetSmartBatteryStatus (void);
	static sBatteryShapes batteryShapes[4];
	CRcmLoader* m_pRcm;		///<  EMDK Rcm DLL loaded dynamically
	BOOL m_bRcmLoaded;		///<  Whether or not the Rcm DLL has been loaded

	BOOL bBatteryThreadRunning;				
	HANDLE hBatteryThread;	
	HANDLE hBatteryNotification;
	HANDLE hBatteryThreadQuit;
	HANDLE hBatteryThreadQueue;
	UINT uInterval;
	BOOL bUserRequest;

	void GetBatteryStatus (void);
	void SetInterval(LPCWSTR lpInterval);
	void SetNotification(LPCWSTR lpNotify);	
	void CBattery::ProcessBatteryThread (void);
	static DWORD CBattery::BatteryThreadProc(LPVOID pParam);		
	void EnableBatteryNotifications (void);
	void DisableBatteryNotifications (void);
	
	BOOL GetBatteryDiagnostics (void);
	CDiagnostics *m_pDiagnostics;
	
protected:


	HPEN hShapePen; // thin pen to draw charging shape
	virtual void DrawGraph (HDC hdc);
	virtual void CreateLayout (void);
	virtual void Paint (HDC hdc);
	virtual void Refresh (void);
	virtual HRGN GetIconRegion (void);
};

/******************************************************************************/
class CSignal: public CIndicator
{
public:
	CSignal (HINSTANCE hinstance, HWND hparent, int instance_id);
	virtual ~CSignal ();
	BOOL OpenWLAN (void);

	BOOL SetMiscValue (LPCWSTR pparameter, LPCWSTR pvalue);
	void OnBeforeNavigate (void);
	virtual void SetLevel (int level);

private:
	static CWLAN *pWLAN;
	static int nWLANUsage;

	LPWSTR psNavigate;
	static sSignalShapes signalShapes[4];

protected:

	
	HPEN hTextPen; // white pen to draw Wifi text
	virtual void DrawGraph (HDC hdc);
	virtual void CreateLayout (void);
	virtual void Paint (HDC hdc);
	virtual void Refresh (void);
	virtual BOOL SetColour (LPCWSTR colour);
	virtual HRGN GetIconRegion (void);
};
