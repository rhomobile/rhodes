// PB_FileTransfer_PLG.cpp : Defines the entry point for the DLL application.
//

#include "PB_FileTransfer_PLG.h"
#include "FileTransfer.h"

CFileTransfer *pFileTransfer;//pointer to my PBModule derived class


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
#pragma region DLL_Exports

////////////////////////////////////////////////////////////////////////////////
//	Description:	Invoke the FileTransfer class's META tag handler
//	Authors:		Paul Henderson
//	Change History:		
//	Apr 2009	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL FileTransfer_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	
	//  Create an instance of the CFileTransfer Class (The Module)
	//  and call preload on that instance.
	if (pFileTransfer == NULL)
	{
		pFileTransfer = new CFileTransfer();
		if(pFileTransfer == NULL){
			//  Failed to allocate memory.
			return FALSE;
		}
	}
	return pFileTransfer->Preload(pPBStructure,pPBCoreStructure);
	
	
	
}

////////////////////////////////////////////////////////////////////////////////
//	Description:	Invoke the FileTransfer class's META tag handler
//	Authors:		Paul Henderson
//	Change History:		
//	Apr 2009	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL FileTransfer_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	
	//  Invoke Metaproc on the FileTransfer object
	if (pFileTransfer != NULL)
		return pFileTransfer->Process(pbMetaStructure, pPBStructure);
	else
		return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Description:	Deallocate all memory associated with the FileTransfer object
// Authors:		Paul Henderson
// Change History:		
//	Nov 2008	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL FileTransfer_Dispose(PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;	
	if (pFileTransfer != NULL)
	{
		iniRet = pFileTransfer->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;
		}
		if(iniRet == DEINITIALISED){//>the Module object will return DEINITIALISED when the last instance of target object is disposed of.
			delete pFileTransfer;
			pFileTransfer = NULL;
		}
		return TRUE;
		
	}

	return FALSE;
}

#pragma endregion 