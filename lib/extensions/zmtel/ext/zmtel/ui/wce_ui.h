
#ifndef WCE_UI_H_INCLUDED
#define WCE_UI_H_INCLUDED

#include "common/spirit_comtypes.h"

#if defined SPIRIT_OS_WIN32_WCE && _WIN32_WCE>=0x501
#include <windows.h>
#include <tchar.h>
#include "ui.h"
#include <vector>
#include <xmemory>
//#include <xstring>
//#include <iosfwd>

typedef std::basic_string<TCHAR,     std::char_traits<TCHAR>, std::allocator<TCHAR> > 	tstring;

class tWCE_UI : public tUI
{
public:

  tWCE_UI();
  virtual ~tWCE_UI() ;
  virtual void Printf(const TCHAR* format, ...) ;
  virtual void cPrintf(const char* format, ...);
  virtual void WaitForUserInput();
  virtual const TCHAR* UserInputString() {return _T("Press 'Return' button");};
 
  virtual bool IsUIPauseNeeded() {return true;};

  virtual bool Create(const char *name);

  //wce specific
  void OnKeyUp();
  bool SetHWND(HWND hWnd);
  void SetListHWND(HWND hWnd) {
    m_hListWnd = hWnd;
  }
  HWND GetListHWND() {
    return m_hListWnd;
  }
  const TCHAR * GetWindowName() {return m_strName;};


protected:
  void Stop();
private:
  tSystemHandle m_hUIEvent;
  tSystemHandle m_textCS;

  HANDLE m_hUIThread;
  HWND m_hWnd, m_hListWnd;

  DWORD m_dwThreadId;

  bool m_bWaitingUserInput;
  TCHAR m_strName[64];
};

#endif

#endif
