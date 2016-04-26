

#ifdef __cplusplus 
extern "C" 
{
#endif


#include "../../../Common/Public/pbplugin.h"

// defined with this macro as being exported.
#ifdef PB_EMMP_PLG_EXPORTS
#define PB_EMMP_PLG_API __declspec(dllexport)
#else
#define PB_EMMP_PLG_API __declspec(dllimport)
#endif

// different calling conventions for CE vs. desktop 
#ifndef UNDER_CE 
#define CALLING_CONVENTION __cdecl 
#else 
#define CALLING_CONVENTION __stdcall 
#endif 

__declspec(dllexport) BOOL CALLING_CONVENTION EMMLProfile_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure);
__declspec(dllexport) BOOL CALLING_CONVENTION EMMLProfile_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure);
__declspec(dllexport) BOOL CALLING_CONVENTION EMMLProfile_Dispose(PPBSTRUCT pPBStructure);


#ifdef __cplusplus
}
#endif