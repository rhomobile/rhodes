// PB_Multimedia_PLG.cpp : Defines the entry point for the DLL application.
//



#include "PB_Multimedia_PLG.h"
#include "AudioCaptureModule.h"
#include "VideoCaptureModule.h"
#include "MediaPlayerModule.h"


CAudioCaptureModule *pAudioCaptureModule;
CVideoCaptureModule *pVideoCaptureModule;
CMediaPlayerModule *pMediaPlayerModule;

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
//	Apr 2012	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL AudioCapture_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
		
	//  Create an instance of the CSignature Class (The Module)
	//  and call preload on that instance.
	
	if (pAudioCaptureModule == NULL)
	{
		pAudioCaptureModule = new CAudioCaptureModule();
		
		if(pAudioCaptureModule == NULL){
			//  Failed to allocate memory.
			return FALSE;
		}
	}
	return pAudioCaptureModule->Preload(pPBStructure,pPBCoreStructure);
	
}

////////////////////////////////////////////////////////////////////////////////
//	Description:	Invoke the CAudioCaptureModule class's META tag handler
//	Authors:		Paul Henderson
//	Change History:		
//	Apr 2012	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL AudioCapture_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	
	//  Invoke Metaproc on the signature object
	if (pAudioCaptureModule != NULL)
		return pAudioCaptureModule->Process(pbMetaStructure, pPBStructure);
	else
		return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Description:	Deallocate all memory associated with the CAudioCaptureModule object
// Authors:		Paul Henderson
// Change History:		
// Apr 2012	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL AudioCapture_Dispose(PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;
	if (pAudioCaptureModule != NULL){
		iniRet = pAudioCaptureModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;//the module has logged the error
		}
		if(iniRet== DEINITIALISED){//>the Module object will return DEINITIALISED when the last instance of target object is disposed of.
			delete pAudioCaptureModule;
			pAudioCaptureModule = NULL;
		}
		return TRUE;
	}
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////
//	Description:	Create a PBModule derived object on the first call from the core.
//					Then handle the Preload with it. 
//	Authors:		Paul Henderson
//	Change History:		
//	Apr 2012	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL VideoCapture_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
		
	//  Create an instance of the CSignature Class (The Module)
	//  and call preload on that instance.
	
	if (pVideoCaptureModule == NULL)
	{
		pVideoCaptureModule = new CVideoCaptureModule();
		
		if(pVideoCaptureModule == NULL){
			//  Failed to allocate memory.
			return FALSE;
		}
	}
	return pVideoCaptureModule->Preload(pPBStructure,pPBCoreStructure);
	
}

////////////////////////////////////////////////////////////////////////////////
//	Description:	Invoke the CVideoCaptureModule class's META tag handler
//	Authors:		Paul Henderson
//	Change History:		
//	Apr 2012	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL VideoCapture_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	//  Invoke Metaproc on the signature object
	if (pVideoCaptureModule != NULL)
		return pVideoCaptureModule->Process(pbMetaStructure, pPBStructure);
	else
		return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Description:	Deallocate all memory associated with the CVideoCaptureModule object
// Authors:		Paul Henderson
// Change History:		
// Apr 2012	- Created (PH)
////////////////////////////////////////////////////////////////////////////////
BOOL VideoCapture_Dispose(PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;
	if (pVideoCaptureModule != NULL){
		iniRet = pVideoCaptureModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;//the module has logged the error
		}
		if(iniRet== DEINITIALISED){//>the Module object will return DEINITIALISED when the last instance of target object is disposed of.
			delete pVideoCaptureModule;
			pVideoCaptureModule = NULL;
		}
		return TRUE;
	}
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////
//	Description:	Create a PBModule derived object on the first call from the core.
//					Then handle the Preload with it. 
//	Authors:		John Heywood
//	Change History:		
//	Nov 2012	- Created (JH)
////////////////////////////////////////////////////////////////////////////////
BOOL Mediaplayer_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
		
	//  Create an instance of the CSignature Class (The Module)
	//  and call preload on that instance.
	
	if (pMediaPlayerModule == NULL)
	{
		pMediaPlayerModule = new CMediaPlayerModule();
		
		if(pMediaPlayerModule == NULL){
			//  Failed to allocate memory.
			return FALSE;
		}
	}
	return pMediaPlayerModule->Preload(pPBStructure,pPBCoreStructure);
	
}

////////////////////////////////////////////////////////////////////////////////
//	Description:	Invoke the CMediaPlayerModule class's META tag handler
//	Authors:		John Heywood
//	Change History:		
//	Nov 2012	- Created (JH)
////////////////////////////////////////////////////////////////////////////////
BOOL Mediaplayer_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure) 
{
	
	//  Invoke Metaproc on the signature object
	if (pMediaPlayerModule != NULL)
		return pMediaPlayerModule->Process(pbMetaStructure, pPBStructure);
	else
		return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Description:	Deallocate all memory associated with the CMediaPlayerModule object
//	Authors:		John Heywood
//	Change History:		
//	Nov 2012	- Created (JH)
////////////////////////////////////////////////////////////////////////////////
BOOL Mediaplayer_Dispose(PPBSTRUCT pPBStructure) 
{
	INITRET iniRet;
	if (pMediaPlayerModule != NULL){
		iniRet = pMediaPlayerModule->Dispose(pPBStructure);
		if(iniRet == FAILED){
			return FALSE;//the module has logged the error
		}
		if(iniRet== DEINITIALISED){//>the Module object will return DEINITIALISED when the last instance of target object is disposed of.
			delete pMediaPlayerModule;
			pMediaPlayerModule = NULL;
		}
		return TRUE;
	}
	return FALSE;
}

