#ifndef _RHOLOG_H_
#define _RHOLOG_H_

#include "RhoLogConf.h"
#include "common/StringConverter.h"
#include "RhoLogCat.h"
#include "common/RhoFatalError.h"

namespace rho {

#if defined (RHO_DEBUG) && RHO_STRIP_LOG <= L_TRACE
#define COMPACT_RHO_LOGC_TRACE(category) rho::LogMessage(__FILE__, __LINE__, L_TRACE, LOGCONF(), category )

//#define TRACEC_IF(exp,category) !(exp) ? (void) 0 : rho::LogMessageVoidify() & TRACEC(category)
#define TRACE_IF(exp) !(exp) ? (void) 0 : rho::LogMessageVoidify() & COMPACT_RHO_LOGC_TRACE(__rhoCurrentCategory)

#else
#define COMPACT_RHO_LOGC_TRACE(category) rho::NullMessage()
#define TRACE_IF(exp) rho::NullMessage()
#endif

#if RHO_STRIP_LOG <= L_INFO
#define COMPACT_RHO_LOGC_INFO(category) rho::LogMessage(__FILE__, __LINE__, L_INFO, LOGCONF(), category )
#else
#define COMPACT_RHO_LOGC_INFO(category) rho::NullMessage()
#endif

#if RHO_STRIP_LOG <= L_WARNING
#define COMPACT_RHO_LOGC_WARNING(category) rho::LogMessage(__FILE__, __LINE__, L_WARNING, LOGCONF(), category )
#else
#define COMPACT_RHO_LOGC_WARNING(category) rho::NullMessage()
#endif

#if RHO_STRIP_LOG <= L_ERROR
#define COMPACT_RHO_LOGC_ERROR(category) rho::LogMessage(__FILE__, __LINE__, L_ERROR, LOGCONF(), category )
#else
#define COMPACT_RHO_LOGC_ERROR(category) rho::NullMessage()
#endif

#if RHO_STRIP_LOG <= L_FATAL
#define COMPACT_RHO_LOGC_FATAL(category) rho::LogMessage(__FILE__, __LINE__, L_FATAL, LOGCONF(), category )
#else
#define COMPACT_RHO_LOGC_FATAL(category) rho::NullMessageFatal()
#endif

#define LOG(severity) COMPACT_RHO_LOGC_ ## severity(__rhoCurrentCategory)
#define LOGC(severity,category) COMPACT_RHO_LOGC_ ## severity(category)

class LogMessage {
public:
    class LogFormat {
    public:
        String m_strFmt;
        LogFormat(const char* szFmt = ""){ m_strFmt = szFmt; }
    };
private:
    LogSeverity m_severity;
    LogCategory m_category;
    String m_strMessage;

    LogFormat m_lastFmt;

    LogSettings& m_oLogConf;
public:

    LogMessage(const char* file, int line, LogSeverity severity, LogSettings& oSettings, const LogCategory& category);

    ~LogMessage(){ flushLog(); }

    LogSeverity getSeverity()const{ return m_severity; }

    void addMessage(String strMesage){ m_strMessage+=strMesage; }

    const String& getLastFmt()const{ return m_lastFmt.m_strFmt; } 
    void  clearLastFmt(){ m_lastFmt.m_strFmt = String(); }
    void  setLastFmt(const LogFormat& fmt ){ m_lastFmt = fmt; }

    bool isEnabled()const;

    template<class T>
    inline LogMessage& operator+(const T &value) { 

        if ( isEnabled() )
        {
            if ( getLastFmt().length() > 0 )
            {
                char buf[100];
                sprintf(buf,getLastFmt().c_str(),value);
                clearLastFmt();
                addMessage(buf);
            }
            else
                addMessage( common::convertToStringA(value) );
        }

        return *this; 
    }

//    template<>
    inline LogMessage& operator+/*<LogMessage::LogFormat>*/(const LogMessage::LogFormat& value) { 
        setLastFmt( value );

        return *this;
    }

//    template<>
    inline LogMessage& operator+/*<String>*/(const String &value) { 

        return *this + value.c_str();
    }

private:
    void flushLog();
    void addPrefix(const char* file, int line);
    LogSettings& getLogConf()const{ return m_oLogConf; }
};

#define LOGFMT(fmt) rho::LogMessage::LogFormat(fmt)

class NullMessage {
public:
    NullMessage(){}

    template<class T>
    inline NullMessage& operator+(const T &value) { return *this; }

};

class NullMessageFatal {
public:
    NullMessageFatal(){}
    ~NullMessageFatal(){ common::CRhoFatalError().processFatalError(); }
};

class LogMessageVoidify {
 public:
  LogMessageVoidify() { }
  // This has to be an operator with a precedence lower than << but
  // higher than ?:
  void operator&(LogMessage&) { }
};

}

#endif //_RHOLOG_H_
