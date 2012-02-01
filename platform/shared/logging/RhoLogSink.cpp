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
#include "net/RawSocket.h"

#if defined( OS_SYMBIAN )
#include <e32debug.h>
#endif

namespace rho {

CLogFileSink::CLogFileSink(const LogSettings& oSettings)
    : m_pFile(0), m_pPosFile(0), m_oLogConf(oSettings), m_nCirclePos(-1), m_nFileLogSize(0)
{

}

void CLogFileSink::writeLogMessage( String& strMsg ){
    unsigned int len = strMsg.length();

    if ( !m_pFile )
        m_pFile = new common::CRhoFile();

    if ( !m_pFile->isOpened() ){
        m_pFile->open( getLogConf().getLogFilePath().c_str(), common::CRhoFile::OpenForAppend );
        m_nFileLogSize = m_pFile->size();
        loadLogPosition();
    }

    if ( getLogConf().getMaxLogFileSize() > 0 )
    {
        if ( ( m_nCirclePos >= 0 && m_nCirclePos + len > getLogConf().getMaxLogFileSize() ) || 
             ( m_nCirclePos < 0 && m_nFileLogSize + len > getLogConf().getMaxLogFileSize() ) )
        {
            m_pFile->movePosToStart();
            m_nFileLogSize = 0;
            m_nCirclePos = 0;
        }
    }

    int nWritten = m_pFile->write( strMsg.c_str(), len );
    m_pFile->flush();

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
    if ( m_pFile )    {
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

#if defined( OS_WINDOWS )
		::OutputDebugStringA(szMsg);
#elif defined( OS_PLATFORM_MOTCE )
		::OutputDebugStringW(common::convertToStringW(strMsg).c_str());
#elif defined( OS_SYMBIAN )
        TPtrC8 des((const TUint8*)szMsg);
      	RDebug::RawPrint(des);
        return;
#endif

#if !defined( OS_PLATFORM_MOTCE )
    for( int n = 0; n < (int)strMsg.length(); n+= 100 )
        fwrite(szMsg+n, 1, min(100,strMsg.length()-n) , stdout );

    fflush(stdout);
#endif
}

CLogSocketSink::CLogSocketSink(const LogSettings& oSettings) 
	: m_oLogConf(oSettings)
{
	m_aHttp = new net::CAsyncHttp(); 
	m_addrHost = "http://"+oSettings.getLogHost() + ":" + oSettings.getLogPort();
}

void CLogSocketSink::writeLogMessage( String& strMsg )
{
	if (m_aHttp)
    {
		rho_param* p = rho_param_hash(2);
		p->v.hash->name[0] = strdup("url");
		p->v.hash->value[0] = rho_param_str(const_cast<char*>(m_addrHost.c_str()));
		p->v.hash->name[1] = strdup("body");
		p->v.hash->value[1] = rho_param_str(const_cast<char*>(strMsg.c_str()));
		m_aHttp->addHttpCommand(new net::CAsyncHttp::CHttpCommand( "POST", p ));
		rho_param_free(p);
    }
}
        
}
