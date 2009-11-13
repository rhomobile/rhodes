/*
 * Copyright (c) 1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999
 * Boris Fomitchev
 *
 * Written 2000
 * Anton Lapach
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

#include <windows.h>
#include <limits.h>
#if defined (_STLP_MSVC) || defined (__ICL)
#  include <memory.h>
#endif
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

#if defined (_STLP_USE_SAFE_STRING_FUNCTIONS)
#  define _STLP_STRCPY(D, S) strcpy_s(_STLP_ARRAY_AND_SIZE(D), S)
#  define _STLP_STRCPY2(D, DS, S) strcpy_s(D, DS, S)
#  define _STLP_RETURN_STRCPY2(D, DS, S) strcpy_s(D, DS, S); return D
#  define _STLP_STRNCPY(D, DS, S, C) strncpy_s(D, DS, S, C)
#  define _STLP_STRCAT(D, S) strcat_s(_STLP_ARRAY_AND_SIZE(D), S)
#  define _STLP_STRCAT2(D, DS, S) strcat_s(D, DS, S)
#  if !defined (_STLP_NO_WCHAR_T)
#    define _STLP_WCSNCPY(D, DS, S, C) wcsncpy_s(D, DS, S, C)
#  endif
#else
#  define _STLP_STRCPY(D, S) strcpy(D, S)
#  define _STLP_STRCPY2(D, DS, S) strcpy(D, S)
#  define _STLP_RETURN_STRCPY2(D, DS, S) return strcpy(D, S)
#  define _STLP_STRNCPY(D, DS, S, C) strncpy(D, S, C)
#  define _STLP_STRCAT(D, S) strcat(D, S)
#  define _STLP_STRCAT2(D, DS, S) strcat(D, S)
#  if !defined (_STLP_NO_WCHAR_T)
#    define _STLP_WCSNCPY(D, DS, S, C) wcsncpy(D, S, C)
#  endif
#endif

#if defined (__cplusplus)
extern "C" {
#endif

/* Framework functions */
/*
  locale :: "lang[_country[.code_page]]"
  | ".code_page"
  | ""
  | NULL

*/

#if !defined (_LEADBYTE)
/* multibyte leadbyte */
#  define _LEADBYTE 0x8000
#endif

typedef struct _LOCALECONV {
  const char* name;
  const char* abbrev;
} LOCALECONV;

#define MAX_LANG_LEN        64  /* max language name length */
#define MAX_CTRY_LEN        64  /* max country name length */
#define MAX_MODIFIER_LEN    0   /* max modifier name length - n/a */
#define MAX_LC_LEN          (MAX_LANG_LEN+MAX_CTRY_LEN+MAX_MODIFIER_LEN+3)
                                /* max entire locale string length */
#define MAX_CP_LEN          5   /* max code page name length */


/* Metrowerks has different define here */
#if !defined (LC_MAX)
#  if defined (LC_LAST)
#    define LC_MAX LC_LAST
#  endif
#endif

/*  non-NLS language string table */
static LOCALECONV __rg_language[] = {
  {"american",                    "ENU"},
  {"american english",            "ENU"},
  {"american-english",            "ENU"},
  {"australian",                  "ENA"},
  {"belgian",                     "NLB"},
  {"canadian",                    "ENC"},
  {"chh",                         "ZHH"},
  {"chi",                         "ZHI"},
  {"chinese",                     "CHS"},
  {"chinese-hongkong",            "ZHH"},
  {"chinese-simplified",          "CHS"},
  {"chinese-singapore",           "ZHI"},
  {"chinese-traditional",         "CHT"},
  {"dutch-belgian",               "NLB"},
  {"english-american",            "ENU"},
  {"english-aus",                 "ENA"},
  {"english-belize",              "ENL"},
  {"english-can",                 "ENC"},
  {"english-caribbean",           "ENB"},
  {"english-ire",                 "ENI"},
  {"english-jamaica",             "ENJ"},
  {"english-nz",                  "ENZ"},
  {"english-south africa",        "ENS"},
  {"english-trinidad y tobago",   "ENT"},
  {"english-uk",                  "ENG"},
  {"english-us",                  "ENU"},
  {"english-usa",                 "ENU"},
  {"french-belgian",              "FRB"},
  {"french-canadian",             "FRC"},
  {"french-luxembourg",           "FRL"},
  {"french-swiss",                "FRS"},
  {"german-austrian",             "DEA"},
  {"german-lichtenstein",         "DEC"},
  {"german-luxembourg",           "DEL"},
  {"german-swiss",                "DES"},
  {"irish-english",               "ENI"},
  {"italian-swiss",               "ITS"},
  {"norwegian",                   "NOR"},
  {"norwegian-bokmal",            "NOR"},
  {"norwegian-nynorsk",           "NON"},
  {"portuguese-brazilian",        "PTB"},
  {"spanish-argentina",           "ESS"},
  {"spanish-bolivia",             "ESB"},
  {"spanish-chile",               "ESL"},
  {"spanish-colombia",            "ESO"},
  {"spanish-costa rica",          "ESC"},
  {"spanish-dominican republic",  "ESD"},
  {"spanish-ecuador",             "ESF"},
  {"spanish-el salvador",         "ESE"},
  {"spanish-guatemala",           "ESG"},
  {"spanish-honduras",            "ESH"},
  {"spanish-mexican",             "ESM"},
  {"spanish-modern",              "ESN"},
  {"spanish-nicaragua",           "ESI"},
  {"spanish-panama",              "ESA"},
  {"spanish-paraguay",            "ESZ"},
  {"spanish-peru",                "ESR"},
  {"spanish-puerto rico",         "ESU"},
  {"spanish-uruguay",             "ESY"},
  {"spanish-venezuela",           "ESV"},
  {"swedish-finland",             "SVF"},
  {"swiss",                       "DES"},
  {"uk",                          "ENG"},
  {"us",                          "ENU"},
  {"usa",                         "ENU"}
};

/*  non-NLS country string table */
static LOCALECONV __rg_country[] = {
  {"america",                     "USA"},
  {"britain",                     "GBR"},
  {"china",                       "CHN"},
  {"czech",                       "CZE"},
  {"england",                     "GBR"},
  {"great britain",               "GBR"},
  {"holland",                     "NLD"},
  {"hong-kong",                   "HKG"},
  {"new-zealand",                 "NZL"},
  {"nz",                          "NZL"},
  {"pr china",                    "CHN"},
  {"pr-china",                    "CHN"},
  {"puerto-rico",                 "PRI"},
  {"slovak",                      "SVK"},
  {"south africa",                "ZAF"},
  {"south korea",                 "KOR"},
  {"south-africa",                "ZAF"},
  {"south-korea",                 "KOR"},
  {"trinidad & tobago",           "TTO"},
  {"uk",                          "GBR"},
  {"united-kingdom",              "GBR"},
  {"united-states",               "USA"},
  {"us",                          "USA"},
};

typedef struct _Locale_name_hint {
  LCID id;
} _Locale_lcid_t;

typedef struct _Locale_ctype {
  _Locale_lcid_t lc;
  UINT cp;
  unsigned int ctable[256];
} _Locale_ctype_t;

typedef struct _Locale_numeric {
  _Locale_lcid_t lc;
  char cp[MAX_CP_LEN + 1];
  char decimal_point[4];
  char thousands_sep[4];
  char *grouping;
} _Locale_numeric_t;

typedef struct _Locale_time {
  _Locale_lcid_t lc;
  char cp[MAX_CP_LEN + 1];
  char *month[12];
  char *abbrev_month[12];
  char *dayofweek[7];
  char *abbrev_dayofweek[7];
  char *date_time_format;
  char *long_date_time_format;
  char *date_format;
  char *long_date_format;
  char *time_format;
  char am[9];
  char pm[9];
} _Locale_time_t;

typedef struct _Locale_collate {
  _Locale_lcid_t lc;
  char cp[MAX_CP_LEN + 1];
} _Locale_collate_t;

typedef struct _Locale_monetary {
  _Locale_lcid_t lc;
  char cp[MAX_CP_LEN + 1];
  char decimal_point[4];
  char thousands_sep[4];
  char *grouping;
  char int_curr_symbol[5]; /* 3 + 1 + 1 */
  char curr_symbol[6];
  char negative_sign[5];
  char positive_sign[5];
  int frac_digits;
  int int_frac_digits;
} _Locale_monetary_t;

typedef struct _Locale_messages {
  _Locale_lcid_t lc;
  char cp[MAX_CP_LEN + 1];
} _Locale_messages_t;

/* Internal function */
static void __FixGrouping(char *grouping);
static const char* __ConvertName(const char* lname, LOCALECONV* ConvTable, int TableSize);
static int __ParseLocaleString(const char* lname, char* lang, char* ctry, char* page);
static int __GetLCID(const char* lang, const char* ctry, LCID* lcid);
static int __GetLCIDFromName(const char* lname, LCID* lcid, char *cp, _Locale_lcid_t *hint);
static char const* __GetLocaleName(LCID lcid, const char* cp, char* buf);
static char const* __Extract_locale_name(const char* loc, int category, char* buf);
static char const* __TranslateToSystem(const char* lname, char* buf, _Locale_lcid_t* hint);
static void __GetLocaleInfoUsingACP(LCID lcid, const char* cp, LCTYPE lctype, char* buf, int buf_size);
static int __intGetACP(LCID lcid);
static int __intGetOCP(LCID lcid);
static int __GetDefaultCP(LCID lcid);
static char* __ConvertToCP(int from_cp, int to_cp, const char *from, size_t size, size_t *ret_buf_size);
static void my_ltoa(long __x, char* buf);

