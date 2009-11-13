#ifndef _RHOLOG_H_
#define _RHOLOG_H_

#ifdef __cplusplus

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
    void addRawString(const char* data, int nLen){ m_strMessage.append(data, nLen); }

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
                int buflen = 99;
                char buf[100];
                int len = snprintf(buf,buflen,getLastFmt().c_str(),value);
                if (len < 0 || len >= buflen){
#ifdef OS_SYMBIAN
                    len = buflen - 1;
#else
                    len = buflen;
#endif
                }
                buf[len]=0;
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

    /*inline LogMessage& operator+(const char* value) 
    { 
        if (isEnabled())
        {
            m_strMessage+= value;
            clearLastFmt();
        }
        return *this;
    }*/
    
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
#else

#include "common/RhoPort.h"

#endif //__cplusplus


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

extern const char* _rawDefaultCategory;
#define DEFAULT_LOGCATEGORY _rawDefaultCategory 

void rhoPlainLogVar(const char* file, int line, LogSeverity severity, const char* szCategory,
                 const char* format, ... );
void rhoPlainLogArg(const char* file, int line, LogSeverity severity, const char* szCategory,
                 const char* format, va_list ap );
void rhoPlainLogArgW(const char* file, int line, int severity, const char* szCategory,
                 const wchar_t* format, va_list ap );

int rhoPlainLog(const char* file, int line, LogSeverity severity, const char* szCategory,
                  const char* msg );
int rhoPlainLogData(const char* file, int line, LogSeverity severity, const char* szCategory,
                    const void* data, int len );

#define RAWLOG_NONE ((void)0)

#if RHO_STRIP_LOG <= L_INFO
#define RAWLOGC_INFO(category,msg) rhoPlainLog(__FILE__, __LINE__, L_INFO, category, msg )
#define RAWLOGC_INFO1(category,msg,arg1) rhoPlainLogVar(__FILE__, __LINE__, L_INFO, category, msg, arg1 )
#define RAWLOGC_INFO2(category,msg,arg1,arg2) rhoPlainLogVar(__FILE__, __LINE__, L_INFO, category, msg, arg1,arg2 )
#define RAWLOGC_INFO3(category,msg,arg1,arg2,arg3) rhoPlainLogVar(__FILE__, __LINE__, L_INFO, category, msg, arg1,arg2,arg3 )
#define RAWLOGC_INFO4(category,msg,arg1,arg2,arg3,arg4) rhoPlainLogVar(__FILE__, __LINE__, L_INFO, category, msg, arg1,arg2,arg3,arg4 )
#define RAWLOGC_INFO6(category,msg,arg1,arg2,arg3,arg4,arg5,arg6) rhoPlainLogVar(__FILE__, __LINE__, L_INFO, category, msg, arg1,arg2,arg3,arg4,arg5,arg6 )

#define RAWLOG_INFO(msg) RAWLOGC_INFO( DEFAULT_LOGCATEGORY, msg )
#define RAWLOG_INFO1(msg,arg1) RAWLOGC_INFO1( DEFAULT_LOGCATEGORY, msg, arg1 )
#define RAWLOG_INFO2(msg,arg1,arg2) RAWLOGC_INFO2( DEFAULT_LOGCATEGORY, msg, arg1,arg2 )
#define RAWLOG_INFO3(msg,arg1,arg2,arg3) RAWLOGC_INFO3( DEFAULT_LOGCATEGORY, msg, arg1,arg2,arg3 )
#define RAWLOG_INFO4(msg,arg1,arg2,arg3,arg4) RAWLOGC_INFO4( DEFAULT_LOGCATEGORY, msg, arg1,arg2,arg3,arg4 )
#define RAWLOG_INFO6(msg,arg1,arg2,arg3,arg4,arg5,arg6) RAWLOGC_INFO6( DEFAULT_LOGCATEGORY, msg, arg1,arg2,arg3,arg4,arg5,arg6 )

#else
#define RAWLOGC_INFO(category,msg) RAWLOG_NONE
#define RAWLOGC_INFO1(category,msg,arg1) RAWLOG_NONE
#define RAWLOGC_INFO2(category,msg,arg1,arg2) RAWLOG_NONE
#define RAWLOGC_INFO3(category,msg,arg1,arg2,arg3) RAWLOG_NONE
#define RAWLOGC_INFO4(category,msg,arg1,arg2,arg3,arg4) RAWLOG_NONE
#define RAWLOGC_INFO6(msg,arg1,arg2,arg3,arg4,arg5,arg6) RAWLOG_NONE

