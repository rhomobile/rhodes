/**
 * \file PB_CardReader_PLG.h
 * \brief Definition of the card reader (CardReader) plugin methods which constitute the PB 3.0
 * plugin API's.
 */

#ifdef __cplusplus
extern "C" {
#endif
#include "../../Common/Public/pbplugin.h"

#define NO_APP		-1

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PB_CardReader_PLG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PB_CardReader_PLG_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PB_CARDREADER_PLG_EXPORTS
#define PB_CARDREADER_PLG_API __declspec(dllexport)
#else
#define PB_CARDREADER_PLG_API __declspec(dllimport)
#endif

//  Different calling conventions are required when exporting functions on the
//  device and on the desktop
#ifndef UNDER_CE
#define CALLING_CONVENTION __cdecl
#else
#define CALLING_CONVENTION __stdcall
#endif

/**
 * Creates an instance of a CCardReader object to hold the state
 * of the card reader, set its type, and calls CCardReader's Preload.
 * \param pPBStructure  data used to define Browser environment
 * \param pPBCoreStructure  data used to to provide implementaion for built in function, which ultimately post back to the core
 * \return FALSE if it cannot instantiate the CCardReader object or the generic CardReader Preload fails.
 * \return TRUE otherwise.
 */
PB_CARDREADER_PLG_API BOOL CALLING_CONVENTION CardReader_Preload(PBStruct *pPBStructure, PBCoreStruct *pPBCoreStructure);

/**
 * If the pointer to the CCardReader object for the card reader is non-NULL, calls CCardReader's MetaProc function.
 * \param pPBMetaStructure  contains the parameter and values belonging to the meta tag
 * \param pPBStructure  data used to define Browser environment
 * \return FALSE, if pointer to CCardReader object is NULL.
 * \return TRUE otherwise.
 */
PB_CARDREADER_PLG_API BOOL CALLING_CONVENTION CardReader_MetaProc(PBMetaStruct *pPBMetaStructure, PBStruct *pPBStructure);

/**
 * Calls the generic CardReader dispose, then unallocates the CCardReader object and sets the pCardReader pointer to NULL.
 * \param pPBStructure  data used to define Browser environment
 * \return FALSE if the generic dispose returns FALSE.
 * \return TRUE otherwise.
 */
PB_CARDREADER_PLG_API BOOL CALLING_CONVENTION CardReader_Dispose(PBStruct *pPBStructure);

#ifdef __cplusplus
}
#endif
