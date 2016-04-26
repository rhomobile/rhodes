#pragma once

#include <windows.h>

class CCradleModule;

class CCradle
{
public:

	CCradle(int iInstanceID, CCradleModule *pModule);
	~CCradle();

	/**
	* Request the TSD cradle ID and return it in an event
	* /return TRUE/FALSE	success/failure
	*/
	BOOL GetCradleID();

	/**
	* Set the cradleEvent URI
	* /return TRUE/FALSE	success/failure
	*/
	BOOL SetCradleURI(LPCTSTR pszURI);

	/**
	* Clears the cradleEvent URI
	* /return TRUE/FALSE	success/failure
	*/
	BOOL ClearCradleURI();

private:
	
	CCradleModule	*m_pModule;		///< module pointer
	int				m_iInstanceID;	///< module instance ID
	LPTSTR			m_pCradleURI;	///< cradleEventURI

};