#define RAWLOG_INFO(msg) RAWLOG_NONE
#define RAWLOG_INFO1(msg,arg1) RAWLOG_NONE
#define RAWLOG_INFO2(msg,arg1,arg2) RAWLOG_NONE
#define RAWLOG_INFO3(msg,arg1,arg2,arg3) RAWLOG_NONE
#define RAWLOG_INFO4(msg,arg1,arg2,arg3,arg4) RAWLOG_NONE
#define RAWLOG_INFO6(msg,arg1,arg2,arg3,arg4,arg5,arg6) RAWLOG_NONE
#endif

#if RHO_STRIP_LOG <= L_ERROR
#define RAWLOGC_ERROR(category,msg) rhoPlainLog(__FILE__, __LINE__, L_ERROR, category, msg )
#define RAWLOGC_ERROR1(category,msg,arg1) rhoPlainLogVar(__FILE__, __LINE__, L_ERROR, category, msg, arg1 )
#define RAWLOGC_ERROR2(category,msg,arg1,arg2) rhoPlainLogVar(__FILE__, __LINE__, L_ERROR, category, msg, arg1,arg2 )
#define RAWLOGC_ERROR3(category,msg,arg1,arg2,arg3) rhoPlainLogVar(__FILE__, __LINE__, L_ERROR, category, msg, arg1,arg2,arg3 )
#define RAWLOGC_ERROR4(category,msg,arg1,arg2,arg3,arg4) rhoPlainLogVar(__FILE__, __LINE__, L_ERROR, category, msg, arg1,arg2,arg3,arg4 )

#define RAWLOG_ERROR(msg) RAWLOGC_ERROR( DEFAULT_LOGCATEGORY, msg )
#define RAWLOG_ERROR1(msg,arg1) RAWLOGC_ERROR1( DEFAULT_LOGCATEGORY, msg, arg1 )
#define RAWLOG_ERROR2(msg,arg1,arg2) RAWLOGC_ERROR2( DEFAULT_LOGCATEGORY, msg, arg1,arg2 )
#define RAWLOG_ERROR3(msg,arg1,arg2,arg3) RAWLOGC_ERROR3( DEFAULT_LOGCATEGORY, msg, arg1,arg2,arg3 )
#define RAWLOG_ERROR4(msg,arg1,arg2,arg3,arg4) RAWLOGC_ERROR4( DEFAULT_LOGCATEGORY, msg, arg1,arg2,arg3,arg4 )
    
#else
#define RAWLOGC_ERROR(category,msg) RAWLOG_NONE
#define RAWLOGC_ERROR1(category,msg,arg1) RAWLOG_NONE
#define RAWLOGC_ERROR2(category,msg,arg1,arg2) RAWLOG_NONE
#define RAWLOGC_ERROR3(category,msg,arg1,arg2,arg3) RAWLOG_NONE
#define RAWLOGC_ERROR4(category,msg,arg1,arg2,arg3,arg4) RAWLOG_NONE

#define RAWLOG_ERROR(msg) RAWLOG_NONE
#define RAWLOG_ERROR1(msg,arg1) RAWLOG_NONE
#define RAWLOG_ERROR2(msg,arg1,arg2) RAWLOG_NONE
#define RAWLOG_ERROR3(msg,arg1,arg2,arg3) RAWLOG_NONE
#define RAWLOG_ERROR4(msg,arg1,arg2,arg3,arg4) RAWLOG_NONE
    
#endif

#if RHO_STRIP_LOG <= L_FATAL
#define RAWLOGC_FATAL(category,msg) rhoPlainLog(__FILE__, __LINE__, L_FATAL, category, msg )
#define RAWLOGC_FATAL1(category,msg,arg1) rhoPlainLogVar(__FILE__, __LINE__, L_FATAL, category, msg, arg1 )
#define RAWLOGC_FATAL2(category,msg,arg1,arg2) rhoPlainLogVar(__FILE__, __LINE__, L_FATAL, category, msg, arg1,arg2 )

#define RAWLOG_FATAL(msg) RAWLOGC_FATAL( DEFAULT_LOGCATEGORY, msg )
#define RAWLOG_FATAL1(msg,arg1) RAWLOGC_FATAL1( DEFAULT_LOGCATEGORY, msg, arg1 )
#define RAWLOG_FATAL2(msg,arg1,arg2) RAWLOGC_FATAL2( DEFAULT_LOGCATEGORY, msg, arg1,arg2 )
#else
#define RAWLOGC_FATAL(category,msg) RAWLOG_NONE
#define RAWLOGC_FATAL1(category,msg,arg1) RAWLOG_NONE
#define RAWLOGC_FATAL2(category,msg,arg1,arg2) RAWLOG_NONE

