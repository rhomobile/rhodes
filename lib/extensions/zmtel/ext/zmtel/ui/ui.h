#ifndef _UI_H_INCLUDED
#define _UI_H_INCLUDED

#include "../common/common.h"
#include "media_manager_api/spirit_media_manager.h"
//This is an abstract class providing simple user interface function Printf
class tUI
{
public:
  virtual ~tUI() {};
  virtual void Printf(const TCHAR* format, ...) = 0;
  virtual void cPrintf(const char* format, ...) = 0;
  virtual void WaitForUserInput() = 0;
  virtual const TCHAR* UserInputString() = 0;

  virtual bool Create(const char* name) { return true;};

  virtual bool IsUIPauseNeeded() {return false;};
  void DumpIpPort(const TCHAR *prefix,tIpAddr *pAddr)
  {
    Printf(_T("\t%s: %i.%i.%i.%i:%u\n"),prefix,pAddr->ip.i8[3],pAddr->ip.i8[2],pAddr->ip.i8[1],pAddr->ip.i8[0],pAddr->port);
  }

};

#if defined SPIRIT_OS_WIN32_WCE && _WIN32_WCE>=0x501

#include "wce_ui.h"

class tWCE_UI;
typedef tWCE_UI tUI_impl;

#else 
#include "console_ui.h"
class tConsole_UI;
typedef tConsole_UI tUI_impl ;
#endif
#endif // _UI_H_INCLUDED
