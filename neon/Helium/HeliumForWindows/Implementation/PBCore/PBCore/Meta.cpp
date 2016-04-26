#include "StdAfx.h"
#include "Meta.h"

extern PPBCORESTRUCT		g_pPBCore;

#pragma region PB_Meta

CMeta::CMeta(void)
{
	m_pHeadMetaRec	= NULL;
	memset(&m_PreloadCount,0,sizeof(preloadCount)); 
}

CMeta::~CMeta(void)
{
	delete 		m_pHeadMetaRec;
	deletePreloadCount(&m_PreloadCount);
}

void CMeta::deletePreloadCount(preloadCount *pPreloadCount)
{
	if(pPreloadCount->pNext){
		deletePreloadCount(pPreloadCount->pNext);
	}
	
}

BOOL CMeta::CallPlugMethod	(PBStruct *pPBStructure,LPCTSTR pTargetMod,LPCTSTR pMethod,LPCTSTR pCallingModule)
{
	CModRec *pMod;		
	if(pMod = GetModule(pTargetMod)){
		pMod->Preload(pPBStructure);
		return pMod->CallMethod(pPBStructure,pMethod);
	}

	TCHAR tcMessage[] = {L"Module %s not found."};
	LPTSTR errorString = new TCHAR[_tcslen(pTargetMod) + _tcslen(tcMessage) -1 ];
	if(errorString){
		wsprintf(errorString, tcMessage, pTargetMod);
		Log(PB_LOG_WARNING, errorString, L"CMeta::CallPlugMethod", __LINE__, L"Core");
		delete [] errorString;
	}

	return FALSE; //error or the library has not been loaded yet
}


BOOL CMeta::SetPlugProperty (PBStruct *pPBStructure,LPCTSTR pTargetMod,LPCTSTR pParam,LPCTSTR pValue,LPCTSTR pCallingModule)
{
	CModRec *pMod;	
	if(pMod = GetModule(pTargetMod)){
		pMod->Preload(pPBStructure);
		return pMod->SetProperty(pPBStructure,pParam,pValue);
	}

	TCHAR tcMessage[] = {L"Module %s not found."};
	LPTSTR errorString = new TCHAR[_tcslen(pTargetMod) + _tcslen(tcMessage) -1 ];
	
	if(errorString){	
		wsprintf(errorString, tcMessage, pTargetMod);
		Log(PB_LOG_WARNING, errorString, L"CMeta::SetPlugProperty", __LINE__, L"Core");
		delete [] errorString;
	}

	return FALSE; //error or the library has not been loaded yet
}



//call preload on a module
//pTargetMod cannot be an alias as specified in the configuration XML file 
BOOL CMeta::PreloadModule(PPBSTRUCT pPBStructure,LPCTSTR pTargetMod)
{
	CModRec *pMod;
	
	if(pMod = GetModule(pTargetMod))
		return pMod->Preload(pPBStructure);

	TCHAR tcMessage[] = {L"Module %s not found."};
	LPTSTR errorString = new TCHAR[_tcslen(pTargetMod) + _tcslen(tcMessage) -1 ];
	
	if(errorString){	
		wsprintf(errorString, tcMessage, pTargetMod);
		Log(PB_LOG_WARNING, errorString, L"CMeta::PreloadModule", __LINE__, L"Core");
		delete [] errorString;
	}

	return FALSE;
}



//called when an application exits
//The plug-in manager will call dispose on all modules dependent on this application instance  
BOOL CMeta::RemoveApp(PBStruct *pbStructure)
{
	if(m_pHeadMetaRec == NULL)
		return 0;
	
	CMetaRec	*pTempRec;
	CModRec		*pTempMod;
	
	//match the value in the memory
	for(pTempRec = m_pHeadMetaRec;pTempRec;pTempRec = pTempRec->m_pMetaRec)//for each DLL
	{
		for(pTempMod = pTempRec->m_pModule;pTempMod;pTempMod = pTempMod->m_pModule )///<for each module in that DLL
		{
			if(wcsstr(pTempRec->m_pLocation, L"WTG_Scanner_PLG")!= NULL)
			{
				PBMetaStruct scannerMetaStruct;
				TCHAR tcEmptyString[1];
				tcEmptyString[0] = '\0';
				if(pTempMod->m_pMetaFunc)
				{
					scannerMetaStruct.lpParameter	= TEXT("Disabled");
					scannerMetaStruct.lpValue		= tcEmptyString;
					pTempMod->m_pMetaFunc(&scannerMetaStruct,pbStructure);
				}
			}
			else
			{
				if (wcsstr(pTempRec->m_pLocation, L"WTG_Controls_PLG") != NULL || wcsstr(pTempRec->m_pLocation, L"WTG_DeviceApplication_PLG") != NULL)
				{
					//  There has always been a known issue shutting down Browser ,
					//  this work around allows the RE extension to shut down at least
				}
				else
				{
					DEBUGMSG(TRUE, (L"Calling dispose on %s\n", pTempMod->m_pName));
					pTempMod->Dispose(pbStructure);
				}
			}

		}
	}	
	return TRUE;//couldn't find the plugin name - check to see if it has been specified in the configuration XML file
}

