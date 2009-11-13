/*
 * Copyright (c) 1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */


#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef _POSIX_MAPPED_FILES
# include <sys/mman.h>
#endif

#include <stl/c_locale.h>
#include <limits.h>
#include <wctype.h>
/* #include <libc-lock.h> */

#include <locale.h>
#include <argz.h>
#include "gcc_localeinfo.h"

wint_t btowc(int c);
int wctob (wint_t c);

size_t mbrtowc (wchar_t *pwc, const char *s, size_t n, mbstate_t *ps);
size_t wcrtomb (char *s, wchar_t wc, mbstate_t *ps);
size_t mbrlen (const char* s, size_t n, mbstate_t *ps);

#include <nl_types.h>

void _Locale_init()
{}

void _Locale_final()
{}

typedef struct _Locale_ctype    {
  const struct locale_data* gcc_data;
  const int* __tolower;
  const int* __toupper;
  _Locale_mask_t* __class;
} L_ctype_t;

typedef struct _Locale_numeric  {
  const struct locale_data* gcc_data;
} L_numeric_t;

typedef struct _Locale_time     {
  const struct locale_data* gcc_data;
} L_time_t;

typedef struct _Locale_collate  {
  const struct locale_data* gcc_data;
} L_collate_t;

typedef struct _Locale_monetary {
  const struct locale_data* gcc_data;
} L_monetary_t;

typedef struct _Locale_messages {
  const struct locale_data* gcc_data;
} L_messages_t;

struct _Locale_name_hint* _Locale_get_ctype_hint(struct _Locale_ctype* ctype)
{ return 0; }
struct _Locale_name_hint* _Locale_get_numeric_hint(struct _Locale_numeric* numeric)
{ return 0; }
struct _Locale_name_hint* _Locale_get_time_hint(struct _Locale_time* time)
{ return 0; }
struct _Locale_name_hint* _Locale_get_collate_hint(struct _Locale_collate* collate)
{ return 0; }
struct _Locale_name_hint* _Locale_get_monetary_hint(struct _Locale_monetary* monetary)
{ return 0; }
struct _Locale_name_hint* _Locale_get_messages_hint(struct _Locale_messages* messages)
{ return 0; }

static char const*
_Locale_extract_name ( const char *cname, char *into, int category )
{
  int i = 0;
  const char * end;

  if ( cname[0] != '/' )
    return strcpy(into, cname); /* simple locale name */

  for ( i = 0; i <= category; i ++ ) {
    while ( *cname != '\0' && *cname != '/' )
      cname++;
    if ( *cname == '\0' )
      return into;
    cname++;
  }

  if ( *cname == '\0' )
    return into;

  end = cname;
  while ( *end != '\0' && *end != '/' )
    end++;

  strncpy ( into, cname, end - cname );
  into [ end - cname ] = '\0';

  return into;
}

char const* _Locale_name(const struct locale_data* gcc_data,
            char* buf)
{
  if (!(gcc_data && gcc_data->name)) return 0;
  strncpy(buf, gcc_data->name, _Locale_MAX_SIMPLE_NAME);
  buf [ _Locale_MAX_SIMPLE_NAME - 1 ] = '\0';
  return buf;
}


/* calls _nl_find_locale which is a function internal to the glibc
    locale implementation that loads locale data in from the data
    files.  The locale_data struct has information for all categories.
    In the following implementation we use a locale_data struct for
    each category for simplicity, though there is an obvious waste in
    doing that.  */
const struct locale_data *
_Find_locale (char *locale_path, size_t locale_path_len,
        int category, char **name)
{
  return __nl_find_locale(locale_path, locale_path_len, category, name);
}


static void
_Remove_locale (int locale, struct locale_data *data)
{
  /* this should eventually call _nl_remove_locale() in glibc 2.1 */
  /* _nl_remove_locale( locale, data ); */
}

/* couldn't find where LOCALE_PATH was defined in glibc,
   but this is the directory it is defined to -JGS */
#define __LOCALE_PATH "/usr/share/locale"

