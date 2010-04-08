#ifndef __COMMON__H__
#define __COMMON__H__

#include <stdlib.h>
#include "common/spirit_comtypes.h"

#if defined(WIN32) || defined(WINCE)

// string compatibility
#include <tchar.h>

#define ALLOC_WCS_TO_CHS(x, y)
#define FREE_WCS_TO_CHS(x)

// network compatibility
#include <winsock.h>

#define socklen_t       int
#define SD_BOTH	        0x2

#else // WIN32 || WINCE

// main compatibility
#define _tmain      main

// string compatibility
#define _T(x)       x
#define _TCHAR      char
#define TCHAR       char
#define _tscanf     scanf
#define _strnicmp   strncasecmp
#define _ttoi 	    atoi
#define _tcscmp	    strcmp
#define _tcsncmp	  strncmp
#define _tcslen	    strlen
#define _tcscpy     strcpy
#define _tcsncpy    strncpy
#define _tcsncat    strncat
#define ALLOC_WCS_TO_CHS(x, y)                                  \
{                                                               \
  TCHAR *pFormat;                                               \
                                                                \
  pFormat = strdup(y);                                          \
  if(!pFormat)                                                  \
    return;                                                     \
                                                                \
  *x = pFormat;                                                 \
                                                                \
  if(*pFormat == '%' && *(pFormat + 1) == 'S')                  \
     *pFormat = 's';                                            \
                                                                \
  pFormat++;                                                    \
  while(*pFormat)                                               \
  {                                                             \
    if(*pFormat == '%' && *(pFormat + 1) == 'S' && *(pFormat - 1) != '\\')    \
      *(pFormat + 1) = 's';                                     \
    pFormat++;                                                  \
  }                                                             \
}
#define FREE_WCS_TO_CHS(x)  if(x) free((void*)x)

// network compatibility
#include <sys/socket.h>
#include <netinet/in.h>

#define INVALID_SOCKET	0xFFFFFFFF
#define SOCKET_ERROR	  0xFFFFFFFF

#define SD_BOTH	        0x2
#define closesocket(x)  close(x)

#define	SOCKET    int

#endif // WIN32 || WINCE

#if defined( UNICODE ) || defined( _UNICODE )
  #define TCHAR2CHAR(dest,src,maxlen) wcstombs(dest,src,maxlen);
  #define CHAR2TCHAR(dest,src,maxlen) mbstowcs(dest,src,min(1+strlen(src),maxlen));
#else
  #define TCHAR2CHAR(dest,src,maxlen) strncpy(dest,src,maxlen);
  #define CHAR2TCHAR(dest,src,maxlen) strncpy(dest,src,maxlen);
#endif

#endif // __COMMON__H__