//call dispose on a instance attachment to a module
BOOL CMeta::DisposeModule(PBStruct *pbStructure,LPCTSTR pTargetMod)
{
	
	CModRec *pMod;		
	if(pMod = GetModule(pTargetMod)){
		return pMod->Dispose(pbStructure);
	}
	
	return FALSE; //error or the library has not been loaded yet
	
}


//searches all of the records, modules and aliases for a matching name
//returns the function pointer
//If the library has not been loaded, it will get loaded here 
//NB: This function will call Preload on a module
//

CModRec	*CMeta::GetModule(LPCTSTR pModName,BOOL bPreLoad)
{
	CMetaRec	*pTempRec;
	CModRec		*pTempMod;
	CAlias		*pTempAlias;
	//match the value in the memory
	
	if(m_pHeadMetaRec == NULL)
		return 0;
	int iLen = wcslen(pModName);
	for(pTempRec = m_pHeadMetaRec;pTempRec;pTempRec = pTempRec->m_pMetaRec)//for each DLL
	{
		for(pTempMod = pTempRec->m_pModule;pTempMod;pTempMod = pTempMod->m_pModule )///<for each module in that DLL
		{//for each module
			
			if(!_memicmp(pTempMod->m_pName,pModName,iLen * 2)){
				if(wcslen(pTempMod->m_pName)== iLen){
					//we have a match
					return pTempMod;	
				}
			}
			///< now check each alias for this module 
			for(pTempAlias = pTempMod->m_pAlias;pTempAlias;pTempAlias = pTempAlias->m_pAlias)//for each alias
			{
				if(!_memicmp(pTempAlias->m_pAliasName,pModName,iLen*2)){
					if(wcslen(pTempAlias->m_pAliasName)== iLen){
						//we have a match
						return pTempMod;
					}
				}
			}
		}
	}
	return NULL;//module not found
}



CModRec	*CMeta::GetModule(LPCTSTR pModName)
{
	CMetaRec	*pTempRec;
	CModRec		*pTempMod;
	CAlias		*pTempAlias;
	//match the value in the memory
	
	if(m_pHeadMetaRec == NULL)
		return 0;
	int iLen = wcslen(pModName);
	for(pTempRec = m_pHeadMetaRec;pTempRec;pTempRec = pTempRec->m_pMetaRec)//for each DLL
	{
		for(pTempMod = pTempRec->m_pModule;pTempMod;pTempMod = pTempMod->m_pModule )///<for each module in that DLL
		{//for each module
			
			if(!_memicmp(pTempMod->m_pName,pModName,iLen * 2)){
				if(wcslen(pTempMod->m_pName)== iLen){
					//we have a match
					goto _matchFound;	
				}
			}
			///< now check each alias for this module 
			for(pTempAlias = pTempMod->m_pAlias;pTempAlias;pTempAlias = pTempAlias->m_pAlias)//for each alias
			{
				if(!_memicmp(pTempAlias->m_pAliasName,pModName,iLen*2)){
					if(wcslen(pTempAlias->m_pAliasName)== iLen){
						//we have a match
						goto _matchFound;
					}
				}
			}
		}
	}
	LPTSTR pErr = new TCHAR[_tcslen(pModName)+100];
	if(pErr){
		_tcscpy(pErr,L"Plug-in: ");
		_tcscat(pErr,pModName);
		_tcscat(pErr,L"cannot be found in Plugin.xml");
		Log(PB_LOG_INFO,pErr,L"CMeta::GetModule",__LINE__,L"Core");
		delete [] pErr;
	}
	
	return NULL;//module not found

_matchFound:

	if(pTempRec->m_hLib == NULL)
	{
		pTempRec->m_hLib = LoadLibrary(pTempRec->m_pLocation);
		if(pTempRec->m_hLib == NULL)
		{
			LPTSTR pErr = new TCHAR[_tcslen(pModName) + _tcslen(pTempRec->m_pLocation) + 100];
			if(pErr)
			{
				//  Get error information produced by LoadLibrary and log it.
				DWORD dwLastError = GetLastError();
				wsprintf(pErr, 
					L"Plug-in: %s in DLL: %s can not be loaded.  Last Error: %u", 
					pModName, pTempRec->m_pLocation, dwLastError);
				Log(PB_LOG_WARNING, pErr, L"CMeta::GetModule", __LINE__, L"Core");
				delete [] pErr;
			}
			return NULL;///<Couldn't load the DLL
		}
		
	}
	if(pTempMod->m_pPreLoadFunc == NULL){///< if this is the second module of a DLL, the DLL handle will not be NULL
		if(!pTempMod->LoadPlugin(pTempRec->m_hLib)){
			return NULL;
		}
	}
	
	
	return pTempMod;

}