const struct locale_data*
_Category_create(const char * name, int category)
{
  /* JGS, where should this path come from? */
  char* locpath_var;
  char* locale_path = NULL;
  size_t locale_path_len = 0;

  locpath_var = __secure_getenv("LOCPATH");

  if (locpath_var != NULL && locpath_var[0] != '\0')
    if (argz_create_sep (locpath_var, ':',
       &locale_path, &locale_path_len) != 0)
      return NULL;

  if (argz_add_sep (&locale_path, &locale_path_len, __LOCALE_PATH, ':') != 0)
    return NULL;

  return _Find_locale(locale_path, locale_path_len,
          category, (char**)&name);
}



static const char* get_default_locale(char* buf) {
  char* lang = getenv("LANG");
  if (lang == NULL || lang[0] == '\0') {
    buf[0] = '\0';
    return NULL;
  }
  else {
    strcpy(buf, lang);
    return buf;
  }
}

const char* _Locale_ctype_default(char* buf) {
  char fullname[_Locale_MAX_COMPOSITE_NAME];
  if (get_default_locale(fullname) == NULL)
    return NULL;
  else
    return _Locale_extract_ctype_name(fullname, buf, 0);
}

const char* _Locale_numeric_default(char* buf) {
  char fullname[_Locale_MAX_COMPOSITE_NAME];
  if (get_default_locale(fullname) == NULL)
    return NULL;
  else
    return _Locale_extract_numeric_name(fullname, buf, 0);
}

const char* _Locale_time_default(char* buf) {
  char fullname[_Locale_MAX_COMPOSITE_NAME];
  if (get_default_locale(fullname) == NULL)
    return NULL;
  else
    return _Locale_extract_time_name(fullname, buf, 0);
}

const char* _Locale_collate_default(char* buf)  {
  char fullname[_Locale_MAX_COMPOSITE_NAME];
  if (get_default_locale(fullname) == NULL)
    return NULL;
  else
    return _Locale_extract_collate_name(fullname, buf, 0);
}

const char* _Locale_monetary_default(char* buf) {
  char fullname[_Locale_MAX_COMPOSITE_NAME];
  if (get_default_locale(fullname) == NULL)
    return NULL;
  else
    return _Locale_extract_monetary_name(fullname, buf, 0);
}

const char* _Locale_messages_default(char* buf) {
  char fullname[_Locale_MAX_COMPOSITE_NAME];
  if (get_default_locale(fullname) == NULL)
    return NULL;
  else
    return _Locale_extract_messages_name(fullname, buf, 0);
}


/****** Numeric Category ******/

void*
_Locale_numeric_create(const char * name, struct _Locale_name_hint* hint) {
  L_numeric_t*  lnum = (L_numeric_t*)malloc(sizeof(L_numeric_t));
  lnum->gcc_data = _Category_create(name, LC_NUMERIC);
  return (void*)lnum;
}


char const* _Locale_numeric_name(const void* lnum,
              char* buf) {
  return _Locale_name(((struct _Locale_ctype*)lnum)->gcc_data, buf);
}
void _Locale_numeric_destroy(void* lnum)
{
  _Remove_locale(LC_NUMERIC, (struct locale_data *)((struct _Locale_ctype*)lnum)->gcc_data);
  free(lnum);
}
char const* _Locale_extract_numeric_name(const char* cname, char* buf, struct _Locale_name_hint* hint)
{
  return _Locale_extract_name(cname, buf, LC_NUMERIC);
}
char _Locale_decimal_point(struct _Locale_numeric* lnum)
{
  return lnum->gcc_data->values[_NL_ITEM_INDEX(DECIMAL_POINT)].string[0];
}
char _Locale_thousands_sep(struct _Locale_numeric* lnum)
{
  return lnum->gcc_data->values[_NL_ITEM_INDEX(THOUSANDS_SEP)].string[0];
}
const char* _Locale_grouping(struct _Locale_numeric * lnum)
{
  return lnum->gcc_data->values[_NL_ITEM_INDEX(GROUPING)].string;
}

/* JGS: gcc/linux does not provide true/false names in their
 * locale data files
*/

