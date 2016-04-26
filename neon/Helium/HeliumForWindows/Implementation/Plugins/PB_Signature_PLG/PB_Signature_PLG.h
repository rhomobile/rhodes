#ifdef __cplusplus
extern "C" {
#endif

#include "..\..\Common\Public\pbplugin.h"

#ifdef PB_SIGNATURE_PLG_EXPORTS
#define PB_SIGNATURE_PLG_API __declspec(dllexport)
#else
#define PB_SIGNATURE_PLG_API __declspec(dllimport)
#endif



// different calling conventions for CE vs. desktop
#ifndef	UNDER_CE
#define	CALLING_CONVENTION __cdecl
#else
#define	CALLING_CONVENTION __stdcall
#endif


// functions exported by this DLL

/**
 * Exported function which is called to setup the Signature module, and which
 * creates an instance of CSignature if one does not exist, and then calls
 * its Preload member function.
 *
 * \param pPBStructure pointer to the PBStruct to be used
 * \param pPBCoreStructure pointer to the PBCoreStruct to be used
 * \return TRUE if module is successfully instantiated
 * \return FALSE if module cannot be instantiated and initialized
 */
PB_SIGNATURE_PLG_API BOOL CALLING_CONVENTION SignatureCapture_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure);


/**
 * Exported function which is called when the Signature module needs to process 
 * a meta-tag.  Calls MetaProc in the current instance of CSignature.
 *
 * \param pPBMetaStructure pointer to the PBMetaStructure to be used.  
 * PBMetaStructure contains the meta-tag information.
 * \param pPBStructure pointer to the PBStruct to be used
 * \return TRUE if meta-tag is processed without errors
 * \return FALSE if errors occur during meta-tag processing
 */
PB_SIGNATURE_PLG_API BOOL CALLING_CONVENTION SignatureCapture_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure);


/**
 * Exported function which is called when the Signature module is to be shut down 
 * and detatched.  Calls the Dispose member function belonging to the current 
 * instance of CSignature and then deletes the instance.
 *
 * \param pPBStructure pointer to the PBStruct to be used
 * \return TRUE if module is disposed without errors
 * \return FALSE if errors occur during dispose processing
 */
PB_SIGNATURE_PLG_API BOOL CALLING_CONVENTION SignatureCapture_Dispose(PPBSTRUCT pPBStructure);



#ifdef __cplusplus
}
#endif