//public
//adds a plugin to the table and returns a reference to it
PLG_REF CMeta::AddPlugin(LPTSTR pLocation)
{
	if(m_pHeadMetaRec == NULL){
		m_pHeadMetaRec = new CMetaRec();
		
	}
	if(m_pHeadMetaRec){
		return AddPlugin(m_pHeadMetaRec,pLocation);			
	}
	return (PLG_REF)0;
	
}

// private
//recursive function
PLG_REF CMeta::AddPlugin(CMetaRec *pRec,LPTSTR pLocation)
{
	if(pRec->m_pLocation == NULL){// is this a vacant slot?
		if(pRec->Init(pLocation)){
			return (PLG_REF)pRec;
		}
	}
	
	if(!_memicmp(pRec->m_pLocation,pLocation,wcslen(pLocation) *2)){
		return (PLG_REF)0; 
	}
	if(pRec->m_pMetaRec){//check if this record slot is initialised
		
		return AddPlugin(pRec->m_pMetaRec,pLocation);	
	}
	else{//the current record is full create a new one
		pRec->m_pMetaRec = new CMetaRec();
		if(pRec->m_pMetaRec){
			return AddPlugin(pRec->m_pMetaRec,pLocation);
		}
	}
	
	return (PLG_REF)0;
}


MOD_REF CMeta::AddModule(CModRec &pMod,LPTSTR pName)
{
	if(pMod.m_pName == NULL){//if this is the first module in that record
		if(pMod.Init(pName)){
			return (MOD_REF)&pMod;
		}
	}
	if(pMod.m_pModule){
		return AddModule(*pMod.m_pModule,pName);
	}
	else{
		pMod.m_pModule = new CModRec();
		if(pMod.m_pModule){
			if(pMod.m_pModule->Init(pName)){
				return (MOD_REF)pMod.m_pModule;
			}
		}
	}
	return (MOD_REF)0;

}

//adds a module to the table but does not load the DLL
// it returns a reference to the record
MOD_REF CMeta::AddModule(PLG_REF plgRef,LPTSTR pName)
{
	CMetaRec *pMetaRec = (CMetaRec *)plgRef;
	if(GetModule(pName,FALSE)){
		//module name is not unique
		Log(PB_LOG_ERROR,L"A duplicate module name",L"CMeta::AddModule",__LINE__,L"Core");

	}
	else if(pMetaRec){
		if(pMetaRec->m_pModule == NULL){
			pMetaRec->m_pModule = new CModRec();

		}
		if(pMetaRec->m_pModule){
			return AddModule(*pMetaRec->m_pModule,pName);
		}
	}
	return (MOD_REF)0;
	
}

BOOL CMeta::AddAlias(MOD_REF modRef,LPTSTR pName)
{
	
	//return TRUE;
	CModRec *pMod =(CModRec *)modRef;
	if(GetModule(pName,FALSE)){
		//module name is not unique
		Log(PB_LOG_ERROR,L"A duplicate alias name",L"CMeta::AddAlias",__LINE__,L"Core");

	}
	else if(pMod){
		if(pMod->m_pAlias == NULL){
			pMod->m_pAlias = new CAlias(); // this is the first alias to this module
			
		}
		if(pMod->m_pAlias){//add the alias to the module
			return pMod->m_pAlias->NewAlias(pName);
		}
	}
	return FALSE;
}








