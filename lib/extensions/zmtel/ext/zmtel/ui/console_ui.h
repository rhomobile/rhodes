#ifndef _CONSOLE_UI_H_INCLUDED
#define _CONSOLE_UI_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>

#include "ui.h"
#include "common/spirit_comtypes.h"
#include "../common/common.h"
#ifdef SPIRIT_OS_LINUX
  #include <signal.h>
  #include <pthread.h>
#endif

class tConsole_UI: public tUI 
{
public:
  tConsole_UI(FILE *pOutFile = stdout) : m_pOutFile(pOutFile) {
#ifdef SPIRIT_OS_LINUX
    _tcscpy(m_strUserInput,_T("Press Ctrl-C"));
    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGINT);
    sigaddset (&signal_mask, SIGTERM);
    int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);

#elif defined WINCE
    _tcscpy(m_strUserInput,_T("Please wait for about half a minute"));
#else
    _tcscpy(m_strUserInput,_T("Press any key and enter"));
#endif

  };
  virtual ~tConsole_UI() {}

  virtual void cPrintf(const char * format, ...) {
    va_list argptr;
    va_start(argptr, format);
    vfprintf(m_pOutFile,format,argptr);
    va_end(argptr);
  }
  virtual void Printf(const TCHAR * format, ...) {
    va_list argptr;
    va_start(argptr, format);
#ifdef UNICODE
    vfwprintf(m_pOutFile,format,argptr);
#else
    vfprintf(m_pOutFile,format,argptr);
#endif
    va_end(argptr);
  }
  virtual void WaitForUserInput() {
#ifdef SPIRIT_OS_LINUX
    int       sig_caught;    
    sigwait (&signal_mask, &sig_caught);
#elif defined WINCE
    Sleep(30000);
#else
    char sbuf[32];
    _tscanf(_T("%c"),sbuf);
#endif
  };
  virtual const TCHAR* UserInputString() {
    return m_strUserInput;
  };
private:
  FILE *m_pOutFile;
  TCHAR m_strUserInput[128];
#ifdef SPIRIT_OS_LINUX
  sigset_t   signal_mask;  /* signals to block         */
#endif

};

#endif // _CONSOLE_UI_H_INCLUDED
