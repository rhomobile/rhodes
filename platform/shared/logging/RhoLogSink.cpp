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

#include "RhoLogSink.h"

#include "common/RhoFile.h"
#include "common/StringConverter.h"
#include "common/RhodesApp.h"
#include "net/RawSocket.h"
#include "net/URI.h"
#ifdef OS_SAILFISH
#include <QDebug>
#include <QString>
#endif

#if defined( OS_SYMBIAN )
#include <e32debug.h>
#endif


#if defined(OS_MACOSX) && !defined(RHODES_EMULATOR)

extern "C" void rho_ios_log_console_output(const char* message);

#endif


namespace rho {

CLogFileSink::CLogFileSink(const LogSettings& oSettings)
    : m_pFile(0), m_pPosFile(0), m_oLogConf(oSettings), m_nCirclePos(-1), m_nFileLogSize(0)
{

}

CLogFileSink::~CLogFileSink()
{
    if(m_pFile){
        if (m_pFile->isOpened()){
            m_pFile->flush();
        }
        delete m_pFile;
    }
}


void CLogFileSink::writeLogMessage( String& strMsg ){
    unsigned int len = strMsg.length();

    if ( m_pFile ){}else{m_pFile = new common::CRhoFile();}

    if ( m_pFile->isOpened() ){}else{
        m_pFile->open( getLogConf().getLogFilePath().c_str(), common::CRhoFile::OpenForAppend );
        m_nFileLogSize = m_pFile->size();
        loadLogPosition();
    }

    if ( getLogConf().getMaxLogFileSize() > 0 )
    {
        if ( ( m_nCirclePos >= 0 && m_nCirclePos + len > getLogConf().getMaxLogFileSize() ) || 
             ( m_nCirclePos < 0 && m_nFileLogSize + len > getLogConf().getMaxLogFileSize() ) )
        {
            m_pFile->truncate(m_pFile->getPos());
            m_pFile->movePosToStart();
            m_nFileLogSize = 0;
            m_nCirclePos = 0;
        }
    }

    int nWritten = m_pFile->write( strMsg.c_str(), len );
#ifdef OS_WINDOWS_DESKTOP
    static int writeCounter = 0;
    static const bool isMinsNull = LOGCONF().getMinSeverity() == L_TRACE;
    if (isMinsNull){
        m_pFile->flush();
    }else{
        if ((++writeCounter % 16) == 0) m_pFile->flush();
    }
#else
    m_pFile->flush();
#endif

    if ( m_nCirclePos >= 0 )
        m_nCirclePos += nWritten;
    else
        m_nFileLogSize += nWritten;

    saveLogPosition();
}

int CLogFileSink::getCurPos()
{
    return m_nCirclePos >= 0 ? m_nCirclePos : m_nFileLogSize;
}

void CLogFileSink::clear(){
    if ( m_pFile )
    {
        if (m_pFile->isOpened()){
            m_pFile->flush();
        }
        delete m_pFile;
        m_pFile = NULL;
    }

    common::CRhoFile().deleteFile(getLogConf().getLogFilePath().c_str());
    String strPosPath = getLogConf().getLogFilePath() + "_pos";
    common::CRhoFile().deleteFile(strPosPath.c_str());
}

void CLogFileSink::loadLogPosition(){
    if ( !m_pPosFile )
        m_pPosFile = new common::CRhoFile();

    if ( !m_pPosFile->isOpened() ){
        String strPosPath = getLogConf().getLogFilePath() + "_pos";
        m_pPosFile->open( strPosPath.c_str(), common::CRhoFile::OpenForReadWrite );
    }

    if ( !m_pPosFile->isOpened() )
        return;

    String strPos;
    m_pPosFile->movePosToStart();
    m_pPosFile->readString(strPos);
    if ( strPos.length() == 0 )
        return;

    m_nCirclePos = atoi(strPos.c_str());
    if ( m_nCirclePos < 0 || m_nCirclePos > (int)m_nFileLogSize )
    	m_nCirclePos = -1;
    
    if ( m_nCirclePos >= 0 )
        m_pFile->setPosTo( m_nCirclePos );
}

void CLogFileSink::saveLogPosition(){
    if ( m_nCirclePos < 0 )
        return;

    if ( m_nCirclePos > (int)getLogConf().getMaxLogFileSize() )
    	return;
    
    String strPos = common::convertToStringA(m_nCirclePos);
    for( int i = strPos.length(); i < 10; i++ )
    	strPos += ' ';
    
    m_pPosFile->movePosToStart();
    m_pPosFile->write( strPos.c_str(), strPos.length() );
    m_pPosFile->flush();
}

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

void CLogOutputSink::writeLogMessage( String& strMsg )
{
    if ( strMsg.length() > 1 && strMsg[strMsg.length()-2] == '\r' )
        strMsg.erase(strMsg.length()-2,1);

    const char* szMsg = strMsg.c_str(); 

#if defined( OS_WINDOWS_DESKTOP )
		::OutputDebugStringA(szMsg);
#elif defined( OS_PLATFORM_MOTCE )
		::OutputDebugStringW(common::convertToStringW(strMsg).c_str());
#elif defined(OS_WP8) || defined(OS_UWP)
		::OutputDebugStringW(common::convertToStringW(strMsg).c_str());
#elif defined( OS_SYMBIAN )
        TPtrC8 des((const TUint8*)szMsg);
      	RDebug::RawPrint(des);
        return;
#elif defined(OS_MACOSX) && !defined(RHODES_EMULATOR)
        rho_ios_log_console_output(szMsg);
#elif defined(OS_SAILFISH)
        //qDebug() << QString::fromStdString(strMsg);
#endif

#if !defined( OS_PLATFORM_MOTCE ) && !(defined(OS_MACOSX) && !defined(RHODES_EMULATOR))
    for( int n = 0; n < (int)strMsg.length(); n+= 100 )
        fwrite(szMsg+n, 1, min(100,strMsg.length()-n) , stdout );

    fflush(stdout);
#endif
}

CLogSocketSink::CLogSocketSink(const LogSettings& oSettings)
{
	m_URL = oSettings.getLogURL();

    CThreadQueue::setLogCategory(LogCategory("NO_LOGGING"));
    setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);
#if !defined( OS_WINDOWS_DESKTOP )
    start(epLow);
#endif
}

CLogSocketSink::~CLogSocketSink()
{
	//wait till all commands will be sent to server
#if !defined( OS_WINDOWS_DESKTOP )    
    CRhoThread::stop(2000);
#endif    
}

void CLogSocketSink::setUrl(String url)
{
	m_URL = url;
}

void CLogSocketSink::writeLogMessage( String& strMsg )
{
	addQueueCommand(new LogCommand(m_URL.c_str(), strMsg.c_str()));
}

void CLogSocketSink::processCommand(IQueueCommand* pCmd)
{
    LogCommand *cmd = (LogCommand *)pCmd;
  
    /*
      Checking CRhodesApp instance is needed because net request requires it to be valid.
      If socket sink is initialized from ext manager, log messages could occur before app instance is set so we will crash.
    */     
    if (!cmd || (common::CRhodesApp::getInstance()==0))
        return;
#if defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME)

	getNet().doRequest( "GET", cmd->m_url+"?LogSeverity="+cmd->m_body[0]+"&LogComment="+net::URI::urlEncode(cmd->m_body), String(), 0, 0 );
#else
	getNet().doRequest( "POST", cmd->m_url, cmd->m_body, 0, 0 );
#endif
}
        
}