static const char* __true_name = "true";
static const char* __false_name = "false";

const char * _Locale_true(struct _Locale_numeric *l)
{ return __true_name; }
const char * _Locale_false(struct _Locale_numeric *l)
{ return __false_name; }


/****** Monetary Category ******/

void* _Locale_monetary_create(const char* name, struct _Locale_name_hint* hint) {
  L_monetary_t* lmon = (L_monetary_t*)malloc(sizeof(L_monetary_t));
  lmon->gcc_data = _Category_create(name, LC_MONETARY);
  return lmon;
}

char const* _Locale_monetary_name(const void* lmon,
          char* buf) {
  return _Locale_name(((struct _Locale_monetary*)lmon)->gcc_data, buf);
}

void _Locale_monetary_destroy(void*lmon) {
  _Remove_locale(LC_MONETARY, (struct locale_data *)((struct _Locale_monetary*)lmon)->gcc_data);
  free(lmon);
}

char const* _Locale_extract_monetary_name(const char* cname, char* buf, struct _Locale_name_hint* hint) {
  return _Locale_extract_name(cname, buf, LC_MONETARY);
}

const char* _Locale_int_curr_symbol(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(INT_CURR_SYMBOL)].string;
}
const char* _Locale_currency_symbol(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(CURRENCY_SYMBOL)].string;
}
char        _Locale_mon_decimal_point(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(MON_DECIMAL_POINT)].string[0];
}
char        _Locale_mon_thousands_sep(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(MON_THOUSANDS_SEP)].string[0];
}
const char* _Locale_mon_grouping(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(MON_GROUPING)].string;
}
const char* _Locale_positive_sign(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(POSITIVE_SIGN)].string;
}
const char* _Locale_negative_sign(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(NEGATIVE_SIGN)].string;
}
char        _Locale_int_frac_digits(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(INT_FRAC_DIGITS)].string[0];
}
char        _Locale_frac_digits(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(FRAC_DIGITS)].string[0];
}
int         _Locale_p_cs_precedes(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(P_CS_PRECEDES)].word;
}
int         _Locale_p_sep_by_space(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(P_SEP_BY_SPACE)].word;
}
int         _Locale_p_sign_posn(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(P_SIGN_POSN)].word;
}
int         _Locale_n_cs_precedes(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(N_CS_PRECEDES)].word;
}
int          _Locale_n_sep_by_space(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(N_SEP_BY_SPACE)].word;
}
int          _Locale_n_sign_posn(struct _Locale_monetary* lmon) {
  return lmon->gcc_data->values[_NL_ITEM_INDEX(N_SIGN_POSN)].word;
}

/****** Time Category ******/

void* _Locale_time_create(const char * name, struct _Locale_name_hint* hint) {
  L_time_t*  ltime = (L_time_t*)malloc(sizeof(L_time_t));
  ltime->gcc_data = _Category_create(name, LC_TIME);
  return ltime;
}

