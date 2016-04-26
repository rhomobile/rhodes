#include "StdAfx.h"
#include "CriticalSect.h"

CCriticalSect::CCriticalSect(void)
{
	InitializeCriticalSection(&m_CritSect);
}

CCriticalSect::~CCriticalSect(void)
{
	DeleteCriticalSection(&m_CritSect);
}


Lock::Lock(CCriticalSect *pCritSect)
{
	m_pCritSect = pCritSect;
	m_pCritSect->Enter();
}

Lock::~Lock()
{
	m_pCritSect->Leave();
}



Lockable::Lockable(void)
{
}

Lockable::~Lockable(void)
{
}
