#pragma once

#define LOCKFUNCTION Lock LocalLock(&m_CriticalSect)

class CCriticalSect
{
public:
	CCriticalSect(void);
	~CCriticalSect(void);
	void Enter(){EnterCriticalSection(&m_CritSect);};
	void Leave(){LeaveCriticalSection(&m_CritSect);};
private:
	CRITICAL_SECTION m_CritSect;
};


class Lock
{
public:
	Lock(CCriticalSect *pCritSect);
	~Lock(void);

private:
	CCriticalSect *m_pCritSect;

};

class Lockable
{
public:
	Lockable(void);
	~Lockable(void);
	void LockObj(){m_CriticalSect.Enter();};
	void UnlockObj(){m_CriticalSect.Leave();};
	CCriticalSect *GetCriticalSect(){return &m_CriticalSect;};
protected:
	CCriticalSect m_CriticalSect;
};