char const* _Locale_time_name(const void* ltime,
      char* buf) {
  return _Locale_name(((struct _Locale_time*)ltime)->gcc_data, buf);
}
char const* _Locale_extract_time_name(const char* cname, char* buf, struct _Locale_name_hint* hint) {
  return _Locale_extract_name(cname, buf, LC_TIME);
}
void _Locale_time_destroy(void* ltime) {
  _Remove_locale(LC_TIME, (struct locale_data *)((struct _Locale_time*)ltime)->gcc_data);
  free(ltime);
}
const char * _Locale_full_monthname(struct _Locale_time *ltime, int month) {
  const char **names = (const char **)&(ltime->gcc_data->values[_NL_ITEM_INDEX(MON_1)]);
  return names[month];
}
const char * _Locale_abbrev_monthname(struct _Locale_time *ltime, int month) {
  const char **names = (const char **)&(ltime->gcc_data->values[_NL_ITEM_INDEX(ABMON_1)]);
  return names[month];
}
const char * _Locale_full_dayofweek(struct _Locale_time *ltime, int day) {
  const char **names = (const char **)&(ltime->gcc_data->values[_NL_ITEM_INDEX(DAY_1)]);
  return names[day];
}
const char * _Locale_abbrev_dayofweek(struct _Locale_time *ltime, int day) {
  const char **names = (const char **)&(ltime->gcc_data->values[_NL_ITEM_INDEX(ABDAY_1)]);
  return names[day];
}
const char* _Locale_d_t_fmt(struct _Locale_time* ltime) {
  return ltime->gcc_data->values[_NL_ITEM_INDEX(D_T_FMT)].string;
}
const char* _Locale_long_d_t_fmt(struct _Locale_time* ltime) {
  return ltime->gcc_data->values[_NL_ITEM_INDEX(D_T_FMT)].string;
}
const char* _Locale_d_fmt(struct _Locale_time* ltime)
{
  return ltime->gcc_data->values[_NL_ITEM_INDEX(D_FMT)].string;
}
const char* _Locale_long_d_fmt(struct _Locale_time* ltime)
{
  return ltime->gcc_data->values[_NL_ITEM_INDEX(D_FMT)].string;
}
const char* _Locale_t_fmt(struct _Locale_time* ltime) {
  return ltime->gcc_data->values[_NL_ITEM_INDEX(T_FMT)].string;
}
const char* _Locale_am_str(struct _Locale_time* ltime) {
  return ltime->gcc_data->values[_NL_ITEM_INDEX(AM_STR)].string;
}
const char* _Locale_pm_str(struct _Locale_time* ltime) {
  return ltime->gcc_data->values[_NL_ITEM_INDEX(PM_STR)].string;
}
const char* _Locale_t_fmt_ampm(struct _Locale_time* ltime)
{
  return ltime->gcc_data->values[_NL_ITEM_INDEX(T_FMT_AMPM)].string;
}


/****** Messages Category ******/

void* _Locale_messages_create(const char * name, struct _Locale_name_hint* hint) {
  L_messages_t*  lmsg = (L_messages_t*)malloc(sizeof(L_messages_t));
  lmsg->gcc_data = _Category_create(name, LC_MESSAGES);
  return lmsg;
}

char const* _Locale_messages_name(const void* lmsg, char* buf) {
  return _Locale_name(((struct _Locale_messages*)lmsg)->gcc_data, buf);
}

void _Locale_messages_destroy(void* lmsg) {
  _Remove_locale(LC_MESSAGES, (struct locale_data *)((struct _Locale_messages*)lmsg)->gcc_data);
  free(lmsg);
}

char const* _Locale_extract_messages_name(const char* cname, char* buf, struct _Locale_name_hint* hint) {
  return _Locale_extract_name(cname, buf, LC_MESSAGES);
}


/*
  Could not find support for locale specific messages in glibc

  Also, this C locale interface should use a typedef for the catalog
  instead of just an int. Currently I'm casting a void* (nl_catd)
  back and forth to and int.

  -JGS
 */

int _Locale_catopen(struct _Locale_messages*l, const char* cat_name) {
  return (int)catopen(cat_name, 0); /*  JGS, don't know about the flag */
}
void _Locale_catclose(struct _Locale_messages*l, int catalog) {
  catclose((nl_catd)catalog);
}
const char* _Locale_catgets(struct _Locale_messages*l, int catalog,
                                       int set_num, int msg_num,
                                       const char *dfault){
  return catgets((nl_catd)catalog, set_num, msg_num, dfault);
}


/****** ctype Category ******/


/*
  gcc uses a different set of masks for wide characters than for
  normal characters. However, the C++ standard requires there
  to be only one set of masks for both. Therefore we must
  translate the mask values from the wide characters to the
  mask values for the normal characters.  -JGS
 */
