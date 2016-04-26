#pragma once

#include "WLAN.h"
#include "Control.h"
#include "RcmLoader.h"
#include <pm.h>
#include "api_generator/MethodResult.h"
#include "../../../../../../../neon/Helium/HeliumForWindows/Implementation/Plugins/PB_Controls/Plugin/Diagnostics.h"

#ifndef QUEUE_ENTRIES 
#define QUEUE_ENTRIES       3 
#endif 
#ifndef MAX_NAMELEN 
#define MAX_NAMELEN         128 
#endif 
#ifndef QUEUE_SIZE 
#define QUEUE_SIZE          (QUEUE_ENTRIES * (sizeof(POWER_BROADCAST) + (MAX_NAMELEN * sizeof(TCHAR)))) 
#endif 

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

enum eTriggerType {TRIGGER_SYSTEM, TRIGGER_PERIODIC};

class CIndicator: public CControl, public rho::common::CRhoThread
{
	//  Extending rhoThread gives us ability to have periodic access to Signal / Battery
public:
	virtual void run();
	CIndicator (HINSTANCE hinstance, HWND hparent);
	virtual ~CIndicator ();

	// Overrides of virtual base class functions
	virtual BOOL SetLeft (int left);
	virtual BOOL SetTop (int top);
	virtual BOOL SetColour (LPCWSTR colour);
	virtual unsigned int GetInterval() = 0;
	virtual void StartSystemNotifications() = 0;	//  Begin system event driven battery status
	virtual void StopSystemNotifications() = 0;		//  Cease system event driven battery status
	virtual void SetLevel (int level) = 0;	
	virtual	BOOL SetLayout (LPCWSTR direction);
	void SetTimer (int iInterval);					//  For periodic notifications start the thread
	void AddListener();								//  Listener is either the callback or the icon
	void RemoveListener();
	bool GetIconShown() {return bIconShown;}
	BOOL Show ();
	BOOL Hide ();
	void IconInitialization(HINSTANCE hinstance, HWND hparent);	//  Initialisation needs to be done in UI thread
	void SetCallback(rho::apiGenerator::CMethodResult pCallback);

private:
	static LRESULT CALLBACK StaticWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	static BOOL bRegistered;

protected:

	void drawPolyLine(HDC hdc,const int points[][2],int numPoints);   
	int nLevel;			// 0 to 5
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
	unsigned int m_iListeners;	//  Listeners are either callback or icon drawn
	unsigned int m_iInterval;
	bool bIconShown;
	int m_iIndicatorValue;
	rho::apiGenerator::CMethodResult	m_pCallback;
	eTriggerType m_triggerType;	//  Trigger is system event or periodic (battery only)
	virtual void UsePending (void);	//  redraws the control after change
	virtual void CreateLayout (void) = 0;
	virtual void Paint (HDC hdc) = 0;
	virtual void Refresh (rho::apiGenerator::CMethodResult& pCallback) = 0;	//  Notify user of current level
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
	CBattery (HINSTANCE hinstance, HWND hparent);
	virtual ~CBattery ();

	virtual void SetLevel (int level);
	virtual BOOL SetColour (LPCWSTR colour);
	void SetInterval(int iInterval);
	unsigned int GetInterval();
	void Refresh (rho::apiGenerator::CMethodResult& pCallback);
	void SetTrigger(rho::String trigger);	//  Either system event or periodic
	void GetSmartBatteryStatus (rho::apiGenerator::CMethodResult& pCallback);
	void SetTripDuration(unsigned int iTripDuration);
	unsigned int GetTripDuration();
	void SetAverageCurrentConsumption(unsigned int iAverageCurrentConsumption);
	unsigned int GetAverageCurrentConsumption();
	void GetBatteryDiagnostics (rho::apiGenerator::CMethodResult& oResult);
private:
	BOOL bIsCharging;	
	static sBatteryShapes batteryShapes[4];
	CRcmLoader* m_pRcm;		///<  EMDK Rcm DLL loaded dynamically
	BOOL m_bRcmLoaded;		///<  Whether or not the Rcm DLL has been loaded
	unsigned int m_iTripDuration;				///<  Used in calculations for Battery Diagnostics
	unsigned int m_iAverageCurrentConsumption;	///<  Used in calculations for Battery Diagnostics


	//  For System access, mostly lifted from RE1
	BOOL bBatteryThreadRunning;				
	HANDLE hBatteryThread;	
	HANDLE hBatteryNotification;
	HANDLE hBatteryThreadQuit;
	HANDLE hBatteryThreadQueue;

	void SetNotification(bool bEnabled);	
	void CBattery::ProcessBatteryThread (void);
	static DWORD CBattery::BatteryThreadProc(LPVOID pParam);		
	void EnableBatteryNotifications (void);	//  Lifted from RE1 - starts battery system events
	void DisableBatteryNotifications (void);
	void StartSystemNotifications();	//  Begin system event driven battery status
	void StopSystemNotifications();		//  Cease system event driven battery status
	CDiagnostics *m_pDiagnostics;		//  Controller for Battery Diagnostics
	
protected:

	HPEN hShapePen; // thin pen to draw charging shape
	virtual void DrawGraph (HDC hdc);
	virtual void CreateLayout (void);
	virtual void Paint (HDC hdc);
	virtual HRGN GetIconRegion (void);
};

/******************************************************************************/
class CSignalIndicators: public CIndicator
{
public:
	CSignalIndicators (HINSTANCE hinstance, HWND hparent);
	virtual ~CSignalIndicators ();
	BOOL OpenWLAN (void);

	virtual void SetLevel (int level);
	virtual BOOL SetColour (LPCWSTR colour);
	void SetInterval(unsigned int iInterval);
	unsigned int GetInterval() {return m_iInterval;}
	void Refresh (rho::apiGenerator::CMethodResult& pCallback);

private:
	static CWLAN *pWLAN;
	static int nWLANUsage;

	static sSignalShapes signalShapes[4];
	unsigned int	m_iInterval;	//  For periodic access
	void StartSystemNotifications();	//  No function in Signal
	void StopSystemNotifications();		//  No function in Signal

protected:
	
	HPEN hTextPen; // white pen to draw Wifi text
	virtual void DrawGraph (HDC hdc);
	virtual void CreateLayout (void);
	virtual void Paint (HDC hdc);
	virtual HRGN GetIconRegion (void);
};
