// PB_Signature_PLG.cpp : Defines the entry point for the DLL application.
//

#include "PB_Signature_PLG.h"

#include "SignatureModule.h"



CSignatureModule *pSignatureModule;

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
//	Authors:		Paul Henderson
//	Change History:		
//	Apr 2009	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL SignatureCapture_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
		
	//  Create an instance of the CSignature Class (The Module)
	//  and call preload on that instance.
	
	if (pSignatureModule == NULL)
	{
		pSignatureModule = new CSignatureModule();
		
		if(pSignatureModule == NULL){
			//  Failed to allocate memory.
			return FALSE;
		}
	}
	return pSignatureModule->Preload(pPBStructure,pPBCoreStructure);
	
}

////////////////////////////////////////////////////////////////////////////////
//	Description:	Invoke the Signature class's META tag handler
//	Authors:		Paul Henderson
//	Change History:		
//	Apr 2009	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL SignatureCapture_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	
	//  Invoke Metaproc on the signature object
	if (pSignatureModule != NULL)
		return pSignatureModule->Process(pbMetaStructure, pPBStructure);
	else
		return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Description:	Deallocate all memory associated with the CSigMod object
// Authors:		Paul Henderson
// Change History:		
// Apr 2009	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL SignatureCapture_Dispose(PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;
	if (pSignatureModule != NULL){
		iniRet = pSignatureModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;//the module has logged the error
		}
		if(iniRet== DEINITIALISED){//>the Module object will return DEINITIALISED when the last instance of target object is disposed of.
			delete pSignatureModule;
			pSignatureModule = NULL;
		}
		return TRUE;
	}
	return FALSE;
}

