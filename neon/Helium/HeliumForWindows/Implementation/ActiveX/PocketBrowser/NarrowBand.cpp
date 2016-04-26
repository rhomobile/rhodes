// NarrowBand.cpp : Implementation of CNarrowBand
#include "stdafx.h"
#include "NarrowBand.h"


#include "../../../AdditionalLibraries/APD Kit v2-19-06/apd_dll/prtlib.h"

/////////////////////////////////////////////////////////////////////////////
// CNarrowBand

/******************************************************************************/
STDMETHODIMP CNarrowBand::PSExternal(int iCommand, BSTR strParameter)
{

	
	TCHAR * ptcCommand = new TCHAR[wcslen(strParameter)+1];
	
	if(ptcCommand==NULL) 
		return S_FALSE;

	memset(ptcCommand, 0, (wcslen(strParameter)+1)*sizeof(TCHAR));

	wcscpy(ptcCommand, strParameter);

	if (::PSExternal(iCommand, ptcCommand))
	{
		MessageBox(::PSGetLastMessage(),TEXT("Adaptive Printer Driver"),
			MB_OK | MB_ICONINFORMATION);
		delete[] ptcCommand;
		ptcCommand = NULL;
		return S_FALSE;
	}
	
	delete[] ptcCommand;
	ptcCommand = NULL;
	
	return S_OK;
	
}


STDMETHODIMP CNarrowBand::PSExternalEx(int iCommand, BSTR strParameter, 
									   int *iReturn)
{
	TCHAR * ptcCommand = new TCHAR[wcslen(strParameter)+1];
	
	if(ptcCommand==NULL) 
		return S_FALSE;

	memset(ptcCommand, 0, (wcslen(strParameter)+1)*sizeof(TCHAR));

	wcscpy(ptcCommand, strParameter);

	*iReturn = ::PSExternal(iCommand, ptcCommand);
	
	delete[] ptcCommand;
	ptcCommand = NULL;
	
	return S_OK;
}

STDMETHODIMP CNarrowBand::PSGetLastMessage(BSTR *pVal)
{
	
	*pVal = SysAllocString(::PSGetLastMessage());

	return S_OK;
}