void my_ltoa(long __x, char* buf) {
  char rbuf[64];
  char* ptr = rbuf;

  if (__x == 0)
    *ptr++ = '0';
  else {
    for (; __x != 0; __x /= 10)
      *ptr++ = (char)(__x % 10) + '0';
  }
  while(ptr > rbuf) *buf++ = *--ptr;
  /* psw */
  *buf = '\0';
}

#if defined (__cplusplus)
_STLP_BEGIN_NAMESPACE
extern "C" {
#endif

  _Locale_lcid_t* _Locale_get_ctype_hint(_Locale_ctype_t* ltype)
  { return (ltype != 0) ? &ltype->lc : 0; }
  _Locale_lcid_t* _Locale_get_numeric_hint(_Locale_numeric_t* lnumeric)
  { return (lnumeric != 0) ? &lnumeric->lc : 0; }
  _Locale_lcid_t* _Locale_get_time_hint(_Locale_time_t* ltime)
  { return (ltime != 0) ? &ltime->lc : 0; }
  _Locale_lcid_t* _Locale_get_collate_hint(_Locale_collate_t* lcollate)
  { return (lcollate != 0) ? &lcollate->lc : 0; }
  _Locale_lcid_t* _Locale_get_monetary_hint(_Locale_monetary_t* lmonetary)
  { return (lmonetary != 0) ? &lmonetary->lc : 0; }
  _Locale_lcid_t* _Locale_get_messages_hint(_Locale_messages_t* lmessages)
  { return (lmessages != 0) ? &lmessages->lc : 0; }

  void* _Locale_ctype_create(const char * name, _Locale_lcid_t* lc_hint) {
    char cname[_Locale_MAX_SIMPLE_NAME];
    char cp_name[MAX_CP_LEN + 1];
    int NativeCP;
    unsigned char Buffer[256];
    unsigned char *ptr;
    unsigned short ctable[256];
    CPINFO CPInfo;
    int i;
    wchar_t *wbuffer;
    int BufferSize;

    _Locale_ctype_t *ltype = (_Locale_ctype_t*)malloc(sizeof(_Locale_ctype_t));
    if (!ltype) return ltype;
    memset(ltype, 0, sizeof(_Locale_ctype_t));

    __Extract_locale_name(name, LC_CTYPE, cname);

    if (__GetLCIDFromName(cname, &ltype->lc.id, cp_name, lc_hint) == -1)
    { free(ltype); return NULL; }

    ltype->cp = atoi(cp_name);

    NativeCP = __GetDefaultCP(ltype->lc.id);

    /* Make table with all characters. */
    for (i = 0; i < 256; ++i) Buffer[i] = (unsigned char)i;

    if (!GetCPInfo(NativeCP, &CPInfo)) { free(ltype); return NULL; }

    if (CPInfo.MaxCharSize > 1) {
      for (ptr = (unsigned char*)CPInfo.LeadByte; *ptr && *(ptr + 1); ptr+=2)
        for (i = *ptr; i <= *(ptr + 1); ++i) Buffer[i] = 0;
    }

    if ((UINT)NativeCP != ltype->cp) {
      OSVERSIONINFO ver_info;
      ver_info.dwOSVersionInfoSize = sizeof(ver_info);
      GetVersionEx(&ver_info);
      if (ver_info.dwPlatformId == VER_PLATFORM_WIN32_NT) {
        /* Convert character sequence to Unicode. */
        BufferSize = MultiByteToWideChar(ltype->cp, MB_PRECOMPOSED, (const char*)Buffer, 256, NULL, 0);
        wbuffer = (wchar_t*)malloc(BufferSize*sizeof(wchar_t));
        if (!MultiByteToWideChar(ltype->cp, MB_PRECOMPOSED, (const char*)Buffer, 256, wbuffer, BufferSize))
        { free(wbuffer); free(ltype); return NULL; }

        GetStringTypeW(CT_CTYPE1, wbuffer, 256, ctable);

        for (i = 0; i < 256; ++i)
          ltype->ctable[i]=(unsigned int)ctable[i];

        if (CPInfo.MaxCharSize > 1) {
          for (ptr = (unsigned char*)CPInfo.LeadByte; *ptr && *(ptr + 1); ptr+=2)
            for (i = *ptr; i <= *(ptr + 1); i++) ltype->ctable[i] = _LEADBYTE;
        }

        free(wbuffer);
      }
      else {
        unsigned char TargetBuffer[256];
        GetStringTypeA(ltype->lc.id, CT_CTYPE1, (const char*)Buffer, 256, ctable);

        /* Convert character sequence to target code page. */
        BufferSize = MultiByteToWideChar(NativeCP, MB_PRECOMPOSED, (const char*)Buffer, 256, NULL, 0);
        wbuffer = (wchar_t*)malloc(BufferSize*sizeof(wchar_t));
        if (!MultiByteToWideChar(NativeCP, MB_PRECOMPOSED, (const char*)Buffer, 256, wbuffer, BufferSize))
        { free(wbuffer); free(ltype); return NULL; }
        if (!WideCharToMultiByte(ltype->cp, WC_COMPOSITECHECK | WC_SEPCHARS, wbuffer, BufferSize, (char*)TargetBuffer, 256, NULL, FALSE))
        { free(wbuffer); free(ltype); return NULL; }

        free(wbuffer);

        /* Translate ctype table. */
        for (i = 0; i < 256; ++i) {
          if (!TargetBuffer[i]) continue;
          ltype->ctable[TargetBuffer[i]] = ctable[i];
        }

        /* Mark lead byte. */
        if (!GetCPInfo(ltype->cp, &CPInfo)) { free(ltype); return NULL; }

        if (CPInfo.MaxCharSize > 1) {
          for (ptr = (unsigned char*)CPInfo.LeadByte; *ptr && *(ptr + 1); ptr+=2)
            for (i = *ptr; i <= *(ptr + 1); ++i) ltype->ctable[i] = _LEADBYTE;
        }
      }
    }
    else {
      GetStringTypeA(ltype->lc.id, CT_CTYPE1, (const char*)Buffer, 256, ctable);
      for (i = 0; i < 256; ++i)
        ltype->ctable[i]=(unsigned int)ctable[i];

      if (CPInfo.MaxCharSize > 1) {
        for (ptr = (unsigned char*)CPInfo.LeadByte; *ptr && *(ptr + 1); ptr+=2)
          for (i = *ptr; i <= *(ptr + 1); ++i) ltype->ctable[i] = _LEADBYTE;
      }
    }
    return ltype;
  }

  void* _Locale_numeric_create(const char * name, _Locale_lcid_t* lc_hint) {
    char *GroupingBuffer;
    char cname[_Locale_MAX_SIMPLE_NAME];
    int BufferSize;
    _Locale_numeric_t *lnum = (_Locale_numeric_t*)malloc(sizeof(_Locale_numeric_t));
    if (!lnum) return lnum; /* MS normal behavior for 'new' */

    __Extract_locale_name(name, LC_NUMERIC, cname);

    if (__GetLCIDFromName(cname, &lnum->lc.id, lnum->cp, lc_hint) == -1)
    { free(lnum); return NULL; }

    __GetLocaleInfoUsingACP(lnum->lc.id, lnum->cp, LOCALE_SDECIMAL, lnum->decimal_point, 4);
    __GetLocaleInfoUsingACP(lnum->lc.id, lnum->cp, LOCALE_STHOUSAND, lnum->thousands_sep, 4);

    BufferSize = GetLocaleInfoA(lnum->lc.id, LOCALE_SGROUPING, NULL, 0);
    GroupingBuffer = (char*)malloc(BufferSize);
    if (!GroupingBuffer) { lnum->grouping = NULL; return lnum; }
    GetLocaleInfoA(lnum->lc.id, LOCALE_SGROUPING, GroupingBuffer, BufferSize);
    __FixGrouping(GroupingBuffer);
    lnum->grouping = GroupingBuffer;

    return lnum;
  }

static int __ConvertDate(const char *NTDate, char *buffer, int buf_size) {
  /* This function will return an incomplete buffer if buffer is not long enough */
  const char *cur_char;
  char *cur_output, *end_output;

  /* Correct time format. */
  cur_char = NTDate;
  cur_output = buffer;
  end_output = cur_output + buf_size;
  buf_size = 0;
  while (*cur_char) {
    if (cur_output && (cur_output == end_output)) break;
    switch (*cur_char) {
    case 'd':
    {
      if (*(cur_char + 1) == 'd') {
        if (cur_output && (cur_output + 2 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (*(cur_char + 2) == 'd') {
          if (*(cur_char + 3) == 'd') {
            if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'A'; }
            buf_size += 2;
            cur_char += 3;
          }
          else {
            if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'a'; }
            buf_size += 2;
            cur_char += 2;
          }
        }
        else {
          if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'd'; }
          buf_size += 2;
          cur_char++;
        }
      }
      else {
        if (cur_output && (cur_output + 3 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = '#'; *(cur_output++) = 'd'; }
        buf_size += 3;
      }
    }
    break;
    case 'M':
    {
      if (*(cur_char + 1) == 'M') {
        if (cur_output && (cur_output + 2 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (*(cur_char + 2) == 'M') {
          if (*(cur_char + 3) == 'M') {
            if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'B'; }
            buf_size += 2;
            cur_char += 3;
          }
          else {
            if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'b'; }
            buf_size += 2;
            cur_char += 2;
          }
        }
        else {
          if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'm'; }
          buf_size += 2;
          cur_char++;
        }
      }
      else {
        if (cur_output && (cur_output + 3 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = '#'; *(cur_output++) = 'm'; }
        buf_size += 3;
      }
    }
    break;
    case 'y':
    {
      if (*(cur_char + 1) == 'y') {
        if (cur_output && (cur_output + 2 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (*(cur_char + 2) == 'y' && *(cur_char + 3) == 'y') {
          if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'Y'; }
          buf_size += 2;
          cur_char += 3;
        }
        else {
          if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'y'; }
          buf_size += 2;
          cur_char++;
        }
      }
      else {
        if (cur_output && (cur_output + 3 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = '#'; *(cur_output++) = 'y'; }
        buf_size += 3;
      }
    }
    break;
    case '%':
    {
      if (cur_output && (cur_output + 2 > end_output)) {
        *cur_output = 0;
        return ++buf_size;
      }
      if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = '%'; }
      buf_size += 2;
    }
    break;
    case '\'':
    {
      ++cur_char;
      while (*cur_char != '\'' && *cur_char != 0 && (cur_output == NULL || cur_output != end_output)) {
        if (cur_output) { *cur_output++ = *cur_char; }
        ++cur_char;
        buf_size += 1;
      }
    }
    break;
    default:
    {
      if (cur_output) { *(cur_output++) = *cur_char; }
      buf_size += 1;
    }
    break;
    }
    if (*cur_char == 0) break;
    ++cur_char;
  }

  if (!cur_output || cur_output != end_output) {
    if (cur_output) *cur_output = 0;
    buf_size += 1;
  }
  else {
    /* We trunc result */
    *(--cur_output) = 0;
  }

  return buf_size;
}

static int __ConvertTime(const char *NTTime, char *buffer, int buf_size) {
  const char *cur_char;
  char *cur_output, *end_output;
  cur_char = NTTime;
  cur_output = buffer;
  end_output = cur_output + buf_size;
  buf_size = 0;
  while (*cur_char) {
    switch(*cur_char) {
    case 'h':
      if (*(cur_char + 1) == 'h') {
        if (cur_output && (cur_output + 2 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'I'; }
        buf_size += 2;
        ++cur_char;
      }
      else {
        if (cur_output && (cur_output + 3 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = '#'; *(cur_output++) = 'I'; }
        buf_size += 3;
      }
      break;
    case 'H':
      if (*(cur_char + 1) == 'H') {
        if (cur_output && (cur_output + 2 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'H'; }
        buf_size += 2;
        ++cur_char;
      }
      else {
        if (cur_output && (cur_output + 3 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = '#'; *(cur_output++) = 'H'; }
        buf_size += 3;
      }
      break;
    case 'm':
      if (*(cur_char + 1) == 'm') {
        if (cur_output && (cur_output + 2 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'M'; }
        buf_size += 2;
        cur_char++;
      }
      else {
        if (cur_output && (cur_output + 3 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = '#'; *(cur_output++) = 'M'; }
        buf_size += 3;
      }
      break;
    case 's':
      if (*(cur_char + 1) == 's') {
        if (cur_output && (cur_output + 2 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'S'; }
        buf_size += 2;
        ++cur_char;
      }
      else {
        if (cur_output && (cur_output + 3 > end_output)) {
          *cur_output = 0;
          return ++buf_size;
        }
        if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = '#'; *(cur_output++) = 'S'; }
        buf_size += 3;
      }
      break;
    case 't':
      if (*(cur_char + 1) == 't')
        ++cur_char;
      if (cur_output && (cur_output + 2 > end_output)) {
        *cur_output = 0;
        return ++buf_size;
      }
      if (cur_output) { *(cur_output++) = '%'; *(cur_output++) = 'p'; }
      buf_size += 2;
      break;
    case '%':
      if (cur_output && (cur_output + 2 > end_output)) {
        *cur_output = 0;
        return ++buf_size;
      }
      if (cur_output) { *(cur_output++)='%'; *(cur_output++)='%'; }
      buf_size += 2;
      break;
    case '\'':
      ++cur_char;
      while (*cur_char != '\'' && *cur_char != 0 && (!cur_output || (cur_output != end_output))) {
        if (cur_output) *cur_output++ = *cur_char;
        ++cur_char;
        buf_size += 1;
      }
      break;
    default:
      if (cur_output) { *(cur_output++) = *cur_char; }
      buf_size += 1;
      break;
    }
    if (*cur_char == 0) break;
    ++cur_char;
  }

  if (!cur_output || cur_output != end_output) {
    if (cur_output) *cur_output = 0;
    buf_size += 1;
  }
  else {
    /* We trunc result */
    *(--cur_output) = 0;
  }

  return buf_size;
}

void* _Locale_time_create(const char * name, _Locale_lcid_t* lc_hint) {
    int size, month, dayofweek;
    size_t length;
    char fmt80[80];
    char cname[_Locale_MAX_SIMPLE_NAME];

    _Locale_time_t *ltime=(_Locale_time_t*)malloc(sizeof(_Locale_time_t));
    if (!ltime) return ltime;
    memset(ltime, 0, sizeof(_Locale_time_t));

    __Extract_locale_name(name, LC_TIME, cname);

    if (__GetLCIDFromName(cname, &ltime->lc.id, ltime->cp, lc_hint) == -1)
    { free(ltime); return NULL; }

    for (month = LOCALE_SMONTHNAME1; month <= LOCALE_SMONTHNAME12; ++month) { /* Small hack :-) */
      size = GetLocaleInfoA(ltime->lc.id, month, NULL, 0);
      ltime->month[month - LOCALE_SMONTHNAME1] = (char*)malloc(size);
      if (!ltime->month[month - LOCALE_SMONTHNAME1]) { _Locale_time_destroy(ltime); return NULL; }
      __GetLocaleInfoUsingACP(ltime->lc.id, ltime->cp, month, ltime->month[month - LOCALE_SMONTHNAME1], size);
    }

    for (month = LOCALE_SABBREVMONTHNAME1; month <= LOCALE_SABBREVMONTHNAME12; ++month) {
      size = GetLocaleInfoA(ltime->lc.id, month, NULL, 0);
      ltime->abbrev_month[month - LOCALE_SABBREVMONTHNAME1] = (char*)malloc(size);
      if (!ltime->abbrev_month[month - LOCALE_SABBREVMONTHNAME1]) { _Locale_time_destroy(ltime); return NULL; }
      __GetLocaleInfoUsingACP(ltime->lc.id, ltime->cp, month, ltime->abbrev_month[month - LOCALE_SABBREVMONTHNAME1], size);
    }

  for (dayofweek = LOCALE_SDAYNAME1; dayofweek <= LOCALE_SDAYNAME7; ++dayofweek) {
      int dayindex = ( dayofweek != LOCALE_SDAYNAME7 ) ? dayofweek - LOCALE_SDAYNAME1 + 1 : 0;
      size = GetLocaleInfoA(ltime->lc.id, dayofweek, NULL, 0);
      ltime->dayofweek[dayindex] = (char*)malloc(size);
      if (!ltime->dayofweek[dayindex]) { _Locale_time_destroy(ltime); return NULL; }
      __GetLocaleInfoUsingACP(ltime->lc.id, ltime->cp, dayofweek, ltime->dayofweek[dayindex], size);
    }

  for (dayofweek = LOCALE_SABBREVDAYNAME1; dayofweek <= LOCALE_SABBREVDAYNAME7; ++dayofweek) {
      int dayindex = ( dayofweek != LOCALE_SABBREVDAYNAME7 ) ? dayofweek - LOCALE_SABBREVDAYNAME1 + 1 : 0;
      size = GetLocaleInfoA(ltime->lc.id, dayofweek, NULL, 0);
      ltime->abbrev_dayofweek[dayindex] = (char*)malloc(size);
      if (!ltime->abbrev_dayofweek[dayindex]) { _Locale_time_destroy(ltime); return NULL; }
      __GetLocaleInfoUsingACP(ltime->lc.id, ltime->cp, dayofweek, ltime->abbrev_dayofweek[dayindex], size);
    }

    __GetLocaleInfoUsingACP(ltime->lc.id, ltime->cp, LOCALE_SSHORTDATE, fmt80, 80);
    size = __ConvertDate(fmt80, NULL, 0);
    ltime->date_format = (char*)malloc(size);
    if (!ltime->date_format) { _Locale_time_destroy(ltime); return NULL; }
    __ConvertDate(fmt80, ltime->date_format, size);

    __GetLocaleInfoUsingACP(ltime->lc.id, ltime->cp, LOCALE_SLONGDATE, fmt80, 80);
    size = __ConvertDate(fmt80, NULL, 0);
    ltime->long_date_format = (char*)malloc(size);
    if (!ltime->long_date_format) { _Locale_time_destroy(ltime); return NULL; }
    __ConvertDate(fmt80, ltime->long_date_format, size);

    __GetLocaleInfoUsingACP(ltime->lc.id, ltime->cp, LOCALE_STIMEFORMAT, fmt80, 80);
    size = __ConvertTime(fmt80, NULL, 0);
    ltime->time_format = (char*)malloc(size);
    if (!ltime->time_format) { _Locale_time_destroy(ltime); return NULL; }
    __ConvertTime(fmt80, ltime->time_format, size);

    /* NT doesn't provide this information, we must simulate. */
    length = strlen(ltime->date_format) + strlen(ltime->time_format) + 1 /* space */ + 1 /* trailing 0 */;
    ltime->date_time_format = (char*)malloc(length);
    if (!ltime->date_time_format) { _Locale_time_destroy(ltime); return NULL; }
    _STLP_STRCPY2(ltime->date_time_format, length, ltime->date_format);
    _STLP_STRCAT2(ltime->date_time_format, length, " ");
    _STLP_STRCAT2(ltime->date_time_format, length, ltime->time_format);

    /* NT doesn't provide this information, we must simulate. */
    length = strlen(ltime->long_date_format) + strlen(ltime->time_format) + 1 /* space */ + 1 /* trailing 0 */;
    ltime->long_date_time_format = (char*)malloc(length);
    if (!ltime->long_date_time_format) { _Locale_time_destroy(ltime); return NULL; }
    _STLP_STRCPY2(ltime->long_date_time_format, length, ltime->long_date_format);
    _STLP_STRCAT2(ltime->long_date_time_format, length, " ");
    _STLP_STRCAT2(ltime->long_date_time_format, length, ltime->time_format);

    __GetLocaleInfoUsingACP(ltime->lc.id, ltime->cp, LOCALE_S1159, ltime->am, 9);
    __GetLocaleInfoUsingACP(ltime->lc.id, ltime->cp, LOCALE_S2359, ltime->pm, 9);

    return ltime;
  }

  void* _Locale_collate_create(const char * name, _Locale_lcid_t* lc_hint) {
    char cname[_Locale_MAX_SIMPLE_NAME];

    _Locale_collate_t *lcol=(_Locale_collate_t*)malloc(sizeof(_Locale_collate_t));
    if (!lcol) return lcol;
    memset(lcol, 0, sizeof(_Locale_collate_t));

    __Extract_locale_name(name, LC_COLLATE, cname);

    if (__GetLCIDFromName(cname, &lcol->lc.id, lcol->cp, lc_hint) == -1)
    { free(lcol); return NULL; }

    return lcol;
  }

  void* _Locale_monetary_create(const char * name, _Locale_lcid_t* lc_hint) {
    char cname[_Locale_MAX_SIMPLE_NAME];
    char *GroupingBuffer;
    int BufferSize;
    char FracDigits[3];

    _Locale_monetary_t *lmon = (_Locale_monetary_t*)malloc(sizeof(_Locale_monetary_t));
    if (!lmon) return lmon;
    memset(lmon, 0, sizeof(_Locale_monetary_t));

    __Extract_locale_name(name, LC_MONETARY, cname);

    if (__GetLCIDFromName(cname, &lmon->lc.id, lmon->cp, lc_hint) == -1)
    { free(lmon); return NULL; }

    /* Extract information about monetary system */
    __GetLocaleInfoUsingACP(lmon->lc.id, lmon->cp, LOCALE_SDECIMAL, lmon->decimal_point, 4);
    __GetLocaleInfoUsingACP(lmon->lc.id, lmon->cp, LOCALE_STHOUSAND, lmon->thousands_sep, 4);

    BufferSize = GetLocaleInfoA(lmon->lc.id, LOCALE_SGROUPING, NULL, 0);
    GroupingBuffer = (char*)malloc(BufferSize);
    if (!GroupingBuffer) { lmon->grouping = NULL; return lmon; }
    GetLocaleInfoA(lmon->lc.id, LOCALE_SGROUPING, GroupingBuffer, BufferSize);
    __FixGrouping(GroupingBuffer);
    lmon->grouping=GroupingBuffer;

    __GetLocaleInfoUsingACP(lmon->lc.id, lmon->cp, LOCALE_SCURRENCY, lmon->curr_symbol, 6);
    __GetLocaleInfoUsingACP(lmon->lc.id, lmon->cp, LOCALE_SNEGATIVESIGN, lmon->negative_sign, 5);
    __GetLocaleInfoUsingACP(lmon->lc.id, lmon->cp, LOCALE_SPOSITIVESIGN, lmon->positive_sign, 5);

    GetLocaleInfoA(lmon->lc.id, LOCALE_ICURRDIGITS, FracDigits, 3);
    lmon->frac_digits = atoi(FracDigits);

    GetLocaleInfoA(lmon->lc.id, LOCALE_IINTLCURRDIGITS, FracDigits, 3);
    lmon->int_frac_digits = atoi(FracDigits);

    __GetLocaleInfoUsingACP(lmon->lc.id, lmon->cp, LOCALE_SINTLSYMBOL, lmon->int_curr_symbol, 5);
    /* Even if Platform SDK documentation says that the returned symbol should
     * be a 3 letters symbol followed by a seperation character, experimentation
     * has shown that no seperation character is ever appended. We are adding it
     * ourself to conform to the POSIX specification.
     */
    if (lmon->int_curr_symbol[3] == 0) {
      lmon->int_curr_symbol[3] = ' ';
      lmon->int_curr_symbol[4] = 0;
    }

    return lmon;
  }

  void* _Locale_messages_create(const char *name, _Locale_lcid_t* lc_hint) {
    char cname[_Locale_MAX_SIMPLE_NAME];
    _Locale_messages_t *lmes=(_Locale_messages_t*)malloc(sizeof(_Locale_messages_t));
    if (!lmes) return lmes;
    memset(lmes, 0, sizeof(_Locale_messages_t));

    _Locale_extract_messages_name(name, cname, lc_hint);
    if (__GetLCIDFromName(cname, &lmes->lc.id, lmes->cp, lc_hint) == -1)
    { free(lmes); return NULL; }

    return lmes;
  }

  static const char* _Locale_common_default(char* buf) {
    char cp[MAX_CP_LEN + 1];
    int CodePage = __GetDefaultCP(LOCALE_USER_DEFAULT);
    my_ltoa(CodePage, cp);
    return __GetLocaleName(LOCALE_USER_DEFAULT, cp, buf);
  }

  const char* _Locale_ctype_default(char* buf)
  { return _Locale_common_default(buf); }

  const char* _Locale_numeric_default(char * buf)
  { return _Locale_common_default(buf); }

  const char* _Locale_time_default(char* buf)
  { return _Locale_common_default(buf); }

  const char* _Locale_collate_default(char* buf)
  { return _Locale_common_default(buf); }

  const char* _Locale_monetary_default(char* buf)
  { return _Locale_common_default(buf); }

  const char* _Locale_messages_default(char* buf)
  { return _Locale_common_default(buf); }

  char const* _Locale_ctype_name(const void* loc, char* buf) {
    char cp_buf[MAX_CP_LEN + 1];
    _Locale_ctype_t* ltype = (_Locale_ctype_t*)loc;
    my_ltoa(ltype->cp, cp_buf);
    return __GetLocaleName(ltype->lc.id, cp_buf, buf);
  }

  char const* _Locale_numeric_name(const void* loc, char* buf) {
    _Locale_numeric_t* lnum = (_Locale_numeric_t*)loc;
    return __GetLocaleName(lnum->lc.id, lnum->cp, buf);
  }

  char const* _Locale_time_name(const void* loc, char* buf) {
    _Locale_time_t* ltime = (_Locale_time_t*)loc;
    return __GetLocaleName(ltime->lc.id, ltime->cp, buf);
  }

  char const* _Locale_collate_name(const void* loc, char* buf) {
    _Locale_collate_t* lcol = (_Locale_collate_t*)loc;
    return __GetLocaleName(lcol->lc.id, lcol->cp, buf);
  }

  char const* _Locale_monetary_name(const void* loc, char* buf) {
    _Locale_monetary_t* lmon = (_Locale_monetary_t*)loc;
    return __GetLocaleName(lmon->lc.id, lmon->cp, buf);
  }

  char const* _Locale_messages_name(const void* loc, char* buf) {
    _Locale_messages_t* lmes = (_Locale_messages_t*)loc;
    return __GetLocaleName(lmes->lc.id, lmes->cp, buf);
  }

  void _Locale_ctype_destroy(void* loc) {
    _Locale_ctype_t *ltype = (_Locale_ctype_t*)loc;
    if (!ltype) return;
    free(ltype);
  }

  void _Locale_numeric_destroy(void* loc) {
    _Locale_numeric_t *lnum = (_Locale_numeric_t *)loc;
    if (!lnum) return;

    if (lnum->grouping) free(lnum->grouping);
    free(lnum);
  }

  void _Locale_time_destroy(void* loc) {
    int i;
    _Locale_time_t* ltime = (_Locale_time_t*)loc;
    if (!ltime) return;

  for (i = 0; i < 12; ++i) {
      if (ltime->month[i]) free(ltime->month[i]);
      if (ltime->abbrev_month[i]) free(ltime->abbrev_month[i]);
    }

  for (i = 0; i < 7; ++i) {
      if (ltime->dayofweek[i]) free(ltime->dayofweek[i]);
      if (ltime->abbrev_dayofweek[i]) free(ltime->abbrev_dayofweek[i]);
    }

    if (ltime->date_format) free(ltime->date_format);
    if (ltime->long_date_format) free(ltime->long_date_format);
    if (ltime->time_format) free(ltime->time_format);
    if (ltime->date_time_format) free(ltime->date_time_format);
    if (ltime->long_date_time_format) free(ltime->long_date_time_format);

    free(ltime);
  }

  void _Locale_collate_destroy(void* loc) {
    _Locale_collate_t* lcol=(_Locale_collate_t*)loc;
    if (!lcol) return;

    free(lcol);
  }

  void _Locale_monetary_destroy(void* loc) {
    _Locale_monetary_t *lmon = (_Locale_monetary_t*)loc;
    if (!lmon) return;

    if (lmon->grouping) free(lmon->grouping);
    free(lmon);
  }

  void _Locale_messages_destroy(void* loc) {
    _Locale_messages_t* lmes = (_Locale_messages_t*)loc;
    if (!lmes) return;

    free(lmes);
  }

  static char const* _Locale_extract_category_name(const char* cname, int category, char* buf, _Locale_lcid_t* hint) {
    char lname[_Locale_MAX_SIMPLE_NAME];
    __Extract_locale_name(cname, category, lname);
    if (lname[0] == 'C' && lname[1] == 0) {
      _STLP_RETURN_STRCPY2(buf, _Locale_MAX_SIMPLE_NAME, lname);
    }
    return __TranslateToSystem(lname, buf, hint);
  }

  char const* _Locale_extract_ctype_name(const char* cname, char* buf, _Locale_lcid_t* hint)
  { return _Locale_extract_category_name(cname, LC_CTYPE, buf, hint); }

  char const* _Locale_extract_numeric_name(const char* cname, char* buf, _Locale_lcid_t* hint)
  { return _Locale_extract_category_name(cname, LC_NUMERIC, buf, hint); }

  char const* _Locale_extract_time_name(const char* cname, char* buf, _Locale_lcid_t* hint)
  { return _Locale_extract_category_name(cname, LC_TIME, buf, hint); }

  char const* _Locale_extract_collate_name(const char* cname, char* buf, _Locale_lcid_t* hint)
  { return _Locale_extract_category_name(cname, LC_COLLATE, buf, hint); }

  char const* _Locale_extract_monetary_name(const char* cname, char* buf, _Locale_lcid_t* hint)
  { return _Locale_extract_category_name(cname, LC_MONETARY, buf, hint); }

  char const* _Locale_extract_messages_name(const char* cname, char* buf, _Locale_lcid_t* hint) {
    if (cname[0] == 'L' && cname[1] == 'C' && cname[2] == '_') {
      _STLP_RETURN_STRCPY2(buf, _Locale_MAX_SIMPLE_NAME, "C");
    }
    if (cname[0] == 'C' && cname[1] == 0) {
      _STLP_RETURN_STRCPY2(buf, _Locale_MAX_SIMPLE_NAME, cname);
    }
    return __TranslateToSystem(cname, buf, hint);
  }

  char const* _Locale_compose_name(char* buf,
                             const char* _ctype, const char* numeric,
                             const char* time, const char* _collate,
                             const char* monetary, const char* messages,
                             const char* default_name) {
    (void) default_name;

    if (!strcmp(_ctype, numeric) &&
       !strcmp(_ctype, time) &&
       !strcmp(_ctype, _collate) &&
       !strcmp(_ctype, monetary) &&
       !strcmp(_ctype, messages)) {
      _STLP_RETURN_STRCPY2(buf, _Locale_MAX_COMPOSITE_NAME, _ctype);
    }

    _STLP_STRCPY2(buf, _Locale_MAX_COMPOSITE_NAME, "LC_CTYPE=");
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, _ctype);
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, ";");
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, "LC_TIME=");
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, time);
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, ";");
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, "LC_NUMERIC=");
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, numeric);
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, ";");
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, "LC_COLLATE=");
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, _collate);
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, ";");
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, "LC_MONETARY=");
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, monetary);
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, ";");
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, "LC_MESSAGES=");
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, messages);
    _STLP_STRCAT2(buf, _Locale_MAX_COMPOSITE_NAME, ";");

    return buf;
  }

  /* ctype */

  const _Locale_mask_t* _Locale_ctype_table(_Locale_ctype_t* ltype) {
    _STLP_STATIC_ASSERT(sizeof(_Locale_mask_t) == sizeof(unsigned int))
    return (const _Locale_mask_t*)ltype->ctable;
  }

  int _Locale_toupper(_Locale_ctype_t* ltype, int c) {
    char buf[2], out_buf[2];
    buf[0] = (char)c; buf[1] = 0;
    if ((UINT)__GetDefaultCP(ltype->lc.id) == ltype->cp) {
      LCMapStringA(ltype->lc.id, LCMAP_LINGUISTIC_CASING | LCMAP_UPPERCASE, buf, 2, out_buf, 2);
      return out_buf[0];
    }
    else {
      wchar_t wbuf[2];
      MultiByteToWideChar(ltype->cp, MB_PRECOMPOSED, buf, 2, wbuf, 2);
      WideCharToMultiByte(__GetDefaultCP(ltype->lc.id), WC_COMPOSITECHECK | WC_SEPCHARS, wbuf, 2, buf, 2, NULL, FALSE);

      LCMapStringA(ltype->lc.id, LCMAP_LINGUISTIC_CASING | LCMAP_UPPERCASE, buf, 2, out_buf, 2);

      MultiByteToWideChar(__GetDefaultCP(ltype->lc.id), MB_PRECOMPOSED, out_buf, 2, wbuf, 2);
      WideCharToMultiByte(ltype->cp, WC_COMPOSITECHECK | WC_SEPCHARS, wbuf, 2, out_buf, 2, NULL, FALSE);
      return out_buf[0];
    }
  }

  int _Locale_tolower(_Locale_ctype_t* ltype, int c) {
    char buf[2], out_buf[2];
    buf[0] = (char)c; buf[1] = 0;
    if ((UINT)__GetDefaultCP(ltype->lc.id) == ltype->cp) {
      LCMapStringA(ltype->lc.id, LCMAP_LINGUISTIC_CASING | LCMAP_LOWERCASE, buf, 2, out_buf, 2);
      return out_buf[0];
    }
    else {
      wchar_t wbuf[2];
      MultiByteToWideChar(ltype->cp, MB_PRECOMPOSED, buf, 2, wbuf, 2);
      WideCharToMultiByte(__GetDefaultCP(ltype->lc.id), WC_COMPOSITECHECK | WC_SEPCHARS, wbuf, 2, buf, 2, NULL, FALSE);

      LCMapStringA(ltype->lc.id, LCMAP_LINGUISTIC_CASING | LCMAP_LOWERCASE, buf, 2, out_buf, 2);

      MultiByteToWideChar(__GetDefaultCP(ltype->lc.id), MB_PRECOMPOSED, out_buf, 2, wbuf, 2);
      WideCharToMultiByte(ltype->cp, WC_COMPOSITECHECK | WC_SEPCHARS, wbuf, 2, out_buf, 2, NULL, FALSE);
      return out_buf[0];
    }
  }

