/** 
* \file ConfigModule.h
*/

/******************************************************************************/
#pragma once

/******************************************************************************/
/** Macro giving count of elements in an array */
#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))

/** Macro to copy a wide string, guaranteed not to overrun the destination and always to be null terminated */
#define WSAFECOPY(d,s)	StringCchCopy(d,COUNTOF(d),s)

/** Macro to write to PBModule log automatically adding function name and line number */
#define LOG(s,m) Log(s, m,_T(__FUNCTION__), __LINE__)

/******************************************************************************/
#include "..\..\Common\Public\PBplugin.h"

/******************************************************************************/
/**
* Main Config plugin module class derived from PBModule.
*/
/******************************************************************************/
class CConfigModule: public PBModule
{
	/** \name Overrides of functions in PBModule base class */
	//@{
	//BOOL onInit(PPBSTRUCT pPBStructure);
	//void onDeInit(PPBSTRUCT pPBStructure);
	//BOOL onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
	//BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, void *pParam);
	//@}
};
