#include "stdafx.h"

#if defined(_WIN32_WCE)
#include <aygshell.h>
#endif
#include <atltime.h>
#include "ext/rho/rhoruby.h"
#include "../MainWindow.h"
#include "Signature.h"
#include "common/RhodesApp.h"

#ifdef _MSC_VER
// warning C4800: 'int' : forcing to bool 'true' or 'false' (performance warning)
#pragma warning ( disable : 4800 )
#endif

extern "C" HWND getMainWnd();

#if defined(_WIN32_WCE)
// strdup is implemented as part of ruby CE port
extern "C" char *strdup(const char * str);
extern "C" wchar_t* wce_mbtowc(const char* a);
#endif



void rho_signature_take_signature(char* callback_url, char* image_format) {
}