#if !defined (_STLP_NO_WCHAR_T)
  _Locale_mask_t _Locale_wchar_ctype(_Locale_ctype_t* ltype, wint_t c,
                                     _Locale_mask_t which_bits) {
    wchar_t buf[2];
    WORD out[2];
    buf[0] = c; buf[1] = 0;
    GetStringTypeW(CT_CTYPE1, buf, -1, out);
    (void*)ltype;
    return (_Locale_mask_t)out[0] & which_bits;
  }

  wint_t _Locale_wchar_tolower(_Locale_ctype_t* ltype, wint_t c) {
    wchar_t in_c = c;
    wchar_t res;

    LCMapStringW(ltype->lc.id, LCMAP_LOWERCASE, &in_c, 1, &res, 1);
    return res;
  }

  wint_t _Locale_wchar_toupper(_Locale_ctype_t* ltype, wint_t c) {
    wchar_t in_c = c;
    wchar_t res;

    LCMapStringW(ltype->lc.id, LCMAP_UPPERCASE, &in_c, 1, &res, 1);
    return res;
  }
#endif

#if !defined (_STLP_NO_MBSTATE_T)

  int _Locale_mb_cur_max (_Locale_ctype_t * ltype) {
    CPINFO CPInfo;
    if (!GetCPInfo(ltype->cp, &CPInfo)) return 0;
    return CPInfo.MaxCharSize;
  }

  int _Locale_mb_cur_min (_Locale_ctype_t *dummy) {
    (void*)dummy;
    return 1;
  }

  int _Locale_is_stateless (_Locale_ctype_t * ltype) {
    CPINFO CPInfo;
    GetCPInfo(ltype->cp, &CPInfo);
    return (CPInfo.MaxCharSize == 1) ? 1 : 0;
  }

