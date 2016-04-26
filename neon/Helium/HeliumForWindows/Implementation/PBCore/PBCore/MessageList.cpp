#include "StdAfx.h"
#include "MessageList.h"

CMessageList::CMessageList(void)
{
	m_Head.pMsg		= NULL;
	m_Head.pNextRec = NULL;

	m_pMod			= NULL;
}

CMessageList::~CMessageList(void)
{
	DeleteList(&m_Head);
}

void CMessageList::DeleteList(PTRREC pRec)
{
	//find the next available slot	
	if(pRec->pNextRec){
		DeleteList(pRec->pNextRec);	
	}
	delete pRec->pMsg;
	delete pRec->pNextRec;
	pRec->pMsg		= NULL;
	pRec->pNextRec	= NULL;

}

BOOL CMessageList::AddToList(MSG *pMsg)
{
	return addToList(pMsg,&m_Head);

}


BOOL CMessageList::addToList(MSG *pMsg,PTRREC pRec)
{
	//find the next available slot	
	if(pRec->pNextRec){
		return addToList(pMsg,pRec->pNextRec);	
	}
	//create a new record
	pRec->pMsg = new MSG;
	if(pRec->pMsg){
		memcpy(pRec->pMsg,pMsg,sizeof(MSG));
		pRec->pNextRec = new Record;		
		if(pRec->pNextRec){
			return TRUE;
		}
	}
	return FALSE;
}

//look for a match 
VOID *CMessageList::Match(MSG *pMsg)
{
	PTRREC pRec = &m_Head;
	
	return match(pRec,pMsg);


}

//private
//returns function pointer or NULL if no match found
VOID *CMessageList::match(PTRREC pRec,MSG *pMsg)
{
	MSG *pMsgRec = pRec->pMsg;	
	if(pRec->pMsg == NULL || pMsgRec == NULL || pRec->pFunc == NULL){//if there are no records return NULL
		return NULL;
	}

		
	if(pMsgRec->message == pMsg->message){
		//a match has been found
		if(pMsgRec->lParam){
			if(pMsgRec->lParam != pMsg->lParam){
				goto _next;
			}

		}	
		if(pMsgRec->wParam){
			if(pMsgRec->wParam != pMsg->wParam){
				goto _next;
			}	
		}
		//m_pMod = pMsgRec->pThis
		return pRec->pFunc;
	}
_next:	
	if(pRec->pNextRec){
		return match(pRec->pNextRec,pMsg);	//check the next record
	}		
	else{//no match found
		return NULL;
	}
	
	
}