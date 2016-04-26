/**
 * \brief Implementation of the interfaces Comm_Preload, Comm_MetaProc,
 * and Comm_Dispose.  These routines call functions in the CCommModule class.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../Common/Public/pbplugin.h"

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. 
#ifdef PB_COMM_PLG_EXPORTS
#define PB_COMM_PLG_API __declspec(dllexport)
#else
#define PB_COMM_PLG_API __declspec(dllimport)
#endif

// different calling conventions for CE vs. desktop
#ifndef	UNDER_CE
#define	CALLING_CONVENTION __cdecl
#else
#define	CALLING_CONVENTION __stdcall
#endif

// functions exported by this DLL
/**
 * Exported function which is called to setup the Comm module, and which
 * creates an instance of CCommModule if one does not exist, and then calls
 * its Preload member function.
 *
 * \return TRUE if module is successfully instantiated
 * \return FALSE if module cannot be instantiated and initialized
 */
PB_COMM_PLG_API BOOL CALLING_CONVENTION Comm_Preload (PBStruct     *pPBStructure,
													  PBCoreStruct *pPBCoreStructure);
/**
 * Exported function which is called when the Comm module needs to process 
 * a meta-tag.  Calls MetaProc in the current instance of CCommModule.
 *
 * \return TRUE if meta-tag is processed without errors
 * \return FALSE if errors occur during meta-tag processing
 */
PB_COMM_PLG_API BOOL CALLING_CONVENTION Comm_MetaProc(PBMetaStruct *pPBMetaStructure,
													  PBStruct     *pPBStructure);
/**
 * Exported function which is called when the Comm module is to be shut down 
 * and detatched.  Calls the Dispose member function belonging to the current 
 * instance of CCommModule and then deletes the instance.
 *
 * \return TRUE if module is disposed without errors
 * \return FALSE if errors occur during dispose processing
 */
PB_COMM_PLG_API BOOL CALLING_CONVENTION Comm_Dispose (PBStruct     *pPBStructure);

#ifdef __cplusplus
}
#endif