#if defined (__BORLANDC__) && defined (__cplusplus)
  /* Weird Borland compiler behavior, even if native wint_t is imported to
   * STLport namespace in _cwchar.h, wint_t is still usable when scoped with
   * the Standard namespace (std::wint_t). As following WEOF macro is expended
   * to (std::wint_t)(0xFFFF) compilation failed. Repeating import avoid this
   * problem.*/
  using __std_alias::wint_t;
#endif

  wint_t _Locale_btowc(_Locale_ctype_t * ltype, int c) {
    wchar_t wc;
    if (c == EOF) return WEOF;

    MultiByteToWideChar(ltype->cp, MB_PRECOMPOSED, (char*)&c, 1, &wc, 1);

    return (wint_t)wc;
  }

  int _Locale_wctob(_Locale_ctype_t * ltype, wint_t wc) {
    char c;

    if (WideCharToMultiByte(ltype->cp, WC_COMPOSITECHECK | WC_DEFAULTCHAR, (wchar_t*)&wc, 1, &c, 1, NULL, NULL) == 0)
      return WEOF; /* Not single byte or error conversion. */

    return (int)c;
  }

  static int __isleadbyte(int c, unsigned int *ctable) {
    return (ctable[(unsigned char)(c)] & _LEADBYTE);
  }

  static size_t __mbtowc(_Locale_ctype_t *l, wchar_t *dst, char src, mbstate_t *shift_state) {
    int result;

    if (*shift_state == 0) {
      if (__isleadbyte(src, l->ctable)) {
        ((unsigned char*)shift_state)[0] = src;
        return (size_t)-2;
      }
      else {
        result = MultiByteToWideChar(l->cp, MB_PRECOMPOSED, &src, 1, dst, 1);
        if (result == 0) return (size_t)-1;

        return 1;
      }
    }
    else {
      ((unsigned char*)shift_state)[1] = src;
      result = MultiByteToWideChar(l->cp, MB_PRECOMPOSED, (const char*)shift_state, 2, dst, 1);
      *shift_state = 0;
      if (result == 0) return (size_t)-1;

      return 1;
    }
  }

