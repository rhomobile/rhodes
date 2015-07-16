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

IMPLEMENT_LOGCLASS(CRhoThreadImpl,"RhoThread");

CRhoThreadImpl::CRhoThreadImpl(): m_Thread(0), m_waitThread(0) {}

CRhoThreadImpl::~CRhoThreadImpl()
{
    if (m_Thread) delete m_Thread;
    if (m_waitThread) delete m_waitThread;
}

void CRhoThreadImpl::start(IRhoRunnable* pRunnable, IRhoRunnable::EPriority ePriority)
{
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
        if (!m_Thread->isRunning()) {
            m_Thread->terminate();
            m_Thread->wait();
        }
        LOG(INFO) + "qt-Terminate thread.";
        delete m_Thread;
        m_Thread = 0;
    }
}

int CRhoThreadImpl::wait(unsigned int nTimeoutMs)
{
    RAWTRACE1("qt-CRhoThreadImpl wait : %d",nTimeoutMs);
    if (m_waitThread)
    {
            RAWTRACE("qt-inside m_waitThread") ;
            stopWait();
            RAWTRACE( "qt-stopWait called");
    }
    m_waitThread = new QThread();
    m_waitThread->start();
    RAWTRACE1("qt-before m_waitThread->wait : isRunning() :  %d",m_waitThread->isRunning());
    RAWTRACE1("qt-before m_waitThread->wait : isFinished():  %d",m_waitThread->isFinished());
	bool result = m_waitThread->wait(nTimeoutMs) ? 0 : 1;
    RAWTRACE1("(ii)qt-after m_waitThread->wait : isRunning() : %d ", m_waitThread->isRunning());
    RAWTRACE1("(ii)qt-after m_waitThread->wait : isFinished(): %d ",  m_waitThread->isFinished());
    
    RAWTRACE1("qt-after m_waitThread->wait : %d", result);
    if(m_waitThread)
        delete m_waitThread;
    RAWTRACE("m_waitThread: destroyed");
    m_waitThread = 0;
    return result;
}

void CRhoThreadImpl::stopWait()
{
    RAWTRACE( "qt-CRhoThreadImpl::stopWait Entry" );
    if (m_waitThread) 
    {
        RAWTRACE("qt-CRhoThreadImpl::stopWait if m_waitThread" );
        m_waitThread->terminate(); // quit();
       RAWTRACE("qt-CRhoThreadImpl::stopWait if m_waitThread Terminated") ;
    }
     if (m_waitThread)
     {
            RAWTRACE("qt-CRhoThreadImpl::stopWait if m_waitThread for wait call" );
            RAWTRACE1("qt-CRhoThreadImpl::stopWait if m_waitThread for wait call : null check : %d" ,(m_waitThread!=NULL));
            
            try
            {
                if(m_waitThread!=NULL)
                    m_waitThread->wait();
                RAWTRACE("qt-CRhoThreadImpl::stopWait if m_waitThread for wait call completed in try") ;
            }
            catch (std::exception &e)
            {
                RAWTRACE1("qt-CRhoThreadImpl::stopWait if m_waitThread for wait call thrown exception %s",e.what()) ;
            }
            RAWTRACE("qt-CRhoThreadImpl::stopWait if m_waitThread for wait call completed") ;
     }
   RAWTRACE( "qt-CRhoThreadImpl::stopWait Exit" );
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
