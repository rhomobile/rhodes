#pragma once
#include "stdafx.h"

class CEMMPMod :
	public PBModule
{
public:
	CEMMPMod(void);
	~CEMMPMod(void);

	BOOL onInit(PPBSTRUCT pPBStructure);
	BOOL onDeInit(PBSTRUCT pPBStructure);
	BOOL onAttachInstance(PPBSTRUCT pPBStructure , PPBINSTSTRUCT pInstStruct);
	BOOL onReleaseInstance(PPBSTRUCT pPBStructure , PPBINSTSTRUCT pInstStruct);
	BOOL MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure,VOID* pParam);
	BOOL onBeforeNavigate(int iInstID);
	IMOREF m_FileTransferIMO;

};