#if !defined (_STLP_NO_WCHAR_T)
  size_t _Locale_mbtowc(_Locale_ctype_t *ltype, wchar_t *to,
                        const char *from, size_t n, mbstate_t *shift_state) {
    CPINFO ci;
    int result;
    (void*)shift_state;
    GetCPInfo(ltype->cp, &ci);
    if (ci.MaxCharSize == 1) { /* Single byte encoding. */
      *shift_state = (mbstate_t)0;
      result = MultiByteToWideChar(ltype->cp, MB_PRECOMPOSED, from, 1, to, 1);
      if (result == 0) return (size_t)-1;
      return result;
    }
    else { /* Multi byte encoding. */
      size_t retval = 0, count = 0;
      while(n--) {
        retval = __mbtowc(ltype, to, *from, shift_state);
        if (retval == -2) { from++; count++; }
        else if (retval == -1) return -1;
        else return count+retval;
      }
      if (retval == -2) return (size_t)-2;

      return n;
    }
  }

  size_t _Locale_wctomb(_Locale_ctype_t *ltype, char *to, size_t n,
                        const wchar_t c, mbstate_t *shift_state) {
    int size = \
      WideCharToMultiByte(ltype->cp,  WC_COMPOSITECHECK | WC_SEPCHARS, &c, 1, NULL, 0, NULL, NULL);

    if ((size_t)size > n) return (size_t)-2;

    if (n > INT_MAX)
      /* Limiting the output buf size to INT_MAX seems like reasonable to transform a single wchar_t. */
      n = INT_MAX;

    size = \
      WideCharToMultiByte(ltype->cp,  WC_COMPOSITECHECK | WC_SEPCHARS, &c, 1, to, (int)n, NULL, NULL);

    if (size == 0) return (size_t)-1;

    (void*)shift_state;
    return (size_t)size;
  }
