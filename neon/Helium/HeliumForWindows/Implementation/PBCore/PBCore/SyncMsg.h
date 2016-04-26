#pragma once
#include "..\..\Common\Public\PBplugin.h"


//one for each registered module
struct plgCB{
	PBModule	*pObj;			//pointer to the module object
	//PBModule	*pObj;			//pointer to the module object
	plgCB	*pNext;			//pointer to the structure in the linked list
	LPCTSTR		pModName;		//pointer to the name of the module
};

class CSyncMsg
{
public:
	CSyncMsg(void);
	~CSyncMsg(void);
	BOOL	RegisterCallback(PBModule *pObj,LPCTSTR pCallingModule);
	BOOL	UnRegisterCallback(PBModule	*pObj);
	plgCB	*deletemem(plgCB *pCallbackRec);
	BOOL	RunSync(LPARAM lParam1,LPARAM lParam2);
private:
	plgCB *m_pCB;

};
