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

#include "RhoLog.h"
#include "common/RhoFilePath.h"
#include "common/RhoTime.h"
#include "common/RhoSystem.h"

rho::LogCategory __rhoCurrentCategory;
unsigned int  __logThreadId = 0;

const char*const LogSeverityNames[L_NUM_SEVERITIES] = {
  "TRACE", "INFO", "WARNING", "ERROR", "FATAL"
};

namespace rho {

LogMessage::LogMessage(const char* file, int line, LogSeverity severity, LogSettings& oSettings, const LogCategory& category ) : 
    m_severity(severity), m_category(category), m_oLogConf(oSettings){
    if ( getLogConf().isLogPrefix() && isEnabled() )
        addPrefix( file, line );
}

bool LogMessage::isEnabled()const{
	if(__logThreadId == common::CSystem::getThreadID()) return false;
	
	if ( m_severity >= getLogConf().getMinSeverity() ){
        if ( m_category.isEmpty() || m_severity >= L_ERROR )
            return true;

        return getLogConf().isCategoryEnabled(m_category);
    }

    return false;
}

void LogMessage::addPrefix(const char* file, int line){
    //(log level, local date time, thread_id, file basename, line)
    //I time f5d4fbb0 RhoLog.cpp:11|

    if ( m_severity == L_FATAL )
        *this + LogSeverityNames[m_severity];
    else
        *this + LogSeverityNames[m_severity][0];

    *this + " " + common::CLocalTime().toString(true) + ' ' +
        LOGFMT("%08x") + common::CSystem().getThreadID() + ' ';
    
    if ( m_category.isEmpty() )
        *this +  LOGFMT("%15s") + common::CFilePath(file).getBaseName() + ':' + LOGFMT("%4d") + line;
    else
        *this + LOGFMT("%20s") + m_category.getName();

    *this  + "| ";
}

void LogMessage::flushLog(){
    if ( !isEnabled() )
        return;

    if ( m_strMessage.length() > 0 ){
        if ( m_strMessage[m_strMessage.length()-1] != '\n' )
            m_strMessage += LOG_NEWLINE;
        //else if ( m_strMessage.length() > 1 && m_strMessage[m_strMessage.length()-2] == '\r' )
        //    m_strMessage.erase(m_strMessage.length()-2,1);
    }

    getLogConf().sinkLogMessage( m_strMessage );

    if ( m_severity == L_FATAL ) //TODO: may be save fatal info in separate place
        common::CRhoFatalError().processFatalError();
}

void LogMessage::applyExcludeFilter(String& strMsg)
{
    Vector<String>& arSecure = LOGCONF().getExcludeAttribs();
    if ( arSecure.size() == 0 )
        return;

    for ( int i = 0; i < strMsg.length(); i++ )
    {
        rho::boolean bFound = false;
        for ( int j = 0; j < arSecure.size(); j++ )
        {
            const String& strExclude = arSecure.elementAt(j);
            if ( strncmp(strMsg.c_str() + i, strExclude.c_str(), strExclude.length()) == 0 )
            {
                rho::boolean bSlash = false;
                int nRemoveStart = i + strExclude.length(); 
                int nEndSep = '"';

                if ( strncmp(strMsg.c_str() + nRemoveStart, "\":\"", 3) == 0 )
                    nRemoveStart += 3;
                else if ( strncmp(strMsg.c_str() + nRemoveStart, "\"=>\"", 4) == 0 )
                    nRemoveStart += 4;
                else if ( strncmp(strMsg.c_str() + nRemoveStart, "=", 1) == 0 )
                {
                    nRemoveStart += 1;
                    nEndSep = '&';
                }
                else
                    break;

                int nFill = nRemoveStart;
                for ( ; nFill < strMsg.length(); nFill++ )
                {
                    if ( bSlash && strMsg[nFill] == '\\' ) 
                    {
                        bSlash = false;
                        continue;
                    }
                    else if ( nEndSep != '&' && strMsg[nFill] == '\\' )
                        bSlash = true;
                    else
                    {
                        if ( strMsg[nFill] == nEndSep && !bSlash )
                        {
                            bFound = true;
                            break;
                        }

                        bSlash = false;
                    }
                }
                if ( bFound || nEndSep == '&')
                {
                    strMsg.erase(nRemoveStart, nFill-nRemoveStart );
                    i += strExclude.length();
                }
            }
            if ( bFound)
                break;
        }
    }

}

}
