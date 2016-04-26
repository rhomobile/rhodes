// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PB_IMAGECAPTURE_PLG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PB_IMAGECAPTURE_PLG_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#include "../../common/public/pbplugin.h"

#ifdef PB_IMAGECAPTURE_PLG_EXPORTS
#define PB_IMAGECAPTURE_PLG_API __declspec(dllexport)
#else
#define PB_IMAGECAPTURE_PLG_API __declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif

	//  Different calling conventions are required when exporting functions on the
	//  device and on the desktop
	#ifndef UNDER_CE
	#define CALLING_CONVENTION __cdecl
	#else
	#define CALLING_CONVENTION __stdcall
	#endif


PB_IMAGECAPTURE_PLG_API BOOL CALLING_CONVENTION Imager_Preload(PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure);
PB_IMAGECAPTURE_PLG_API BOOL CALLING_CONVENTION Imager_MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure);
PB_IMAGECAPTURE_PLG_API BOOL CALLING_CONVENTION Imager_Dispose(PPBSTRUCT pPBStructure);
#ifdef __cplusplus
}
#endif
