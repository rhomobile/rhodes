#ifndef _RHOPLAINLOG_H_
#define _RHOPLAINLOG_H_

#include "common/RhoPort.h"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

typedef int LogSeverity;

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
#define RAWLOG_INFO4(msg,arg1,arg2,arg3,arg4) RAWLOGC_INFO3( DEFAULT_LOGCATEGORY, msg, arg1,arg2,arg3,arg4 )
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
#define RAWLOG_INFO3(msg,arg1,arg2,arg4) RAWLOG_NONE
#define RAWLOG_INFO6(msg,arg1,arg2,arg3,arg4,arg5,arg6) RAWLOG_NONE
#endif

#if RHO_STRIP_LOG <= L_ERROR
#define RAWLOGC_ERROR(category,msg) rhoPlainLog(__FILE__, __LINE__, L_ERROR, category, msg )
#define RAWLOGC_ERROR1(category,msg,arg1) rhoPlainLogVar(__FILE__, __LINE__, L_ERROR, category, msg, arg1 )
#define RAWLOGC_ERROR2(category,msg,arg1,arg2) rhoPlainLogVar(__FILE__, __LINE__, L_ERROR, category, msg, arg1,arg2 )
#define RAWLOGC_ERROR3(category,msg,arg1,arg2,arg3) rhoPlainLogVar(__FILE__, __LINE__, L_ERROR, category, msg, arg1,arg2,arg3 )

#define RAWLOG_ERROR(msg) RAWLOGC_ERROR( DEFAULT_LOGCATEGORY, msg )
#define RAWLOG_ERROR1(msg,arg1) RAWLOGC_ERROR1( DEFAULT_LOGCATEGORY, msg, arg1 )
#define RAWLOG_ERROR2(msg,arg1,arg2) RAWLOGC_ERROR2( DEFAULT_LOGCATEGORY, msg, arg1,arg2 )
#define RAWLOG_ERROR3(msg,arg1,arg2,arg3) RAWLOGC_ERROR3( DEFAULT_LOGCATEGORY, msg, arg1,arg2,arg3 )
#else
#define RAWLOGC_ERROR(category,msg) RAWLOG_NONE
#define RAWLOGC_ERROR1(category,msg,arg1) RAWLOG_NONE
#define RAWLOGC_ERROR2(category,msg,arg1,arg2) RAWLOG_NONE
#define RAWLOGC_ERROR3(category,msg,arg1,arg2,arg3) RAWLOG_NONE

#define RAWLOG_ERROR(msg) RAWLOG_NONE
#define RAWLOG_ERROR1(msg,arg1) RAWLOG_NONE
#define RAWLOG_ERROR2(msg,arg1,arg2) RAWLOG_NONE
#define RAWLOG_ERROR3(msg,arg1,arg2,arg3) RAWLOG_NONE
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

#define RAWTRACE(msg) RAWTRACEC(DEFAULT_LOGCATEGORY,msg)
#define RAWTRACE1(msg,arg1) RAWTRACEC1(DEFAULT_LOGCATEGORY,msg,arg1)
#define RAWTRACE2(msg,arg1,arg2) RAWTRACEC2(DEFAULT_LOGCATEGORY,msg,arg1,arg2)

#define RAWTRACE_IF(exp,msg) !(exp) ? (void) 0 : RAWTRACE(msg)
#define RAWTRACE_IF1(exp,msg,arg1) !(exp) ? (void) 0 : RAWTRACE1(msg,arg1)

#else

#define RAWTRACEC(category,msg) RAWLOG_NONE
#define RAWTRACEC1(category, msg,arg1) RAWLOG_NONE
#define RAWTRACEC2(category, msg,arg1,arg2) RAWLOG_NONE

#define RAWTRACE(msg) RAWLOG_NONE
#define RAWTRACE1(msg,arg1) RAWLOG_NONE
#define RAWTRACE2(msg,arg1,arg2) RAWLOG_NONE

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

#endif //_RHOPLAINLOG_H_