static _Locale_mask_t _Map_wchar_mask_to_char_mask(wctype_t m) {
  _Locale_mask_t ret = 0;
  if (m & _ISwcntrl)  ret |= _Locale_CNTRL;
  if (m & _ISwupper)  ret |= _Locale_UPPER;
  if (m & _ISwlower)  ret |= _Locale_LOWER;
  if (m & _ISwdigit)  ret |= _Locale_DIGIT;
  if (m & _ISwxdigit) ret |= _Locale_XDIGIT;
  if (m & _ISwpunct)  ret |= _Locale_PUNCT;
  if (m & _ISwspace)  ret |= _Locale_SPACE;
  if (m & _ISwprint)  ret |= _Locale_PRINT;
  if (m & _ISwalpha)  ret |= _Locale_ALPHA;
  return ret;
}


void* _Locale_ctype_create(const char * name, struct _Locale_name_hint* hint) {
  const union locale_data_value *ctypes;
  L_ctype_t* lctype;

  lctype = (L_ctype_t*)malloc(sizeof(L_ctype_t));
  lctype->gcc_data = _Category_create(name, LC_CTYPE);
  ctypes = lctype->gcc_data->values;

  lctype->__class = (_Locale_mask_t *)
    (ctypes[_NL_ITEM_INDEX (_NL_CTYPE_CLASS)] .string) + 128;
#ifdef _STLP_GLIBC_LOCALE_2
  lctype->__tolower = (const int *)
    (ctypes[_NL_ITEM_INDEX (_NL_CTYPE_TOLOWER)].string) + 128;
  lctype->__toupper = (const int *)
    (ctypes[_NL_ITEM_INDEX (_NL_CTYPE_TOUPPER)].string) + 128;
#else
# if BYTE_ORDER == BIG_ENDIAN
  lctype->__tolower = (const int *)
    (ctypes[_NL_ITEM_INDEX (_NL_CTYPE_TOLOWER_EB)].string) + 128;
  lctype->__toupper = (const int *)
    (ctypes[_NL_ITEM_INDEX (_NL_CTYPE_TOUPPER_EB)].string) + 128;
# elif BYTE_ORDER == LITTLE_ENDIAN
  lctype->__tolower = (const int *)
    (ctypes[_NL_ITEM_INDEX (_NL_CTYPE_TOLOWER_EL)].string) + 128;
  lctype->__toupper = (const int *)
    (ctypes[_NL_ITEM_INDEX (_NL_CTYPE_TOUPPER_EL)].string) + 128;
# else
#  error bizarre byte order
# endif
#endif /* _STLP_GLIBC_LOCALE_2 */
  return lctype;
}
char const* _Locale_ctype_name(const void* lctype,
       char* buf) {
  return _Locale_name(((struct _Locale_ctype*)lctype)->gcc_data, buf);
}
void _Locale_ctype_destroy(void* lctype) {
  _Remove_locale(LC_CTYPE, (struct locale_data *)((struct _Locale_ctype*)lctype)->gcc_data);
  free(lctype);
}
char const* _Locale_extract_ctype_name(const char* cname, char* buf, struct _Locale_name_hint* hint) {
  return _Locale_extract_name(cname, buf, LC_CTYPE);
}
const _Locale_mask_t* _Locale_ctype_table(struct _Locale_ctype* lctype) {
  return lctype->__class;
}
int _Locale_toupper(struct _Locale_ctype* lctype, int c) {
  return lctype->__toupper[c];
}
int _Locale_tolower(struct _Locale_ctype* lctype, int c) {
  return lctype->__tolower[c];
}

/* Wide Character Functions */

static inline size_t
cname_lookup (wint_t wc, const struct locale_data* loc)
{
#ifdef _STLP_GLIBC_LOCALE_2
  printf( "******** Fix me: %s:%d", __FILE__, __LINE__ );
  return ~((size_t) 0);
#else
  unsigned int *__nl_ctype_names;
  unsigned int hash_size, hash_layers;
  size_t result, cnt;

# if BYTE_ORDER == BIG_ENDIAN
  __nl_ctype_names = (unsigned int*)loc->values[_NL_ITEM_INDEX(_NL_CTYPE_NAMES_EB)].string;
# elif BYTE_ORDER == LITTLE_ENDIAN
  __nl_ctype_names = (unsigned int*)loc->values[_NL_ITEM_INDEX(_NL_CTYPE_NAMES_EL)].string;
# else
#  error bizarre byte order
# endif

  hash_size = loc->values[_NL_ITEM_INDEX(_NL_CTYPE_HASH_SIZE)].word;
  hash_layers = loc->values[_NL_ITEM_INDEX(_NL_CTYPE_HASH_LAYERS)].word;

  result = wc % hash_size;
  for (cnt = 0; cnt < hash_layers; ++cnt) {
    if (__nl_ctype_names[result] == wc)
      break;
    result += hash_size;
  }
  return cnt < hash_layers ? result : ~((size_t) 0);
#endif
}