#endif

  size_t _Locale_unshift(_Locale_ctype_t *ltype, mbstate_t *st,
                         char *buf, size_t n, char **next) {
    (void*)ltype;
    if (*st == 0) {
      *next = buf;
      return 0;
    }
    else {
      if (n < 1) { *next = buf; return (size_t)-2; }

      *next = buf + 1;
      return 1;
    }
  }

#endif /*  _STLP_NO_MBSTATE_T */


#ifndef CSTR_EQUAL /* VC5SP3*/
#  define CSTR_EQUAL 2
#endif
#ifndef CSTR_LESS_THAN /* VC5SP3 */
#  define CSTR_LESS_THAN 1
#endif

  static DWORD max_DWORD = 0xffffffff;
  static DWORD trim_size_t_to_DWORD(size_t n) { return n < (size_t)max_DWORD ? (DWORD)n : max_DWORD; }

  /* Collate */
  /* This function takes care of the potential size_t DWORD different size. */
  static int _Locale_strcmp_auxA(_Locale_collate_t* lcol,
                                 const char* s1, size_t n1,
                                 const char* s2, size_t n2) {
    int result = CSTR_EQUAL;
    while (n1 > 0 || n2 > 0) {
      DWORD size1 = trim_size_t_to_DWORD(n1);
      DWORD size2 = trim_size_t_to_DWORD(n2);
      result = CompareStringA(lcol->lc.id, 0, s1, size1, s2, size2);
      if (result != CSTR_EQUAL)
        break;
      n1 -= size1;
      n2 -= size2;
    }
    return result;
  }

  int _Locale_strcmp(_Locale_collate_t* lcol,
                     const char* s1, size_t n1,
                     const char* s2, size_t n2) {
    int result;
    if (__GetDefaultCP(lcol->lc.id) == atoi(lcol->cp)) {
      result = _Locale_strcmp_auxA(lcol, s1, n1, s2, n2);
    }
    else {
      char *buf1, *buf2;
      size_t size1, size2;
      buf1 = __ConvertToCP(atoi(lcol->cp), __GetDefaultCP(lcol->lc.id), s1, n1, &size1);
      buf2 = __ConvertToCP(atoi(lcol->cp), __GetDefaultCP(lcol->lc.id), s2, n2, &size2);

      result = _Locale_strcmp_auxA(lcol, buf1, size1, buf2, size2);
      free(buf1); free(buf2);
    }
    return (result == CSTR_EQUAL) ? 0 : (result == CSTR_LESS_THAN) ? -1 : 1;
  }

#if !defined (_STLP_NO_WCHAR_T)
  /* This function takes care of the potential size_t DWORD different size. */
  static int _Locale_strcmp_auxW(_Locale_collate_t* lcol,
                                 const wchar_t* s1, size_t n1,
                                 const wchar_t* s2, size_t n2) {
    int result = CSTR_EQUAL;
    while (n1 > 0 || n2 > 0) {
      DWORD size1 = trim_size_t_to_DWORD(n1);
      DWORD size2 = trim_size_t_to_DWORD(n2);
      result = CompareStringW(lcol->lc.id, 0, s1, size1, s2, size2);
      if (result != CSTR_EQUAL)
        break;
      n1 -= size1;
      n2 -= size2;
    }
    return result;
  }

  int _Locale_strwcmp(_Locale_collate_t* lcol,
                      const wchar_t* s1, size_t n1,
                      const wchar_t* s2, size_t n2) {
    int result;
    result = _Locale_strcmp_auxW(lcol, s1, n1, s2, n2);
    return (result == CSTR_EQUAL) ? 0 : (result == CSTR_LESS_THAN) ? -1 : 1;
  }
#endif

  size_t _Locale_strxfrm(_Locale_collate_t* lcol,
                         char* dst, size_t dst_size,
                         const char* src, size_t src_size) {
    int result;

    /* The Windows API do not support transformation of very long strings (src_size > INT_MAX)
     * In this case the result will just be the input string:
     */
    if (src_size > INT_MAX) {
      if (dst != 0) {
        _STLP_STRNCPY(dst, dst_size, src, src_size);
      }
      return src_size;
    }
    if (dst_size > INT_MAX) {
      /* now that we know that src_size <= INT_MAX we can safely decrease dst_size to INT_MAX. */
      dst_size = INT_MAX;
    }

    if (__GetDefaultCP(lcol->lc.id) == atoi(lcol->cp))
      result = LCMapStringA(lcol->lc.id, LCMAP_SORTKEY, src, (int)src_size, dst, (int)dst_size);
    else {
      char *buf;
      size_t size;
      buf = __ConvertToCP(atoi(lcol->cp), __GetDefaultCP(lcol->lc.id), src, src_size, &size);

      result = LCMapStringA(lcol->lc.id, LCMAP_SORTKEY, buf, (int)size, dst, (int)dst_size);
      free(buf);
    }
    return result != 0 ? result - 1 : 0;
  }

#if !defined (_STLP_NO_WCHAR_T)
  size_t _Locale_strwxfrm(_Locale_collate_t* lcol,
                          wchar_t* dst, size_t dst_size,
                          const wchar_t* src, size_t src_size) {
    int result;

    /* see _Locale_strxfrm: */
    if (src_size > INT_MAX) {
      if (dst != 0) {
        _STLP_WCSNCPY(dst, dst_size, src, src_size);
      }
      return src_size;
    }
    if (dst_size > INT_MAX) {
      dst_size = INT_MAX;
    }
    result = LCMapStringW(lcol->lc.id, LCMAP_SORTKEY, src, (int)src_size, dst, (int)dst_size);
    return result != 0 ? result - 1 : 0;
  }
#endif

  /* Numeric */

  static const char* __true_name = "true";
  static const char* __false_name = "false";

  char _Locale_decimal_point(_Locale_numeric_t* lnum) {
    return lnum->decimal_point[0];
  }

  char _Locale_thousands_sep(_Locale_numeric_t* lnum) {
    return lnum->thousands_sep[0];
  }

  const char* _Locale_grouping(_Locale_numeric_t * lnum) {
    if (!lnum->grouping) return "";
    else return lnum->grouping;
  }

  const char * _Locale_true(_Locale_numeric_t * lnum) {
    (void*)lnum;
    return __true_name; /* NT does't provide information about this */
  }

  const char * _Locale_false(_Locale_numeric_t * lnum) {
    (void*)lnum;
    return __false_name; /* NT does't provide information about this */
  }


  /* Monetary */
  const char* _Locale_int_curr_symbol(_Locale_monetary_t * lmon)
  { return lmon->int_curr_symbol; }

  const char* _Locale_currency_symbol(_Locale_monetary_t * lmon)
  { return lmon->curr_symbol; }

  char _Locale_mon_decimal_point(_Locale_monetary_t * lmon)
  { return lmon->decimal_point[0]; }

  char _Locale_mon_thousands_sep(_Locale_monetary_t * lmon)
  { return lmon->thousands_sep[0]; }

  const char* _Locale_mon_grouping(_Locale_monetary_t * lmon) {
    if (!lmon->grouping) return "";
    else return lmon->grouping;
  }

  const char* _Locale_positive_sign(_Locale_monetary_t * lmon)
  { return lmon->positive_sign; }

  const char* _Locale_negative_sign(_Locale_monetary_t * lmon)
  { return lmon->negative_sign; }

  char _Locale_int_frac_digits(_Locale_monetary_t * lmon)
  { return (char)lmon->int_frac_digits; }

  char _Locale_frac_digits(_Locale_monetary_t * lmon)
  { return (char)lmon->frac_digits; }

  int _Locale_p_cs_precedes(_Locale_monetary_t * lmon) {
    char loc_data[2];
    GetLocaleInfoA(lmon->lc.id, LOCALE_IPOSSYMPRECEDES, loc_data, 2);
    if (loc_data[0] == '0') return 0;
    else if (loc_data[0] == '1') return 1;
    else return -1;
  }

  int _Locale_p_sep_by_space(_Locale_monetary_t * lmon) {
    char loc_data[2];
    GetLocaleInfoA(lmon->lc.id, LOCALE_IPOSSEPBYSPACE, loc_data, 2);
    if (loc_data[0] == '0') return 0;
    else if (loc_data[0] == '1') return 1;
    else return -1;
  }

  int _Locale_p_sign_posn(_Locale_monetary_t * lmon) {
    char loc_data[2];
    GetLocaleInfoA(lmon->lc.id, LOCALE_IPOSSIGNPOSN, loc_data, 2);
    return atoi(loc_data);
  }

  int _Locale_n_cs_precedes(_Locale_monetary_t * lmon) {
    char loc_data[2];
    GetLocaleInfoA(lmon->lc.id, LOCALE_INEGSYMPRECEDES, loc_data, 2);
    if (loc_data[0] == '0') return 0;
    else if (loc_data[0] == '1') return 1;
    else return -1;
  }

  int _Locale_n_sep_by_space(_Locale_monetary_t * lmon) {
    char loc_data[2];
    GetLocaleInfoA(lmon->lc.id, LOCALE_INEGSEPBYSPACE, loc_data, 2);
    if (loc_data[0] == '0') return 0;
    else if (loc_data[0] == '1') return 1;
    else return -1;
  }

  int _Locale_n_sign_posn(_Locale_monetary_t * lmon) {
    char loc_data[2];
    GetLocaleInfoA(lmon->lc.id, LOCALE_INEGSIGNPOSN, loc_data, 2);
    return atoi(loc_data);
  }


  /* Time */
  const char * _Locale_full_monthname(_Locale_time_t * ltime, int month) {
    const char **names = (const char**)ltime->month;
    return names[month];
  }

  const char * _Locale_abbrev_monthname(_Locale_time_t * ltime, int month) {
    const char **names = (const char**)ltime->abbrev_month;
    return names[month];
  }

  const char * _Locale_full_dayofweek(_Locale_time_t * ltime, int day) {
    const char **names = (const char**)ltime->dayofweek;
    return names[day];
  }

  const char * _Locale_abbrev_dayofweek(_Locale_time_t * ltime, int day) {
    const char **names = (const char**)ltime->abbrev_dayofweek;
    return names[day];
  }

