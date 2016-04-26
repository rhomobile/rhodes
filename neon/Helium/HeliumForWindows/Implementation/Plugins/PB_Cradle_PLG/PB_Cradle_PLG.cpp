// PB_Cradle_PLG.cpp : Defines the entry point for the DLL application.
//



#include "PB_Cradle_PLG.h"
#include "CradleModule.h"


CCradleModule *pCradleModule;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	Description:	Create a PBModule derived object on the first call from the core.
//					Then handle the Preload with it. 
//	Authors:		John Heywood
//	Change History:		
//	Jun 2012	- Created (JHMH)
////////////////////////////////////////////////////////////////////////////////
BOOL Cradle_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
		
	//  Create an instance of the CSignature Class (The Module)
	//  and call preload on that instance.
	
	if (pCradleModule == NULL)
	{
		pCradleModule = new CCradleModule();
		
		if(pCradleModule == NULL){
			//  Failed to allocate memory.
			return FALSE;
		}
	}
	return pCradleModule->Preload(pPBStructure,pPBCoreStructure);
	
}

////////////////////////////////////////////////////////////////////////////////
//	Description:	Invoke the CCradleModule class's META tag handler
//	Authors:		John Heywood
//	Change History:		
//	Jun 2012	- Created (JHMH)
////////////////////////////////////////////////////////////////////////////////
BOOL Cradle_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	
	//  Invoke Metaproc on the signature object
	if (pCradleModule != NULL)
		return pCradleModule->Process(pbMetaStructure, pPBStructure);
	else
		return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Description:	Deallocate all memory associated with the CCradleModule object
// Authors:		John Heywood
// Change History:		
// Jun 2012	- Created (JHMH)
////////////////////////////////////////////////////////////////////////////////
BOOL Cradle_Dispose(PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;
	if (pCradleModule != NULL){
		iniRet = pCradleModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;//the module has logged the error
		}
		if(iniRet== DEINITIALISED){//>the Module object will return DEINITIALISED when the last instance of target object is disposed of.
			delete pCradleModule;
			pCradleModule = NULL;
		}
		return TRUE;
	}
	return FALSE;
}