////////////////////////////////////////////////////////////////////////
// Function:	ConvertToHash
// Description:	Converts a metatag to it's hash equivelent so it can be 
//				efficiantly searched on.  It converts each char to lower
//				case by or-ing with 0x20 then adds it to the accumulator 
//				before left shifting the accumulator.  The result is a 
//				unique number.
//				tcHTTPEquiv is the tag being converted.
//				iLen is the number of characters to convert.
// Author:		James Morley-Smith
// Date:		25 Nov 2004
////////////////////////////////////////////////////////////////////////
DWORD CMeta::ConvertToHash(LPCWSTR tcHTTPEquiv, int iLen)
{

	//set the accumilator to 0
	DWORD dwAccum = 0;

	//get the number of chars to convert
	UINT uiLen = iLen;

	//iLen is defaulted to 0 so if 0 get the len of tcHTTPEquiv
	if(!uiLen)
		uiLen = (UINT)wcslen(tcHTTPEquiv);

	//if still 0 then invalid so return.
	if(!uiLen)
		return 0;

	//tchar to hold the lovercase version of the current char
	TCHAR tcCurrent;
	for(UINT i = 0;;i++)
	{
		//get lowercase version of current char
		//or-ing with 0x20 changes the diferating bit
		//between lower an upper.
		tcCurrent = tcHTTPEquiv[i] | 0x20;
		//add it to the accumilator
		dwAccum += tcCurrent;

		//break if the last char
		if(i==uiLen-1)
			break;

		//left shift the accumilator
		dwAccum = dwAccum << 1;
	}

	return dwAccum;
}


DWORD CMeta::ConvertToHash(LPCWSTR tcHTTPEquiv)
{
	return ConvertToHash(tcHTTPEquiv,wcslen(tcHTTPEquiv));	
}

////////////////////////////////////////////////////////////////////////
// Function:	CompareMetaHash
// Description:	Used by the qsort function in the constructor
// Author:		James Morley-Smith
// Date:		25 Nov 2004
////////////////////////////////////////////////////////////////////////
int CMeta::CompareMetaHash(const void* arg1, const void* arg2)
{
	if(*(DWORD*)arg1 == *(DWORD*)arg2) return 0;
	if(*(DWORD*)arg1 <  *(DWORD*)arg2) return -1;

	return 1;
}


/**
*  \author	Darryn Campbell (DCC, JRQ768)
*  \date	March 2011, Initial Creation (DCC)
*/
BOOL CMeta::RetrieveProductVersion(CVersion* listPluginVersions)
{
	//  m_pHeadMetaRec contains list of CMetaRec
	if (m_pHeadMetaRec == NULL)
		return FALSE;

	CMetaRec	*pTempRec;

	//for each DLL
	for(pTempRec = m_pHeadMetaRec; pTempRec; pTempRec = pTempRec->m_pMetaRec)
	{
		GetProductVersionString(pTempRec->m_pLocation, listPluginVersions);
	}


	return TRUE;
}


#pragma endregion

#pragma region PB_MetaRec


////////////////////////////////////////////////////////////////////////
// The definitions for the CMetaRec  and CModRec class
////////////////////////////////////////////////////////////////////////


CMetaRec::CMetaRec(void)
{
	m_pModule	= NULL;	// pointer to the next module in the linked list
	m_pLocation	= NULL;	//contains the path and filename of the plugin module. In the future this maybe online
	m_pMetaRec	= NULL;
	m_hLib		= NULL;
}


CMetaRec::~CMetaRec(void)
{
	delete m_pMetaRec;			//delete any linked records
	delete m_pModule;			//delete all modules and the module destructor will delete all aliases
	delete [] m_pLocation;
	
	if(m_hLib)
		FreeLibrary(m_hLib);
}


PLG_REF CMetaRec::Init(LPTSTR pLocation)
{
	delete [] m_pLocation;
	
	//allocate memory for the string
	m_pLocation = new WCHAR[wcslen(pLocation) + 1];
	if(m_pLocation){
		wcscpy(m_pLocation,pLocation);
		return (PLG_REF)this;
		
	}
	
	return (PLG_REF)0;
	
	
}


#pragma endregion

#pragma region PB_Alias

CAlias::CAlias()
{
	m_pAliasName	= NULL;
	m_pAlias		= NULL;
}

CAlias::~CAlias()
{
	
	delete []m_pAliasName;
	delete m_pAlias;
	
}


BOOL CAlias::Init(LPTSTR pName)
{
	delete [] m_pAliasName;
	m_pAliasName = NULL;
	
	m_pAliasName = new TCHAR[wcslen(pName)+1];
	if(m_pAliasName){
		wcscpy(m_pAliasName,pName);
		return TRUE;
	}
	
	return FALSE;
	
}