const char* _Locale_d_t_fmt(_Locale_time_t* ltime)
{ return ltime->date_time_format; }

const char* _Locale_long_d_t_fmt(_Locale_time_t* ltime)
{ return ltime->long_date_time_format; }

const char* _Locale_d_fmt(_Locale_time_t* ltime)
{ return ltime->date_format; }

const char* _Locale_long_d_fmt(_Locale_time_t* ltime)
{ return ltime->long_date_format; }

const char* _Locale_t_fmt(_Locale_time_t* ltime)
{ return ltime->time_format; }

const char* _Locale_am_str(_Locale_time_t* ltime)
{ return ltime->am; }

const char* _Locale_pm_str(_Locale_time_t* ltime)
{ return ltime->pm; }


  /* Messages */

  int _Locale_catopen(_Locale_messages_t* __DUMMY_PAR1, const char* __DUMMY_PAR) {
    (void*)__DUMMY_PAR1;
    (void*)__DUMMY_PAR;
    return -1;
  }
  void _Locale_catclose(_Locale_messages_t* __DUMMY_PAR1, int __DUMMY_PAR) {
    (void*)__DUMMY_PAR1;
    (void*)&__DUMMY_PAR;
  }
  const char* _Locale_catgets(_Locale_messages_t* __DUMMY_PAR1, int __DUMMY_PAR2,
                              int __DUMMY_PAR3, int __DUMMY_PAR4,
                              const char *dfault) {
    (void*)__DUMMY_PAR1;
    (void*)&__DUMMY_PAR2;
    (void*)&__DUMMY_PAR3;
    (void*)&__DUMMY_PAR4;
    return dfault;
  }

#ifdef __cplusplus
} /* extern C */
_STLP_END_NAMESPACE
#endif

void __FixGrouping(char *grouping) {
  /* This converts NT version which uses '0' instead of 0, etc ; to ANSI */
  while (*grouping) {
    if (*grouping >= '0' && *grouping <= '9') {
      *grouping = *grouping - '0';
      ++grouping;
    }
    else if (*grouping == ';') {
      /* remove ';' */
      char *tmp = grouping;
      for (; *tmp; ++tmp)
        *tmp = *(tmp + 1);
    }
    else
      ++grouping;
  }
}

const char* __ConvertName(const char* lname, LOCALECONV* ConvTable, int TableSize) {
  int i;
  int cmp;
  int low = 0;
  int high = TableSize - 1;

  /*  typical binary search - do until no more to search or match */
  while (low <= high) {
    i = (low + high) / 2;

    if ((cmp = lstrcmpiA(lname, (*(ConvTable + i)).name)) == 0)
      return (*(ConvTable + i)).abbrev;
    else if (cmp < 0)
      high = i - 1;
    else
      low = i + 1;
  }
  return lname;
}

int __ParseLocaleString(const char* lname,
                        char* lang, char* ctry, char* page) {
  int param = 0;
  size_t len;
  size_t tmpLen;

  if (lname[0] == 0)
    return 0;

  /* We look for the first country separator '_' */
  len = strcspn(lname, "_");
  if (lname[len] == '_') {
    if (len == 0 || len > MAX_LANG_LEN) return -1; /* empty lang is invalid*/
    _STLP_STRNCPY(lang, MAX_LANG_LEN + 1, lname, len);
    lname += len + 1;
    ++param;
  }

  /* We look for the last code page separator '.' */
  len = -1;
  tmpLen = strcspn(lname, ".");
  while (lname[tmpLen] == '.') {
    len = tmpLen; ++tmpLen;
    tmpLen += strcspn(lname + tmpLen, ".");
  }
  if (len != -1) { /* Means that we found a '.' */
    if (param == 0) {
      /* We have no lang yet so we have to fill it first, no country */
      if (len > MAX_LANG_LEN) return -1;
      if (len == 0) {
        /* No language nor country, only code page */
        ++param;
      }
      else
      { _STLP_STRNCPY(lang, MAX_LANG_LEN + 1, lname, len); }
      ++param;
    }
    else {
      /* We already have a lang so we are now looking for the country: */
      if (len == 0) return -1; /* We forbid locale name with the "_." motif in it */
      if (len > MAX_CTRY_LEN) return -1;
      _STLP_STRNCPY(ctry, MAX_CTRY_LEN + 1, lname, len);
    }
    ++param;
    lname += len + 1;
  }

  /* We look for ',' for compatibility with POSIX */
  len = strcspn(lname, ",");
  switch (param) {
    case 0:
      if (len > MAX_LANG_LEN) return -1;
      _STLP_STRNCPY(lang, MAX_LANG_LEN + 1, lname, len);
      break;
    case 1:
      if (len > MAX_CTRY_LEN) return -1;
      _STLP_STRNCPY(ctry, MAX_CTRY_LEN + 1, lname, len);
      break;
    default:
      if (len > MAX_CP_LEN) return -1;
      _STLP_STRNCPY(page, MAX_CP_LEN + 1, lname, len);
      break;
  }

  /* ',' POSIX modifier is not used in NT */
  return 0;
}

/* Data necessary for find LCID*/
static CRITICAL_SECTION __criticalSection;
static int __FindFlag;
static LCID __FndLCID;
static const char* __FndLang;
static const char* __FndCtry;

void _Locale_init()
{ InitializeCriticalSection(&__criticalSection); }

void _Locale_final()
{ DeleteCriticalSection(&__criticalSection); }

static LCID LocaleFromHex(const char* locale) {
  unsigned long result = 0;
  int digit;
  while (*locale) {
    result <<= 4;
    digit = (*locale >= '0' && *locale <= '9') ? *locale - '0':
            (*locale >= 'A' && *locale <= 'F') ? (*locale - 'A') + 10
                                               : (*locale - 'a') + 10;
    result += digit;
    ++locale;
  }
  return (LCID)result;
}

static BOOL CALLBACK EnumLocalesProcA(LPSTR locale) {
  LCID lcid = LocaleFromHex(locale);
  int LangFlag = 0, CtryFlag = !__FndCtry;
  static char Lang[MAX_LANG_LEN], Ctry[MAX_CTRY_LEN];

  GetLocaleInfoA(lcid, LOCALE_SENGLANGUAGE, Lang, MAX_LANG_LEN);
  if (lstrcmpiA(Lang, __FndLang) != 0) {
    GetLocaleInfoA(lcid, LOCALE_SABBREVLANGNAME, Lang, MAX_LANG_LEN);
    if (lstrcmpiA(Lang, __FndLang) != 0) {
      GetLocaleInfoA(lcid, LOCALE_SISO639LANGNAME, Lang, MAX_LANG_LEN);
      if (lstrcmpiA(Lang, __FndLang) == 0) LangFlag = 1;
    }
    else LangFlag = 1;
  }
  else LangFlag = 1;

  if (__FndCtry) {
    GetLocaleInfoA(lcid, LOCALE_SENGCOUNTRY, Ctry, MAX_CTRY_LEN);
    if (lstrcmpiA(Ctry, __FndCtry) != 0) {
      GetLocaleInfoA(lcid, LOCALE_SABBREVCTRYNAME, Ctry, MAX_CTRY_LEN);
      if (lstrcmpiA(Ctry, __FndCtry) != 0) {
        GetLocaleInfoA(lcid, LOCALE_SISO3166CTRYNAME, Ctry, MAX_CTRY_LEN);
        if (lstrcmpiA(Ctry, __FndCtry) == 0) CtryFlag = 1;
      }
      else CtryFlag = 1;
    }
    else
      CtryFlag = 1;
  }

  if (LangFlag && CtryFlag) {
    __FindFlag = 1;
    __FndLCID = lcid;
    return FALSE;
  }

  return TRUE;
}

