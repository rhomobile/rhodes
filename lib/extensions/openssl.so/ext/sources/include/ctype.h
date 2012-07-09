/*  wcecompat: Windows CE C Runtime Library "compatibility" library.
 *
 *  Copyright (C) 2001-2002 Essemer Pty Ltd.  All rights reserved.
 *  http://www.essemer.com.au/
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef __wcecompat__CTYPE_H__
#define __wcecompat__CTYPE_H__

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

extern _CRTIMP const unsigned short  _ctype[];
extern _CRTIMP const unsigned short *_pctype;
extern _CRTIMP const wctype_t *_pwctype;

#define _UPPER      0x1    /* upper case letter */
#define _LOWER      0x2    /* lower case letter */
#define _DIGIT      0x4    /* digit[0-9] */
#define _SPACE      0x8    /* tab, carriage return, newline, */
#define _PUNCT      0x10    /* punctuation character */
#define _CONTROL    0x20    /* control character */
#define _BLANK      0x40    /* space char */
#define _HEX        0x80    /* hexadecimal digit */
#define _ALPHA      (0x0100|_UPPER|_LOWER)    /* alphabetic character */

// character classification function prototypes
#ifdef _WIN32_WCE_EMULATION
_CRTIMP int     __cdecl iswctype(wint_t, wctype_t);
_CRTIMP wchar_t __cdecl towupper(wchar_t);
_CRTIMP wchar_t __cdecl towlower(wchar_t);
#else
_CRTIMP int     iswctype(wint_t, wctype_t);
_CRTIMP wchar_t towupper(wchar_t);
_CRTIMP wchar_t towlower(wchar_t);
#endif

// the character classification macro definitions
_CRTIMP int __cdecl        _isctype(int, int);
#define isalpha(_c)      ( _isctype(_c,_ALPHA) )
#define isupper(_c)      ( _isctype(_c,_UPPER) )
#define islower(_c)      ( _isctype(_c,_LOWER) )
#define isdigit(_c)      ( _isctype(_c,_DIGIT) )
#define isxdigit(_c)     ( _isctype(_c,_HEX) )
#define isspace(_c)      ( _isctype(_c,_SPACE) )
#define ispunct(_c)      ( _isctype(_c,_PUNCT) )
#define isalnum(_c)      ( _isctype(_c,_ALPHA|_DIGIT) )
#define isprint(_c)      ( _isctype(_c,_BLANK|_PUNCT|_ALPHA|_DIGIT) )
#define isgraph(_c)      ( _isctype(_c,_PUNCT|_ALPHA|_DIGIT) )
#define iscntrl(_c)      ( _isctype(_c,_CONTROL) )
#define __isascii(_c)    ((unsigned)(_c) < 0x80)
#define isascii          __isascii

#define iswalpha(_c)     ( iswctype(_c,_ALPHA) )
#define iswupper(_c)     ( iswctype(_c,_UPPER) )
#define iswlower(_c)     ( iswctype(_c,_LOWER) )
#define iswdigit(_c)     ( iswctype(_c,_DIGIT) )
#define iswxdigit(_c)    ( iswctype(_c,_HEX) )
#define iswspace(_c)     ( iswctype(_c,_SPACE) )
#define iswpunct(_c)     ( iswctype(_c,_PUNCT) )
#define iswalnum(_c)     ( iswctype(_c,_ALPHA|_DIGIT) )
#define iswprint(_c)     ( iswctype(_c,_BLANK|_PUNCT|_ALPHA|_DIGIT) )
#define iswgraph(_c)     ( iswctype(_c,_PUNCT|_ALPHA|_DIGIT) )
#define iswcntrl(_c)     ( iswctype(_c,_CONTROL) )
#define iswascii(_c)     ( (unsigned)(_c) < 0x80 )
#define isleadbyte(_c)   ( IsDBCSLeadByte(_c))

#ifdef __cplusplus
}
#endif

#endif /* __wcecompat__CTYPE_H__ */
