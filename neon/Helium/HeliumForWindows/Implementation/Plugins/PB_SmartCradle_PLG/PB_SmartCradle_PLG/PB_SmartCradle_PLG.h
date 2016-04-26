//// This class is exported from the PB_SmartCradle_PLG.dll
//class PB_SMARTCRADLE_PLG_API CPB_SmartCradle_PLG {
//public:
//	CPB_SmartCradle_PLG(void);
//	// TODO: add your methods here.
//};
//
//extern PB_SMARTCRADLE_PLG_API int nPB_SmartCradle_PLG;
//
//PB_SMARTCRADLE_PLG_API int fnPB_SmartCradle_PLG(void);




#pragma once
#include "../../../common/public/pbplugin.h"

#ifdef PB_SMARTCRADLE_PLG_EXPORTS
#define PB_SMARTCRADLE_PLG_API __declspec(dllexport)
#else
#define PB_SMARTCRADLE_PLG_API __declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif



	#ifndef UNDER_CE
	#define CALLING_CONVENTION __cdecl
	#else
	#define CALLING_CONVENTION __stdcall
	#endif



PB_SMARTCRADLE_PLG_API BOOL CALLING_CONVENTION SmartCradle_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure);
PB_SMARTCRADLE_PLG_API BOOL CALLING_CONVENTION SmartCradle_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure);
PB_SMARTCRADLE_PLG_API BOOL CALLING_CONVENTION SmartCradle_Dispose(PPBSTRUCT pPBStructure);
#ifdef __cplusplus
}
#endif

