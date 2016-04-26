#pragma once

#pragma comment (lib,"../../../Common/Public/pbplugin.lib")

#include "stdafx.h"
#include "../../../Common/Private/emml.h"



class CEMMP
{
public:
	CEMMP(PBModule * pMod, int iTabID);
	~CEMMP(void);
	int Import(LPCTSTR lpProfilePage);
	void SetName(LPCTSTR lpProfilePageName);
	BOOL Delete(LPCTSTR lpProfilePageName);
	BOOL Apply(LPCTSTR lpProfilePageName);
	static int FileTransferredEvent(PVARSTRUCT pVars, int iTABID, LPARAM lParam);

	static LRESULT DoEMMLTagFunc( LPPBNAMEVALUE ppbnvpStruct, LPARAM lParam);

	HANDLE m_hTransfered;
	CEMML* m_pEMMLObj;
private:
	TCHAR *m_pProfilePageName;
	PBModule * m_pMod;
	

};