#define RAWLOG_FATAL(msg) RAWLOG_NONE
#define RAWLOG_FATAL1(msg,arg1) RAWLOG_NONE
#define RAWLOG_FATAL2(msg,arg1,arg2) RAWLOG_NONE
#endif

#if defined (RHO_DEBUG) && RHO_STRIP_LOG <= L_TRACE
#define RAWTRACEC(category, msg) rhoPlainLog(__FILE__, __LINE__, L_TRACE, category, msg )
#define RAWTRACEC1(category, msg,arg1) rhoPlainLogVar(__FILE__, __LINE__, L_TRACE, category, msg, arg1 )
#define RAWTRACEC2(category, msg,arg1,arg2) rhoPlainLogVar(__FILE__, __LINE__, L_TRACE, category, msg, arg1,arg2 )
#define RAWTRACEC3(category, msg,arg1,arg2,arg3) rhoPlainLogVar(__FILE__, __LINE__, L_TRACE, category, msg, arg1,arg2,arg3 )
#define RAWTRACEC4(category, msg,arg1,arg2,arg3,arg4) rhoPlainLogVar(__FILE__, __LINE__, L_TRACE, category, msg, arg1,arg2,arg3,arg4 )

#define RAWTRACE(msg) RAWTRACEC(DEFAULT_LOGCATEGORY,msg)
#define RAWTRACE1(msg,arg1) RAWTRACEC1(DEFAULT_LOGCATEGORY,msg,arg1)
#define RAWTRACE2(msg,arg1,arg2) RAWTRACEC2(DEFAULT_LOGCATEGORY,msg,arg1,arg2)
#define RAWTRACE3(msg,arg1,arg2,arg3) RAWTRACEC3(DEFAULT_LOGCATEGORY,msg,arg1,arg2,arg3)
#define RAWTRACE4(msg,arg1,arg2,arg3,arg4) RAWTRACEC4(DEFAULT_LOGCATEGORY,msg,arg1,arg2,arg3,arg4)

#define RAWTRACE_IF(exp,msg) !(exp) ? (void) 0 : RAWTRACE(msg)
#define RAWTRACE_IF1(exp,msg,arg1) !(exp) ? (void) 0 : RAWTRACE1(msg,arg1)

#define RAWTRACEC_DATA(category,data,len) rhoPlainLogData(__FILE__, __LINE__, L_TRACE, category, data, len)
#define RAWTRACE_DATA(data,len) RAWTRACEC_DATA(DEFAULT_LOGCATEGORY,data,len)

#else

#define RAWTRACEC(category,msg) RAWLOG_NONE
#define RAWTRACEC1(category, msg,arg1) RAWLOG_NONE
#define RAWTRACEC2(category, msg,arg1,arg2) RAWLOG_NONE
#define RAWTRACEC3(category, msg,arg1,arg2,arg3) RAWLOG_NONE
#define RAWTRACEC4(category, msg,arg1,arg2,arg3,arg4) RAWLOG_NONE

#define RAWTRACE(msg) RAWLOG_NONE
#define RAWTRACE1(msg,arg1) RAWLOG_NONE
#define RAWTRACE2(msg,arg1,arg2) RAWLOG_NONE
#define RAWTRACE3(msg,arg1,arg2,arg3) RAWLOG_NONE
#define RAWTRACE4(msg,arg1,arg2,arg3,arg4) RAWLOG_NONE

#define RAWTRACE_IF(exp,msg) RAWLOG_NONE
#define RAWTRACE_IF1(exp,msg,arg1) RAWLOG_NONE

#endif

#define RHO_ASSERT(exp) (void) ((exp) || rhoPlainLog(__FILE__, __LINE__, L_FATAL, "", #exp ))
//{ if (!exp) rhoPlainLog(__FILE__, __LINE__, L_FATAL, "", #exp ); }
#define RHO_ASSERTS(exp,msg) (void) ((exp) || rhoPlainLog(__FILE__, __LINE__, L_FATAL, "", msg ))
//{ if (!exp) rhoPlainLog(__FILE__, __LINE__, L_FATAL, "", msg ); }

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_RHOLOG_H_
