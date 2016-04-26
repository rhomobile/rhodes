#pragma once
#include "windows.h"
#define DIAGAPI32 _T("DiagAPI32.dll")

#define DIAG_DEFAULTDURATION 45


#include "DiagnosticAPI32.h"

typedef DWORD (WINAPI* LPFNDIAGGETDATA)(EDIAGPARAMID, PDIAG_IN_DATA, PDIAG_OUT_DATA);

class CDiagnostics
{
public:
	CDiagnostics(void);
	~CDiagnostics(void);
	BOOL Initialise();
	DWORD DoDiagnosticsCall();
	void SetAverageCurrent(DWORD dwAverageCurrent);//milli amps
	void SetTripDuration(DWORD dwTripDuration);//minutes
	DWORD GetAverageCurrent() {return m_dwAverageCurrent;}
	DWORD GetTripDuration() {return m_dwTripDuration;}
	friend class CBattery;
	friend void RunBattery();
	friend void RunBatteryInCradle();
private:
	
	
	
//function pointers	
	LPFNDIAGGETDATA m_pGetDiagnosticData;
	
///member vars
	HANDLE m_hDLL;
	DIAG_IN_DATA_t m_InData;
	DIAG_OUT_DATA_t m_diagData;// populated after a call to DIAG_GetDiagnosticData 


	DWORD m_dwTripDuration;
	DWORD m_dwAverageCurrent;

};