_Locale_mask_t _Locale_wchar_ctype(struct _Locale_ctype* loc, wint_t wc,
  _Locale_mask_t which_bits) {
  const struct locale_data* locale = loc->gcc_data;
  const unsigned int *class32_b;
  size_t idx;

  idx = cname_lookup (wc, locale);
  if (idx == ~((size_t) 0))
    return 0;

  class32_b = (u_int32_t *)
    locale->values[_NL_ITEM_INDEX (_NL_CTYPE_CLASS32)].string;

  return _Map_wchar_mask_to_char_mask( class32_b[idx] ) & which_bits;
}



wint_t
__towctrans_ld (wint_t wc, wctrans_t desc, const struct locale_data* locale)
{
  size_t idx;

  idx = cname_lookup (wc, locale);
  if (idx == ~((size_t) 0))
    /* Character is not known.  Default action is to simply return it.  */
    return wc;

  return (wint_t) desc[idx];
}

wint_t _Locale_wchar_tolower(struct _Locale_ctype* locale, wint_t wc) {
  return __towctrans_ld (wc, (const unsigned int *)locale->__tolower, locale->gcc_data);
}
wint_t _Locale_wchar_toupper(struct _Locale_ctype* locale, wint_t wc) {
  return __towctrans_ld (wc, (const unsigned int *)locale->__toupper, locale->gcc_data);
}


int _Locale_mb_cur_max (struct _Locale_ctype *lctype) {
  return lctype->gcc_data->values[_NL_ITEM_INDEX(_NL_CTYPE_MB_CUR_MAX)].word;
}

int _Locale_mb_cur_min (struct _Locale_ctype *l) {
  return 1;  /* JGS just a guess */
}

int _Locale_is_stateless (struct _Locale_ctype *l) { return 1; }

wint_t _Locale_btowc(struct _Locale_ctype *l, int c) {
  return btowc(c);
}

/*
  glibc currently doesn't support locale dependent conversion,
  which affects the following functions. When it does, then
  these functions will need to change. Hopeully, the
  just the calls to the glibc functions will need to be
  replaced.
  -JGS
 */

int _Locale_wctob(struct _Locale_ctype *l, wint_t c) {
  return wctob(c);
}

size_t _Locale_mbtowc(struct _Locale_ctype *l,
                                 wchar_t *to,
                                 const char *from, size_t n,
                                 mbstate_t *shift_state)
{
  int ret;
  if (to)
    ret = mbrtowc(to, from, n, shift_state);
  else
    ret = mbrlen(from, n, shift_state);
  return ret;
}

size_t _Locale_wctomb(struct _Locale_ctype *l,
          char *to, size_t n,
          const wchar_t c,
          mbstate_t *shift_state)
{
  char buf [MB_LEN_MAX];
  int ret;
  char* mb = buf;
  ret = wcrtomb(mb, c, shift_state);

  if (ret > n)
    return (size_t)-2;
  else if (ret <= 0)
    return ret;

  n = ret;
  while (n--)
    *to++ = *mb++;

  return ret;
}

size_t _Locale_unshift(struct _Locale_ctype *l,
           mbstate_t * st,
           char *buf, size_t n, char **next) {
  *next = buf; /* JGS stateless, so don't need to do anything? */
  return 0;
}


/****** Collate Category ******/

void* _Locale_collate_create(const char * name, struct _Locale_name_hint* hint) {
  L_collate_t*  lcollate = (L_collate_t*)malloc(sizeof(L_collate_t));
  lcollate->gcc_data = _Category_create(name, LC_COLLATE);
  return lcollate;
}

