#pragma once
#include "..\..\Common\Public\PB_Defines.h"
#include "CriticalSect.h"


extern BOOL Log	(LogTypeInterface logSeverity,LPCTSTR pLogComment, 
									LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule);
extern BOOL GetProductVersionString(TCHAR* tcFileName, CVersion* listVersions);

//each time a hash search takes place, it will populate this structure 
typedef struct ExFuncs
{
	pMetaProcFunc	pMetaProc;//pointer to the exported function EG: RFID_MetaProc
	pPreLoadFunc	pPreLoad;
	pDisposeFunc	pDispose;
}EXFUNCS,*PEXFUNCS;



struct preloadCount
{
	int				iTabID;
	preloadCount	*pNext;
};



class CAlias{
public:	
	CAlias(void);
	~CAlias(void);	
	LPTSTR			m_pAliasName;
	CAlias			*m_pAlias;
	//DWORD			m_dwHashVal;	//holds the hash value of the Meta tag alias for fast searching
	BOOL			NewAlias(LPTSTR pName);
	BOOL			Init(LPTSTR pName);
	
	
	
};


typedef long MOD_REF;
typedef long PLG_REF;
///< one of these for each module
class CModRec{
public:	
	CModRec			(void);
	~CModRec		(void);	
	
	MOD_REF			Init			(LPTSTR pModuleName);
	BOOL			LoadPlugin		(HMODULE hLib);
	pMetaProcFunc	m_pMetaFunc;	//pointer to the exported function EG: RFID_MetaProc
	pPreLoadFunc	m_pPreLoadFunc;
	pDisposeFunc	m_pDisposeFunc;
	
	//protected:

	LPTSTR			m_pName;		//The module name EG: 'RFID' 
	//DWORD			m_dwHashVal;	//holds the hash value of the Meta tag for fast searching
	CAlias			*m_pAlias;		//pointer to a structure that may contain an alias
	CModRec			*m_pModule;
	preloadCount	m_PreloadCount; ///keeps a record of tabIDs that have called Preload on this module 
	void			deleteTabID	(preloadCount *pPreloadCount);
	BOOL			Preload		(PBStruct *pbStructure);
	BOOL			Dispose		(PBStruct *pbStructure);
	BOOL			SetProperty (PBStruct *pbStructure,LPCTSTR pParam,LPCTSTR pValue);
	BOOL			CallMethod	(PBStruct *pbStructure,LPCTSTR pMethod);
	//PPBCORESTRUCT	m_pPBCoreStructure;
};



//
//each class includes a pointer to a linked list of alias'
//
// stores the location,name and handle to each plugin DLL
//
class CMetaRec{
public:	
	CMetaRec(void);
	~CMetaRec(void);
	PLG_REF		Init(LPTSTR pLocation);
	CModRec		*m_pModule;		// pointer to a sub linked list
	LPTSTR		m_pLocation;	//contains the path and filename of the plugin module. In the future this maybe online
	CMetaRec	*m_pMetaRec;	//since this is a linked list, this points to the next structure that shares the dll
	HMODULE 	m_hLib;
	
	
};

//
//	There will be one instance of this class in PB
//	The purpose: 
//	1.	Contains a table of function pointers to the plugin modules
//	2.	When a metatag is submitted to the CMeta object it will be routed to the correct module
//
//	
class CMeta:public Lockable
{
public:
			CMeta(void);
	MOD_REF		AddModule		(PLG_REF plgRef,LPTSTR pName);		//adds a module to the table but does not load the DLL
	BOOL		AddAlias		(MOD_REF modRef,LPTSTR pName);		//adds an alias to the module 	
	PLG_REF		AddPlugin		(LPTSTR pLocation);
	BOOL		CallPlugMethod	(PBStruct *pPBStructure,LPCTSTR pTargetMod,LPCTSTR pMethod,LPCTSTR pCallingModule = NULL);
	BOOL		SetPlugProperty (PBStruct *pPBStructure,LPCTSTR pTargetMod,LPCTSTR pParam,LPCTSTR pValue,LPCTSTR pCallingModule = NULL);
	BOOL		PreloadModule	(PPBSTRUCT pPBStructure,LPCTSTR pTargetMod);
				
	
	BOOL		DisposeModule	(PBStruct *pbStructure,LPCTSTR pTargetMod);
	BOOL		RemoveApp(PBStruct *pbStructure);
		
	BOOL		Load(MOD_REF modRef,BOOL bPreload);
	//BOOL		Init(PPBCORESTRUCT pPBCoreStructure);
	CModRec		*GetModule(LPCTSTR pModName);
	CModRec		*GetModule(LPCTSTR pModName,BOOL bPreLoad);
	~CMeta(void);

	/**
	*  Function to iterate over all the plugins specified in Plugin.xml and 
	*  retrieve their versions.
	*  \param [out] Linked list of version information found in the plugin DLLs
	*  \return true if the version information was successfully retrieved, else false.
	*/
	BOOL		RetrieveProductVersion(CVersion*);
protected:
	
	
	void			deletePreloadCount(preloadCount *pPreloadCount);
	MOD_REF			AddModule(CModRec &pMod,LPTSTR pName);
	PLG_REF			AddPlugin(CMetaRec *pRec,LPTSTR pLocation);
	DWORD			ConvertToHash(LPCWSTR tcHTTPEquiv, int iLen);
	DWORD			ConvertToHash(LPCWSTR tcHTTPEquiv);
	int				CompareMetaHash(const void* arg1, const void* arg2);
	CMetaRec		*m_pHeadMetaRec;		//linked list pointer
	
	preloadCount	m_PreloadCount;
	//BOOL			preload(PBStruct *pbStructure,DWORD dwHashTargMod);
};
