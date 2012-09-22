// rhoruntime.cpp
#include "pch.h"
#include "rhoruntime.h"

using namespace rhoruntime;
using namespace Platform;

CRhoRuntime::CRhoRuntime()
{
}

void CRhoRuntime::SetCallback(CallbackPointer^ loaded_callback)
{
	m_Callback = loaded_callback;
}

void CRhoRuntime::DoCallback()
{
	if (m_Callback)
		m_Callback();
}