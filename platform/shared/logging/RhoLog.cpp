#include "RhoLog.h"
#include "common/RhoFilePath.h"
#include "common/RhoTime.h"
#include "common/RhoSystem.h"

rho::LogCategory __rhoCurrentCategory;

const char*const LogSeverityNames[L_NUM_SEVERITIES] = {
  "TRACE", "INFO", "WARNING", "ERROR", "FATAL"
};

namespace rho {
general::CMutex LogMessage::m_FlushLock;

LogMessage::LogMessage(const char* file, int line, LogSeverity severity, LogSettings& oSettings, const LogCategory& category ) : 
    m_severity(severity), m_category(category), m_oLogConf(oSettings){
    if ( getLogConf().isLogPrefix() && isEnabled() )
        addPrefix( file, line );
}

bool LogMessage::isEnabled()const{
    if ( m_severity >= getLogConf().getMinSeverity() ){
        if ( m_category.isEmpty() || m_severity >= L_ERROR )
            return true;

        return getLogConf().isCategoryEnabled(m_category);
    }

    return false;
}

void LogMessage::addPrefix(const char* file, int line){
    //(log level, local date time, thread_id, file basename, line)
    //[I time f5d4fbb0 RhoLog.cpp:11]

    if ( m_severity == L_FATAL )
        *this + LogSeverityNames[m_severity];
    else
        *this + LogSeverityNames[m_severity][0];

    *this + " " + general::CLocalTime().toString() + ' ' +
        LOGFMT("%08x") + general::CSystem().getThreadID() + ' ';
    
    if ( m_category.isEmpty() )
        *this +  LOGFMT("%15s") + general::CFilePath(file).getBaseName() + ':' + LOGFMT("%4d") + line;
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
        else if ( m_strMessage.length() > 1 && m_strMessage[m_strMessage.length()-2] == '\r' )
            m_strMessage.erase(m_strMessage.length()-2,1);
    }


    {
        general::CMutexLock oLock(m_FlushLock);
        getLogConf().sinkLogMessage( m_strMessage.c_str(), m_strMessage.size() );
    }

    if ( m_severity == L_FATAL ) //TODO: may be save fatal info in separate place
        general::CRhoFatalError().processFatalError();
}

}
