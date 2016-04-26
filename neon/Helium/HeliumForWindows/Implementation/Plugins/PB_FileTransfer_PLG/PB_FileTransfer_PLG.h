#ifdef __cplusplus
extern "C" {
#endif

#include "..\..\Common\Public\pbplugin.h"


#ifdef PB_FILETRANSFER_PLG_EXPORTS
#define PB_FILETRANSFER_PLG_API __declspec(dllexport)
#else
#define PB_FILETRANSFER_PLG_API __declspec(dllimport)
#endif


// different calling conventions for CE vs. desktop
#ifndef	UNDER_CE
#define	CALLING_CONVENTION __cdecl
#else
#define	CALLING_CONVENTION __stdcall
#endif


/**
 * Exported function which is called to setup the FileTransfer module, and which
 * creates an instance of CInet if one does not exist, and then calls
 * its Preload member function.
 *
 * \param pPBStructure pointer to the PBStruct to be used
 * \param pPBCoreStructure pointer to the PBCoreStruct to be used
 * \return TRUE if module is successfully instantiated
 * \return FALSE if module cannot be instantiated and initialized
 */
PB_FILETRANSFER_PLG_API BOOL CALLING_CONVENTION FileTransfer_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure);


/**
 * Exported function which is called when the FileTransfer module needs to process 
 * a meta-tag.  Calls MetaProc in the current instance of CInet.
 *
 * \param pPBMetaStructure pointer to the PBMetaStructure to be used.  
 * PBMetaStructure contains the meta-tag information.
 * \param pPBStructure pointer to the PBStruct to be used
 * \return TRUE if meta-tag is processed without errors
 * \return FALSE if errors occur during meta-tag processing
 */
PB_FILETRANSFER_PLG_API BOOL CALLING_CONVENTION FileTransfer_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure);

/**
 * Exported function which is called when the FileTransfer module is to be shut down 
 * and detatched.  Calls the Dispose member function belonging to the current 
 * instance of CInet and then deletes the instance.
 *
 * \param pPBStructure pointer to the PBStruct to be used
 * \return TRUE if module is disposed without errors
 * \return FALSE if errors occur during dispose processing
 */
PB_FILETRANSFER_PLG_API BOOL CALLING_CONVENTION FileTransfer_Dispose(PPBSTRUCT pPBStructure);


#ifdef __cplusplus
}
#endif
