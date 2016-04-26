#pragma once



class CMessageList
{

public:
	typedef struct Record{
		MSG			*pMsg;
		VOID		*pFunc;
		Record		*pNextRec;

	}*PTRREC;

	CMessageList(void);
	~CMessageList(void);

	BOOL	AddToList(MSG *pMsg);
	VOID	*Match(MSG *pMsg);
	long	*m_pMod;
private:
	void	DeleteList(PTRREC pRec);
	BOOL	addToList(MSG *pMsg,PTRREC pRec);
	VOID	*match(PTRREC pRec,MSG *pMsg);
	Record	m_Head;



};
