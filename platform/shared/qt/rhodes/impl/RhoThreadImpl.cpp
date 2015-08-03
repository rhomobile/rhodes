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

#include "common/RhoDefs.h"

#ifdef OS_WINDOWS_DESKTOP

#include "common/RhoPort.h"
#include "RhoThreadImpl.h"

using namespace rho::common;

IMPLEMENT_LOGCLASS(CRhoThreadImpl,"RhoThreadQT");

CRhoThreadImpl::CRhoThreadImpl(): m_Thread(0), m_waitThread(0) {}

CRhoThreadImpl::~CRhoThreadImpl()
{
    if (m_Thread) delete m_Thread;
    if (m_waitThread) delete m_waitThread;
}

void CRhoThreadImpl::start(IRhoRunnable* pRunnable, IRhoRunnable::EPriority ePriority)
{
    LOG(INFO) + "CRhoThreadImpl(QT) start";
    if (m_Thread)
        stop(0);
    m_Thread = new QRhoThread(pRunnable);
    m_Thread->start();
    setThreadPriority(ePriority);
}

void CRhoThreadImpl::setThreadPriority(IRhoRunnable::EPriority ePriority)
{
    QThread::Priority nPriority = QThread::NormalPriority;
    if ( ePriority == IRhoRunnable::epHigh )
        nPriority = QThread::HighestPriority;
    else if (ePriority == IRhoRunnable::epLow)
        nPriority = QThread::LowestPriority;
    m_Thread->setPriority(nPriority);
}

void CRhoThreadImpl::stop(unsigned int nTimeoutToKill)
{
    stopWait();
    if ( m_Thread ) {
        m_Thread->quit();
        //m_Thread->wait(nTimeoutToKill);
        if (!m_Thread->isRunning())
        {
           LOG(INFO) + "Terminate thread and wait after quit";
            m_Thread->terminate();
            m_Thread->wait();
        }
        else
        {
            LOG(INFO) + "Wait after quit";
            m_Thread->wait();
        }
        delete m_Thread;
        m_Thread = 0;
    }
}

int CRhoThreadImpl::wait(unsigned int nTimeoutMs)
{
    bool isVeyBigTimeoutvalue = false;
    
    if(nTimeoutMs == 4294966296)
	{
		isVeyBigTimeoutvalue = true;
	}
	else if(nTimeoutMs == 4294966295)
	{
		isVeyBigTimeoutvalue = true;
	}

    
    if (m_waitThread)
        stopWait();

    m_waitThread = new QThread();
    m_waitThread->start();

	bool result;
	
	if(isVeyBigTimeoutvalue)
	{
	result = m_waitThread->wait(1000UL*nTimeoutMs) ? 0 : 1;
	LOG(INFO) + "CRhoThreadImpl wait for a long time Result:-  "+result;
	}
	else
	{
	result = m_waitThread->wait(1UL*nTimeoutMs) ? 0 : 1;
	LOG(INFO) + "CRhoThreadImpl wait- Result:-  "+result;
	}

    delete m_waitThread;
    m_waitThread = 0;
    LOG(INFO) + "CRhoThreadImpl wait thread deleted";
    return result;
}

void CRhoThreadImpl::stopWait()
{
    QRhoThread::msleep(100);
    LOG(INFO) + "CRhoThreadImpl stopWait after 100 ms sleep";
    if (m_waitThread) 
    {
       LOG(INFO) + "CRhoThreadImpl stopWait-Terminate waitthread";
        m_waitThread->terminate(); // quit();
    }
}

void CRhoThreadImpl::sleep(unsigned int nTimeout)
{
    QRhoThread::sleep(nTimeout);
}

#endif // OS_WINDOWS_DESKTOP

extern "C" {

void* rho_nativethread_start()
{
    //TODO: rho_nativethread_start
    return 0;
}

void rho_nativethread_end(void* pData)
{
    //TODO: rho_nativethread_end
}

} //extern "C"