char const* _Locale_collate_name(const void* lcollate, char* buf) {
  return _Locale_name(((struct _Locale_collate*)lcollate)->gcc_data, buf);
}

void _Locale_collate_destroy(void* lcollate) {
  _Remove_locale(LC_COLLATE, (struct locale_data *)((struct _Locale_collate*)lcollate)->gcc_data);
  free(lcollate);
}

char const* _Locale_extract_collate_name(const char* cname, char* buf, struct _Locale_name_hint* hint) {
  return _Locale_extract_name(cname, buf, LC_COLLATE);
}

/* copied from the IRIX version -JGS */
char const* _Locale_compose_name(char* buf,
         const char* ctype, const char* numeric,
         const char* time, const char* collate,
         const char* monetary, const char* messages,
         const char *default_name)
{
   (void) default_name;

    if ( !strcmp ( ctype, numeric ) &&
   !strcmp ( ctype, time ) &&
   !strcmp ( ctype, collate ) &&
   !strcmp ( ctype, monetary ) &&
   !strcmp ( ctype, messages ) )
  return strcpy ( buf, ctype );

    strcpy ( buf, "/" );
    strcat ( buf, ctype );

    strcat ( buf, "/" );
    strcat ( buf, numeric );

    strcat ( buf, "/" );
    strcat ( buf, time );

    strcat ( buf, "/" );
    strcat ( buf, collate );

    strcat ( buf, "/" );
    strcat ( buf, monetary );

    strcat ( buf, "/" );
    strcat ( buf, messages );

    return buf;
}



/*
  glibc doesn't have a locale specific strcmp
  This doesn't ignore null chars the way it should
 */
int
_Locale_strcmp(struct _Locale_collate * l,
         const char *s1, size_t n1,
         const char *s2, size_t n2)
{
  int ret;
  int minN = n1 < n2 ? n1 : n2;
  ret = strncmp(s1, s2, minN);
  if (ret == 0) {
    if (n1 < n2) return -1;
    else if (n1 > n2) return 1;
    else return 0;
  } else
    return ret;
}


int _Locale_strwcmp(struct _Locale_collate*l,
        const wchar_t*s1, size_t n1,
        const wchar_t*s2, size_t n2)
{
  int ret;
  int minN = n1 < n2 ? n1 : n2;
  ret = wcsncmp(s1, s2, minN);
  if (ret == 0) {
    if (n1 < n2) return -1;
    else if (n1 > n2) return 1;
    else return 0;
  } else
    return ret;
}
size_t _Locale_strxfrm(struct _Locale_collate *lcollate,
                       char *dest, size_t dest_n,
                       const char *src, size_t src_n )
{
  if (src_n == 0)
  {
    if (dest != NULL) dest[0] = 0;
    return 0;
  }
  const char *real_src;
  char *buf = NULL;
  if (src[src_n] != 0) {
    buf = malloc(src_n + 1);
    strncpy(buf, src, src_n);
    buf[src_n] = 0;
    real_src = buf;
  }
  else
    real_src = src;
  size_t result = strxfrm(dest, real_src, dest_n);
  if (buf != NULL) free(buf);
  return result;
}

# ifndef _STLP_NO_WCHAR_T

size_t _Locale_strwxfrm(struct _Locale_collate *lcollate,
                        wchar_t *dest, size_t dest_n,
                        const wchar_t *src, size_t src_n)
{
  if (src_n == 0)
  {
    if (dest != NULL) dest[0] = 0;
    return 0;
  }
  const wchar_t *real_src;
  wchar_t *buf = NULL;
  if (src[src_n] != 0) {
    buf = malloc((src_n + 1) * sizeof(wchar_t));
    wcsncpy(buf, src, src_n);
    buf[src_n] = 0;
    real_src = buf;
  }
  else
    real_src = src;
  size_t result = wcsxfrm(dest, real_src, dest_n, (__c_locale)__loc);
  if (buf != NULL) free(buf);
  return result;
}

#endif
