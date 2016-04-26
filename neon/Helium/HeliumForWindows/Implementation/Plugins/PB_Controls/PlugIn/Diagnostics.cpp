//#include "StdAfx.h"
#include "Diagnostics.h"

CDiagnostics::CDiagnostics(void): 
m_dwTripDuration(0),		//setting to zero invokes the driver default of 45 mins
m_dwAverageCurrent(0),		//setting to zero utilises the driver's average current  
m_hDLL(NULL),				//handle to the diagnostics DLL
m_pGetDiagnosticData(NULL)	//function pointer for DIAG_GetDiagnosticData

{
	/*m_dwTripDuration	= 0;
	m_dwAverageCurrent	= 0;*/
}

CDiagnostics::~CDiagnostics(void)
{
	if(m_hDLL) FreeLibrary((HMODULE)m_hDLL);
}

DWORD CDiagnostics::DoDiagnosticsCall()
{
	
	
	SI_INIT(&m_InData);//initialise the structure
	SI_INIT(&m_diagData);//initialise the structure
	m_InData.dwTripMin		= m_dwTripDuration;
	m_InData.dwAvgCurrent	= m_dwAverageCurrent;
		
	return m_pGetDiagnosticData (DIAG_ALL,&m_InData,&m_diagData);
	
}

BOOL CDiagnostics::Initialise()
{
	if(m_hDLL && m_pGetDiagnosticData) return TRUE;
	
	try{
		m_hDLL = LoadLibrary(DIAGAPI32);
			if(m_hDLL){
				m_pGetDiagnosticData = (LPFNDIAGGETDATA)GetProcAddress((HMODULE)m_hDLL,L"DIAG_GetDiagnosticData");
				if(m_pGetDiagnosticData){
					return TRUE;
				}
			}
		}
	
	
	catch(...){
		
		
	}
	
	m_pGetDiagnosticData = NULL;
	m_hDLL = NULL;
	return FALSE;

}


void CDiagnostics::SetAverageCurrent(DWORD dwAverageCurrent)//milli amps
{
	m_dwAverageCurrent = dwAverageCurrent;
}

void CDiagnostics::SetTripDuration(DWORD dwTripDuration)//minutes
{
	0 == dwTripDuration ?m_dwTripDuration = DIAG_DEFAULTDURATION:m_dwTripDuration = dwTripDuration;
}
