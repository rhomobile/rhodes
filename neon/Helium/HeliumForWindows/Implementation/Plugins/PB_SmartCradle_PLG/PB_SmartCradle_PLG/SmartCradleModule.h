
#pragma once
#include "../../../common/public/pbplugin.h"
#include "SmartCradle.h"


class CSmartCradleModule :
	public PBModule
{
public:
	CSmartCradleModule(void);
	~CSmartCradleModule(void);
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, VOID* pParam);
	BOOL onAttachInstance(PBStruct *pPBStructure, PBInstStruct *pPBInstStruct);
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct);
	BOOL onInit(PPBSTRUCT pPBStructure);
	void onDeInit(PBStruct *pPBStructure);
	BOOL onBeforeNavigate(int iInstID);

};

