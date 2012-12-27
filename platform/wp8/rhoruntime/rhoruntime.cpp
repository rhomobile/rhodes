/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "pch_rhoruntime.h"
#include "rhoruntime.h"

//#include "../../shared/sqlite/sqlite3.h"
//#include "logging/RhoLogConf.h"
#include "common/RhodesApp.h"
#include "rubyext/WebView.h"

using namespace rhoruntime;
using namespace Platform;

CRhoRuntime^ CRhoRuntime::m_instance = nullptr;

CRhoRuntime::CRhoRuntime(IMainPage^ mainPage):
	m_MainPage(mainPage)
{
}

CRhoRuntime^ CRhoRuntime::getInstance(IMainPage^ mainPage)
{
	if (m_instance == nullptr)
		m_instance = ref new CRhoRuntime(mainPage);
    return m_instance;
}

CRhoRuntime^ CRhoRuntime::getInstance()
{
    return m_instance;
}

IMainPage^ CRhoRuntime::getMainPage()
{
	return m_MainPage;
}

// rhodes executed in a separate thread
void CRhoRuntime::Execute()
{
	rho::String m_strRootPath = rho_native_rhopath(), m_logPort, m_strRuntimePath;
    rho_logconf_Init(m_strRootPath.c_str(), m_strRootPath.c_str(), m_logPort.c_str());

    rho::common::CRhodesApp::Create(m_strRootPath, m_strRootPath, m_strRuntimePath);
    //RHODESAPP().setExtManager( &m_oExtManager );

	//Create Main window

	RHODESAPP().startApp();

	//// wait for 5 seconds
	//m_MainPage->DoWait(5000);
	//// exit application
	//m_MainPage->exitCommand();
}


// *** CALLBACKS from MainPage object ***

void CRhoRuntime::updateSizeProperties(int width, int height)
{
}

void CRhoRuntime::onActivate(int active)
{
	RHODESAPP().callUiCreatedCallback();
}

void CRhoRuntime::logEvent(::Platform::String^ message)
{
}

void CRhoRuntime::createCustomMenu(void)
{
}

void CRhoRuntime::onCustomMenuItemCommand(int nItemPos)
{
}

void CRhoRuntime::onWindowClose(void)
{
}

void CRhoRuntime::onWebViewUrlChanged(::Platform::String^ url)
{
}


// *** PUBLIC METHODS ***

bool CRhoRuntime::Initialize(::Platform::String^ title)
{
	return true;
}

void CRhoRuntime::DestroyUi(void)
{
}
