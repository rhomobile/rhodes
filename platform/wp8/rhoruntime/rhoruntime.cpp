// rhoruntime.cpp
#include "pch_rhoruntime.h"
#include "rhoruntime.h"

#include "../../shared/sqlite/sqlite3.h"

using namespace rhoruntime;
using namespace Platform;

CRhoRuntime::CRhoRuntime():
	m_WaitCallback(nullptr),
	m_UpdateWebViewCallback(nullptr),
	m_ExitCallback(nullptr)
{
	sqlite3_initialize();
}

// this callback is an example of calling the C# API methods from C++
void CRhoRuntime::SetWaitCallback(WaitCallbackPointer^ callback)
{
	m_WaitCallback = callback;
}

// this callback is an example of updating UI controls from C++ non-UI thread
void CRhoRuntime::SetUpdateWebViewCallback(UpdateWebViewCallbackPointer^ callback)
{
	m_UpdateWebViewCallback = callback;
}

// this callback is an example of application exit technique
void CRhoRuntime::SetExitCallback(ExitCallbackPointer^ callback)
{
	m_ExitCallback = callback;
}

// rhodes executed in a separate thread
void CRhoRuntime::Execute()
{
	// sample code just for testing
	for (int i=0; i<10; i++) {
		// wait for 1 second
		if ((i > 0) && m_WaitCallback)
			m_WaitCallback(1000);
		// update HTML code in WebView
		if (m_UpdateWebViewCallback)
			m_UpdateWebViewCallback(i);
	}
	// exit application
	if (m_ExitCallback)
		m_ExitCallback();
}