int __GetLCID(const char* lang, const char* ctry, LCID* lcid) {
  int ret;
  EnterCriticalSection(&__criticalSection);

  __FindFlag = 0;
  __FndLang = lang;
  __FndCtry = ctry;
  EnumSystemLocalesA(EnumLocalesProcA, LCID_INSTALLED);

  if (__FindFlag != 0) *lcid = __FndLCID;
  ret = __FindFlag != 0 ? 0 : -1;

  LeaveCriticalSection(&__criticalSection);
  return ret;
}

int __GetLCIDFromName(const char* lname, LCID* lcid, char* cp, _Locale_lcid_t *hint) {
  char lang[MAX_LANG_LEN + 1], ctry[MAX_CTRY_LEN + 1], page[MAX_CP_LEN + 1];
  int result = 0;
  if (lname == NULL || lname[0] == 0) {
    *lcid = LOCALE_USER_DEFAULT;
    return 0;
  }

  memset(lang, 0, MAX_LANG_LEN + 1);
  memset(ctry, 0, MAX_CTRY_LEN + 1);
  memset(page, 0, MAX_CP_LEN + 1);
  if (__ParseLocaleString(lname, lang, ctry, page) == -1) return -1;

  if (hint != 0) {
    *lcid = hint->id;
  }
  else {
    if (lang[0] == 0 && ctry[0] == 0)
      *lcid = LOCALE_USER_DEFAULT; /* Only code page given. */
    else {
      if (ctry[0] == 0)
        result = __GetLCID(__ConvertName(lang, __rg_language, sizeof(__rg_language) / sizeof(LOCALECONV)), NULL, lcid);
      else {
        result = __GetLCID(__ConvertName(lang, __rg_language, sizeof(__rg_language) / sizeof(LOCALECONV)),
                           __ConvertName(ctry, __rg_country, sizeof(__rg_country) / sizeof(LOCALECONV)),
                           lcid);
        if (result != 0) {
          /* Non NLS mapping might introduce problem with some locales when only one entry is mapped,
          * the lang or the country (example: chinese locales like 'chinese_taiwan' gives 'CHS_taiwan'
          * that do not exists in system). This is why we are giving this locale an other chance by
          * calling __GetLCID without the mapping. */
          result = __GetLCID(lang, ctry, lcid);
        }
      }
    }
  }

  if (result == 0) {
    /* Handling code page */
    if (lstrcmpiA(page, "ACP") == 0 || page[0] == 0)
      my_ltoa(__intGetACP(*lcid), cp);
    else if (lstrcmpiA(page, "OCP") == 0)
      my_ltoa(__intGetOCP(*lcid), cp);
    else
      _STLP_STRNCPY(cp, MAX_CP_LEN + 1, page, 5);

    /* Code page must be an integer value,
     * 0 returned by __intGetACP and 1 returned by __intGetOCP are invalid
     * values.
     */
    if (cp[1] == 0 && (cp[0] == '0' || cp[1] == '1'))
      return -1;
    else if (atoi(cp) == 0)
      return -1;
  }

  return result;
}

char const* __GetLocaleName(LCID lcid, const char* cp, char* buf) {
  char lang[MAX_LANG_LEN + 1], ctry[MAX_CTRY_LEN + 1];
  GetLocaleInfoA(lcid, LOCALE_SENGLANGUAGE, lang, MAX_LANG_LEN);
  GetLocaleInfoA(lcid, LOCALE_SENGCOUNTRY, ctry, MAX_CTRY_LEN);
  _STLP_STRCPY2(buf, _Locale_MAX_SIMPLE_NAME, lang);
  _STLP_STRCAT2(buf, _Locale_MAX_SIMPLE_NAME, "_");
  _STLP_STRCAT2(buf, _Locale_MAX_SIMPLE_NAME, ctry);
  _STLP_STRCAT2(buf, _Locale_MAX_SIMPLE_NAME, ".");
  _STLP_STRCAT2(buf, _Locale_MAX_SIMPLE_NAME, cp);
  return buf;
}

static const char* __loc_categories[]= {
  "LC_ALL", "LC_COLLATE", "LC_CTYPE", "LC_MONETARY", "LC_NUMERIC", "LC_TIME"
};

char const* __Extract_locale_name(const char* loc, int category, char* buf) {
  char *expr;
  size_t len_name;
  buf[0] = 0;
#if defined (__BORLANDC__)
  if (category < LC_MIN || category > LC_MAX) return NULL;
  switch (category) {
    case 0xFF: category = 0; break;
    case 0x01: category = 1; break;
    case 0x02: category = 2; break;
    case 0x04: category = 3; break;
    case 0x10: category = 4; break;
    case 0x20: category = 5; break;
    default  : category = 0;
  }
#else
  if (category < LC_ALL || category > LC_MAX) return NULL;
#endif

  if (loc[0] == 'L' && loc[1] == 'C' && loc[2] == '_') {
    expr = strstr((char*)loc, __loc_categories[category]);
    if (expr == NULL) return NULL; /* Category not found. */
    expr = strchr(expr, '=');
    if (expr == NULL) return NULL;
    ++expr;
    len_name = strcspn(expr, ";");
    len_name = len_name > _Locale_MAX_SIMPLE_NAME ? _Locale_MAX_SIMPLE_NAME
                                                  : len_name;
    _STLP_STRNCPY(buf, _Locale_MAX_SIMPLE_NAME, expr, len_name); buf[len_name] = 0;
    return buf;
  }
  else {
    _STLP_STRNCPY(buf, _Locale_MAX_SIMPLE_NAME, loc, _Locale_MAX_SIMPLE_NAME);
    return buf;
  }
}

char const* __TranslateToSystem(const char* lname, char* buf, _Locale_lcid_t* hint) {
  LCID lcid;
  char cp[MAX_CP_LEN + 1];
  if (__GetLCIDFromName(lname, &lcid, cp, hint) != 0) return NULL;

  return __GetLocaleName(lcid, cp, buf);
}

void __GetLocaleInfoUsingACP(LCID lcid, const char* cp, LCTYPE lctype, char* buf, int buf_size) {
  wchar_t *Buffer;
  int BufferSize;

  GetLocaleInfoA(lcid, lctype, buf, buf_size);

  BufferSize = MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
  Buffer = (wchar_t*)malloc(sizeof(wchar_t) * (BufferSize + 1));
  MultiByteToWideChar(CP_ACP, 0, buf, -1, Buffer, BufferSize);
  WideCharToMultiByte(atoi(cp), 0, Buffer, -1, buf, buf_size, NULL, NULL);
  free(Buffer);
}

/* Return 0 if ANSI code page not used */
int __intGetACP(LCID lcid) {
  char cp[6];
  GetLocaleInfoA(lcid, LOCALE_IDEFAULTANSICODEPAGE, cp, 6);
  return atoi(cp);
}

/* Return 1 if OEM code page not used */
int __intGetOCP(LCID lcid) {
  char cp[6];
  GetLocaleInfoA(lcid, LOCALE_IDEFAULTCODEPAGE, cp, 6);
  return atoi(cp);
}

int __GetDefaultCP(LCID lcid) {
  int cp = __intGetACP(lcid);
  if (cp == 0) return __intGetOCP(lcid);
  else return cp;
}

static int trim_size_t_to_int(size_t n) { return n < (size_t)INT_MAX ? (int)n : INT_MAX; }

char* __ConvertToCP(int from_cp, int to_cp, const char *from, size_t size, size_t *ret_buf_size) {
  size_t wbuffer_size, buffer_size, from_offset, wbuf_offset;
  int from_size, to_size, wbuf_size;
  wchar_t *wbuffer;
  char* buffer;

  size_t orig_size = size;

  wbuffer_size = 0;
  from_offset = 0;
  while (size > 0) {
    from_size = trim_size_t_to_int(size);
    wbuffer_size += MultiByteToWideChar(from_cp, MB_PRECOMPOSED,
                                        from + from_offset, from_size, NULL, 0);
    from_offset += from_size;
    size -= from_size;
  }

  wbuffer = (wchar_t*)malloc(sizeof(wchar_t)*wbuffer_size);

  size = orig_size;
  wbuf_offset = 0;
  from_offset = 0;
  while (size > 0) {
    from_size = trim_size_t_to_int(size);
    wbuf_size = trim_size_t_to_int(wbuffer_size - wbuf_offset);
    wbuf_offset += MultiByteToWideChar(from_cp, MB_PRECOMPOSED,
                                       from + from_offset, from_size, wbuffer + wbuf_offset, wbuf_size);
    from_offset += from_size;
    size -= from_size;
  }

  buffer_size = 0;
  wbuf_offset = 0;
  size = wbuffer_size;
  while (size > 0) {
    wbuf_size = trim_size_t_to_int(size);
    buffer_size += WideCharToMultiByte(to_cp, WC_COMPOSITECHECK | WC_SEPCHARS,
                                       wbuffer + wbuf_offset, wbuf_size,
                                       NULL, 0, NULL, FALSE);
    wbuf_offset += wbuf_size;
    size -= wbuf_size;
  }

  buffer = (char*)malloc(buffer_size);
  *ret_buf_size = buffer_size;

  size = wbuffer_size;
  wbuf_offset = 0;
  while (size > 0) {
    wbuf_size = trim_size_t_to_int(size);
    to_size = trim_size_t_to_int(buffer_size);
    buffer_size -= WideCharToMultiByte(to_cp, WC_COMPOSITECHECK | WC_SEPCHARS,
                                       wbuffer + wbuf_offset, wbuf_size,
                                       buffer, to_size, NULL, FALSE);
    wbuf_offset += wbuf_size;
    size -= wbuf_size;
  }

  free(wbuffer);
  return buffer;
}

#ifdef __cplusplus
}
#endif