BOOL CAlias::NewAlias(LPTSTR pName)
{
	if(m_pAliasName == NULL)
		return Init(pName);
	if(m_pAlias)
		return m_pAlias->NewAlias(pName);
	
	//otherwise
	
	m_pAlias = new CAlias();
	if(m_pAlias)
		return m_pAlias->Init(pName);
		
	return FALSE;
}


#pragma endregion

#pragma region PB_ModRec
/***************************************************************************************
//
//	Parameters: pModuleName - the name of the module EG: Signature
//				lpFuncName	- the name of the exported function from the plugin DLL 					
//				dwHashVal	- the hash value of the function for quick searching	
//
//
****************************************************************************************/
CModRec::CModRec(void)
{
	m_pName					= NULL;
	m_pMetaFunc				= NULL;
	m_pPreLoadFunc			= NULL;
	m_pDisposeFunc			= NULL;
	m_pAlias				= NULL;		//pointer to a structure that could contain a alias
	m_pModule				= NULL;
	m_PreloadCount.iTabID	= PB_INVALID_APPID;
	m_PreloadCount.pNext	= NULL;
	
}

CModRec::~CModRec(void)
{
	//DELETEARR(m_lpFuncName);
    if ( m_pName )
	    delete [] m_pName;			
    if ( m_pModule )
	    delete m_pModule;
    if ( m_pAlias )
	    delete m_pAlias;	//delete all aliases

	deleteTabID(&m_PreloadCount);
}

// looks for an instance match, if it finds one, removes it from the tree and calls dispose 
//
BOOL CModRec::Dispose(PBStruct *pbStructure)
{
	preloadCount *pPreloadCount,*ptemp;
	
	if(m_PreloadCount.iTabID == PB_INVALID_APPID){
		return TRUE;//no preloads have taken place
	}


	if(m_PreloadCount.iTabID == pbStructure->iTabID){
		if(m_PreloadCount.pNext){
			ptemp = m_PreloadCount.pNext->pNext;
			m_PreloadCount.iTabID = m_PreloadCount.pNext->iTabID;
			delete m_PreloadCount.pNext;
			m_PreloadCount.pNext =  ptemp;
			goto _dispose;
		}
		m_PreloadCount.iTabID = PB_INVALID_APPID;
		goto _dispose;
	
	}
	
	
	for(pPreloadCount = &m_PreloadCount;pPreloadCount->pNext;pPreloadCount = pPreloadCount->pNext)
	{
		if(pPreloadCount->pNext->iTabID == pbStructure->iTabID){
			if(pPreloadCount->pNext->pNext){
				
				ptemp = pPreloadCount->pNext->pNext;
				//pPreloadCount->iTabID = pPreloadCount->pNext->iTabID;
				delete pPreloadCount->pNext;
				pPreloadCount->pNext =  ptemp;
				goto _dispose;
			}
			delete pPreloadCount->pNext;
			pPreloadCount->pNext = NULL;
			goto _dispose;
		}
	}
	return FALSE;
_dispose:
	
	if(m_pDisposeFunc){
		return m_pDisposeFunc(pbStructure);	
	}
	
	return TRUE;
}

BOOL CModRec::SetProperty (PBStruct *pbStructure,LPCTSTR pParam,LPCTSTR pValue)
{
	PBMetaStruct metaStruct;
	if(m_pMetaFunc){
		metaStruct.lpParameter	= pParam;
		metaStruct.lpValue		= pValue;

		if(m_pMetaFunc(&metaStruct,pbStructure)){
			return TRUE;
		}
		//execute if failed to set the property
		TCHAR tcMessage[] = {L"%s Plug-in failed to set the property: %s with value: %s"};
		int iLen = _tcslen(tcMessage) + _tcslen(m_pName) + _tcslen(pParam) + _tcslen(pValue);
		
		LPTSTR errorString = new TCHAR[_tcslen(tcMessage) + _tcslen(m_pName) + _tcslen(pParam) + _tcslen(pValue) -5 ];//let's not forget the NULL terminator
		if(errorString){
			wsprintf(errorString, tcMessage, m_pName, pParam, pValue);
			Log(PB_LOG_WARNING, errorString, L"CModRec::SetProperty", __LINE__, L"Core");
			delete [] errorString;
		}
		
	}
	return FALSE;
}

BOOL CModRec::CallMethod	(PBStruct *pbStructure,LPCTSTR pMethod)
{
	PBMetaStruct metaStruct;
	//   pass empty strings when values are not defined
	//  as part of the EMML tag as opposed to NULLs.
	TCHAR tcEmptyString[1];
	tcEmptyString[0] = '\0';
	if(m_pMetaFunc){
		metaStruct.lpParameter	= pMethod;
		metaStruct.lpValue		= tcEmptyString;

		if(m_pMetaFunc(&metaStruct,pbStructure)){
			return TRUE;
		}

		//execute if failed to call the method
		TCHAR tcMessage[] = {L"%s Plug-in failed to call the method: %s"};
		LPTSTR errorString = new TCHAR[_tcslen(tcMessage) + _tcslen(m_pName) + _tcslen(pMethod) -3];//let's not forget the NULL terminator
		if(errorString){	
			wsprintf(errorString, tcMessage, m_pName, pMethod);
			Log(PB_LOG_WARNING, errorString, L"CModRec::CallMethod", __LINE__, L"Core");
			delete [] errorString;
		}
	}
	return FALSE;
}

BOOL CModRec::Preload(PBStruct *pbStructure)
{
	preloadCount *pPreloadCount;
	
	for(pPreloadCount = &m_PreloadCount;;pPreloadCount = pPreloadCount->pNext)
	{
		if(pPreloadCount->iTabID == pbStructure->iTabID){
			return TRUE;	
		}
		if(pPreloadCount->iTabID == PB_INVALID_APPID){//first instance to attach to this module
			pPreloadCount->iTabID = pbStructure->iTabID;
			pPreloadCount->pNext = NULL;
			break;
		}
		else if(pPreloadCount->pNext == NULL){//if no match and next pointer is null
			pPreloadCount->pNext = new preloadCount;
			if(pPreloadCount->pNext){
				pPreloadCount = pPreloadCount->pNext;
				pPreloadCount->iTabID = pbStructure->iTabID;
				pPreloadCount->pNext = NULL;
				break;
			}
		}
	}
	
	if(m_pPreLoadFunc){
		BOOL bRet = m_pPreLoadFunc(pbStructure,g_pPBCore);
		if(!bRet){
			LPTSTR pStr = new TCHAR[_tcslen(m_pName)+30];
			if(pStr){
				_tcscpy(pStr,L"Failed to preload ");
				_tcscat(pStr,m_pName);
				Log(PB_LOG_ERROR,pStr,L"LoadPlugin",__LINE__,L"Core");
				delete [] pStr;
			}
		}
		return bRet;
	}
	
	
	
	
	return FALSE;
}


//
//	Load the plugin 
//
BOOL CModRec::LoadPlugin(HMODULE hLib)
{
	
	//build three function names
	WCHAR MetaName[100];
	WCHAR PreloadName[100];
	WCHAR DisposeName[100];

	//potential memory issue Microsoft bug using wsprintf for strings
	wsprintf(MetaName,L"%s%s%s",m_pName,L"_",L"MetaProc");
	wsprintf(PreloadName,L"%s%s%s",m_pName,L"_",L"Preload");
	wsprintf(DisposeName,L"%s%s%s",m_pName,L"_",L"Dispose");
	
		
	if (hLib){
		m_pMetaFunc = (pMetaProcFunc)GetProcAddress((HINSTANCE)hLib,MetaName);
		if(m_pMetaFunc){
			m_pPreLoadFunc = (pPreLoadFunc)GetProcAddress((HINSTANCE)hLib,PreloadName);
			if(m_pPreLoadFunc){
				m_pDisposeFunc = (pDisposeFunc)GetProcAddress((HINSTANCE)hLib,DisposeName);
				if(m_pDisposeFunc){
					Log(PB_LOG_INFO,m_pName,L"LoadPlugin",__LINE__,L"Core");
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}



void CModRec::deleteTabID(preloadCount *pPreloadCount)
{
	if(pPreloadCount->pNext){
		deleteTabID(pPreloadCount->pNext);
		delete pPreloadCount->pNext;
	}
	 
}





MOD_REF CModRec::Init(LPTSTR pModuleName)
{
	
	delete []m_pName;
	m_pName = NULL;
	
	m_pName = new TCHAR[wcslen(pModuleName)+1];
	if(m_pName){
		wcscpy(m_pName,pModuleName);
		return (MOD_REF)this;
	}
	return (MOD_REF)0;
		
}

#pragma endregion
