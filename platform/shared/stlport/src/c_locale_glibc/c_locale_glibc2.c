
#include <locale.h>
#include <langinfo.h>

#include <stl/c_locale.h>
#include <stdio.h>
#include <wctype.h>
#include <string.h>

#include <stdint.h>

/* Structure describing locale data in core for a category.  */
/* GLIBC internal, see <glibc catalog>/locale/localeinfo.h */
#if (__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ > 2))

/* GLIBC 2.3.x */
struct locale_data
{
  const char *name;
  const char *filedata;         /* Region mapping the file data.  */
  off_t filesize;               /* Size of the file (and the region).  */
  enum                          /* Flavor of storage used for those.  */
  {
    ld_malloced,                /* Both are malloc'd.  */
    ld_mapped,                  /* name is malloc'd, filedata mmap'd */
    ld_archive                  /* Both point into mmap'd archive regions.  */
  } alloc;

  /* This provides a slot for category-specific code to cache data computed
   * about this locale.  That code can set a cleanup function to deallocate
   * the data.
  struct
  {
    void (*cleanup) (struct locale_data *);
    union
    {
      void *data;
      struct lc_time_data *time;
      const struct gconv_fcts *ctype;
    };
  } private;
   */

  unsigned int usage_count;     /* Counter for users.  */

  int use_translit;             /* Nonzero if the mb*towv*() and wc*tomb()
                                   functions should use transliteration.  */

  unsigned int nstrings;        /* Number of strings below.  */
  union locale_data_value
  {
    const uint32_t *wstr;
    const char *string;
    unsigned int word;          /* Note endian issues vs 64-bit pointers.  */
  }
  values[1];     /* Items, usually pointers into `filedata'.  */
};

#else /* GLIBC 2.2.x */

struct locale_data
{
  const char *name;
  const char *filedata;         /* Region mapping the file data.  */
  off_t filesize;               /* Size of the file (and the region).  */
  int mmaped;                   /* If nonzero the data is mmaped.  */

  unsigned int usage_count;     /* Counter for users.  */

  int use_translit;             /* Nonzero if the mb*towv*() and wc*tomb()
                                   functions should use transliteration.  */
  const char *options;          /* Extra options from the locale name,
                                   not used in the path to the locale data.  */

  unsigned int nstrings;        /* Number of strings below.  */
  union locale_data_value
  {
    const uint32_t *wstr;
    const char *string;
    unsigned int word;
  }
  values[1];     /* Items, usually pointers into `filedata'.  */
};

#endif


typedef __locale_t __c_locale;

#if (__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ > 2))
# define __nl_langinfo_l nl_langinfo_l
#endif

#if (__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ > 2))
#  define __LOCALE_CREATE(nm,category) (void*)newlocale(1 << category, nm, NULL )
#  define __LOCALE_DESTROY(__loc)      freelocale((__c_locale)__loc)
#else
#  define __LOCALE_CREATE(nm,category) (void*)__newlocale(1 << category, nm, NULL )
#  define __LOCALE_DESTROY(__loc)      __freelocale((__c_locale)__loc)
#endif

static const char *_empty_str = "";
static const char *_C_name = "C";


#if 0
struct _Locale_ctype
{
  __c_locale __cloc;
};

struct _Locale_numeric
{
  __c_locale __cloc;
};

struct _Locale_time
{
  __c_locale __cloc;
};

struct _Locale_collate
{
  __c_locale __cloc;
};

struct _Locale_monetary
{
  __c_locale __cloc;
};

struct _Locale_messages
{
  __c_locale __cloc;
};
#endif

void _Locale_init()
{}

void _Locale_final()
{}

void *_Locale_ctype_create( const char *nm, struct _Locale_name_hint* hint )
{ return __LOCALE_CREATE( nm, LC_CTYPE ); }

void* _Locale_numeric_create( const char *nm, struct _Locale_name_hint* hint )
{ return __LOCALE_CREATE( nm, LC_NUMERIC ); }

void* _Locale_time_create( const char *nm, struct _Locale_name_hint* hint )
{ return __LOCALE_CREATE( nm, LC_TIME ); }

void *_Locale_collate_create( const char *nm, struct _Locale_name_hint* hint )
{ return __LOCALE_CREATE( nm, LC_COLLATE ); }

void *_Locale_monetary_create( const char *nm, struct _Locale_name_hint* hint )
{ return __LOCALE_CREATE( nm, LC_MONETARY ); }

void *_Locale_messages_create( const char *nm, struct _Locale_name_hint* hint )
{ return __LOCALE_CREATE( nm, LC_MESSAGES ); }

/*
  try to see locale category LC should be used from environment;
  according POSIX, the order is
  1. LC_ALL
  2. category (LC_CTYPE, LC_NUMERIC, ... )
  3. LANG
  If set nothing, return "C" (this really implemetation-specific).
*/
const char *_Locale_aux_default( const char *LC, char *nm )
{
  char *name = getenv( "LC_ALL" );

  if ( name != NULL && *name != 0 ) {
    return strncpy( nm, name, _Locale_MAX_SIMPLE_NAME );
  }
  name = getenv( LC );
  if ( name != NULL && *name != 0 ) {
    return strncpy( nm, name, _Locale_MAX_SIMPLE_NAME );
  }
  name = getenv( "LANG" );
  if ( name != NULL && *name != 0 ) {
    return strncpy( nm, name, _Locale_MAX_SIMPLE_NAME );
  }

  return strcpy( nm, "C" );
}

const char *_Locale_ctype_default( char *nm )
{
  return _Locale_aux_default( "LC_CTYPE", nm );
}

const char *_Locale_numeric_default( char *nm )
{
  return _Locale_aux_default( "LC_NUMERIC", nm );
}

const char *_Locale_time_default( char *nm )
{
  return _Locale_aux_default( "LC_TIME", nm );
}

const char *_Locale_collate_default( char *nm )
{
  return _Locale_aux_default( "LC_COLLATE", nm );
}

const char *_Locale_monetary_default( char *nm )
{
  return _Locale_aux_default( "LC_MONETARY", nm );
}

const char *_Locale_messages_default( char *nm )
{
  return _Locale_aux_default( "LC_MESSAGES", nm );
}

char const*_Locale_ctype_name( const void *__loc, char *buf )
{
  return __loc != 0 ? strncpy( buf, ((__c_locale)__loc)->__locales[LC_CTYPE]->name, _Locale_MAX_SIMPLE_NAME ) : 0;
}

char const*_Locale_numeric_name( const void *__loc, char *buf )
{
  return __loc != 0 ? strncpy( buf, ((__c_locale)__loc)->__locales[LC_NUMERIC]->name, _Locale_MAX_SIMPLE_NAME ) : 0;
}

char const*_Locale_time_name( const void *__loc, char *buf )
{
  return __loc != 0 ? strncpy( buf, ((__c_locale)__loc)->__locales[LC_TIME]->name, _Locale_MAX_SIMPLE_NAME ) : 0;
}

char const*_Locale_collate_name( const void *__loc, char *buf )
{
  return __loc != 0 ? strncpy( buf, ((__c_locale)__loc)->__locales[LC_COLLATE]->name, _Locale_MAX_SIMPLE_NAME ) : 0;
}

char const*_Locale_monetary_name( const void *__loc, char *buf )
{
  return __loc != 0 ? strncpy( buf, ((__c_locale)__loc)->__locales[LC_MONETARY]->name, _Locale_MAX_SIMPLE_NAME ) : 0;
}

char const*_Locale_messages_name( const void *__loc, char *buf )
{
  return __loc != 0 ? strncpy( buf, ((__c_locale)__loc)->__locales[LC_MESSAGES]->name, _Locale_MAX_SIMPLE_NAME ) : 0;
}

void _Locale_ctype_destroy( void *__loc )
{ __LOCALE_DESTROY(__loc); }

void _Locale_numeric_destroy( void *__loc )
{ __LOCALE_DESTROY(__loc); }

void _Locale_time_destroy( void *__loc )
{ __LOCALE_DESTROY(__loc); }

void _Locale_collate_destroy( void *__loc )
{ __LOCALE_DESTROY(__loc); }

void _Locale_monetary_destroy( void *__loc )
{ __LOCALE_DESTROY(__loc); }

void _Locale_messages_destroy( void* __loc )
{ __LOCALE_DESTROY(__loc); }

/*
 * locale loc expected either locale name indeed (platform-specific)
 * or string like "LC_CTYPE=LocaleNameForCType;LC_NUMERIC=LocaleNameForNum;"
 *
 */

char const*__Extract_locale_name( const char *loc, const char *category, char *buf )
{
  char *expr;
  size_t len_name;
  buf[0] = 0;

  if( loc[0]=='L' && loc[1]=='C' && loc[2]=='_') {
    expr = strstr( (char*)loc, category );
    if ( expr == NULL )
      return NULL; /* Category not found. */
    ++expr;
    len_name = strcspn( expr, ";" );
    len_name = len_name > _Locale_MAX_SIMPLE_NAME ? _Locale_MAX_SIMPLE_NAME : len_name;
    strncpy( buf, expr, len_name );
    buf[len_name] = 0;
    return buf;
  }
  return strncpy( buf, loc, _Locale_MAX_SIMPLE_NAME );
}

char const*_Locale_extract_ctype_name( const char *loc, char *buf, struct _Locale_name_hint* hint )
{ return __Extract_locale_name( loc, "LC_CTYPE=", buf ); }

char const*_Locale_extract_numeric_name( const char *loc, char *buf, struct _Locale_name_hint* hint )
{ return __Extract_locale_name( loc, "LC_NUMERIC=", buf ); }

char const*_Locale_extract_time_name( const char *loc, char *buf, struct _Locale_name_hint* hint )
{ return __Extract_locale_name( loc, "LC_TIME=", buf ); }

char const*_Locale_extract_collate_name( const char *loc, char *buf, struct _Locale_name_hint* hint )
{ return __Extract_locale_name( loc, "LC_COLLATE=", buf ); }

char const*_Locale_extract_monetary_name( const char *loc, char *buf, struct _Locale_name_hint* hint )
{ return __Extract_locale_name( loc, "LC_MONETARY=", buf ); }

char const*_Locale_extract_messages_name( const char *loc, char *buf, struct _Locale_name_hint* hint )
{ return __Extract_locale_name( loc, "LC_MESSAGES=", buf ); }

char const*_Locale_compose_name(char*__DUMMY_PAR1, const char*__DUMMY_PAR2, const char*__DUMMY_PAR3,
                            const char*__DUMMY_PAR4,
                            const char*__DUMMY_PAR5, const char*__DUMMY_PAR6, const char*__DUMMY_PAR7, const char*__DUMMY_PAR8) {
  /* TODO: what's this? Is this a marker that this is not yet completely
  implemented? Copy the implementation from c_locale_win32 perhaps? It seems
  to complement the extract functions above. */
  printf( "%s:%d\n", __FILE__, __LINE__ );
  return 0;
}

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

/* ctype */

const _Locale_mask_t *_Locale_ctype_table( struct _Locale_ctype *__loc )
{
  /* return table with masks (upper, lower, alpha, etc.) */
  /* return ((__c_locale)__loc)->__locales[LC_CTYPE]->values[_NL_ITEM_INDEX (_NL_CTYPE_CLASS)].string + 128; */
  return ((__c_locale)__loc)->__ctype_b;
}

int _Locale_toupper( struct _Locale_ctype *__loc, int c )
{ return ((__c_locale)__loc)->__ctype_toupper[c]; }

int _Locale_tolower( struct _Locale_ctype *__loc, int c )
{ return ((__c_locale)__loc)->__ctype_tolower[c]; }

#if !defined (_STLP_NO_WCHAR_T)
_Locale_mask_t _Locale_wchar_ctype(struct _Locale_ctype*__DUMMY_PAR1, wint_t __DUMMY_PAR2, _Locale_mask_t __DUMMY_PAR3) {
  printf( "%s:%d\n", __FILE__, __LINE__ );
  return 0;
}

wint_t _Locale_wchar_tolower( struct _Locale_ctype *__loc, wint_t c ) {
  printf( "%s:%d\n", __FILE__, __LINE__ );
  return __towlower_l( c, ((__c_locale)__loc) );
}

wint_t _Locale_wchar_toupper( struct _Locale_ctype *__loc, wint_t c ) {
  printf( "%s:%d\n", __FILE__, __LINE__ );
  return __towupper_l( c, ((__c_locale)__loc) );
}
#endif

#if !defined (_STLP_NO_MBSTATE_T)

int _Locale_mb_cur_max( struct _Locale_ctype * __DUMMY_PAR) {
  printf( "%s:%d\n", __FILE__, __LINE__ );
  return 0;
}

int _Locale_mb_cur_min( struct _Locale_ctype * __DUMMY_PAR) {
  printf( "%s:%d\n", __FILE__, __LINE__ );
  return 0;
}

int _Locale_is_stateless( struct _Locale_ctype * __DUMMY_PAR) {
  printf( "%s:%d\n", __FILE__, __LINE__ );
  return 1;
}

#if !defined (_STLP_NO_WCHAR_T)
wint_t _Locale_btowc(struct _Locale_ctype *__loc, int c) {
  wint_t _c;
  /* __c_locale __tmp = __uselocale( __loc ); */
  _c = btowc( c );
  /* __uselocale( __tmp ); */
  return _c;
}

int _Locale_wctob(struct _Locale_ctype *__loc, wint_t c) {
  int _c;
  /* __c_locale __tmp = __uselocale( __loc ); */
  _c = wctob( c );
  /* __uselocale( __tmp ); */
  return _c;
}

size_t _Locale_mbtowc(struct _Locale_ctype *__DUMMY_PAR1,
                      wchar_t *__DUMMY_PAR2,
                      const char *__DUMMY_PAR3, size_t __DUMMY_PAR4,
                      mbstate_t *__DUMMY_PAR5) {
  printf( "%s:%d\n", __FILE__, __LINE__ );
  return (size_t) -1;
}

size_t _Locale_wctomb(struct _Locale_ctype *__DUMMY_PAR1,
                      char *__DUMMY_PAR2, size_t __DUMMY_PAR3,
                      const wchar_t __DUMMY_PAR4,
                      mbstate_t *__DUMMY_PAR5) {
  printf( "%s:%d\n", __FILE__, __LINE__ );
  return (size_t) -1;
}
#endif

size_t _Locale_unshift(struct _Locale_ctype *__DUMMY_PAR1,
                       mbstate_t *__DUMMY_PAR2,
                       char *__DUMMY_PAR3, size_t __DUMMY_PAR4, char ** __DUMMY_PAR5) {
  printf( "%s:%d\n", __FILE__, __LINE__ );
  return (size_t) -1;
}

#endif /*  _STLP_NO_MBSTATE_T */

/* Collate */
int _Locale_strcmp(struct _Locale_collate * __loc,
                   const char *s1, size_t n1,
		   const char *s2, size_t n2) {
  int ret;
  char buf1[64], buf2[64];
  while (n1 > 0 && n2 > 0) {
    size_t bufsize1 = n1 < 63 ? n1 : 63;
    size_t bufsize2 = n2 < 63 ? n2 : 63;
    strncpy(buf1, s1, bufsize1); buf1[bufsize1] = 0;
    strncpy(buf2, s2, bufsize2); buf2[bufsize2] = 0;

    ret = strcoll_l(buf1, buf2, (__c_locale)__loc);
    if (ret != 0) return ret;
    s1 += bufsize1; n1 -= bufsize1;
    s2 += bufsize2; n2 -= bufsize2;
  }
  return ret;
}

#if !defined (_STLP_NO_WCHAR_T)
int _Locale_strwcmp(struct _Locale_collate *__loc,
                    const wchar_t *s1, size_t n1,
                    const wchar_t *s2, size_t n2) {
  int ret;
  wchar_t buf1[64], buf2[64];
  while (n1 > 0 && n2 > 0) {
    size_t bufsize1 = n1 < 63 ? n1 : 63;
    size_t bufsize2 = n2 < 63 ? n2 : 63;
    wcsncpy(buf1, s1, bufsize1); buf1[bufsize1] = 0;
    wcsncpy(buf2, s2, bufsize2); buf2[bufsize2] = 0;

    ret = wcscoll_l(buf1, buf2, (__c_locale)__loc);
    if (ret != 0) return ret;
    s1 += bufsize1; n1 -= bufsize1;
    s2 += bufsize2; n2 -= bufsize2;
  }
  return ret;
}

#endif

size_t _Locale_strxfrm(struct _Locale_collate *__loc,
                       char *dest, size_t dest_n,
                       const char *src, size_t src_n )
{
  const char *real_src;
  char *buf = NULL;
  size_t result;

  if (src_n == 0)
  {
    if (dest != NULL) dest[0] = 0;
    return 0;
  }
  if (src[src_n] != 0) {
    buf = malloc(src_n + 1);
    strncpy(buf, src, src_n);
    buf[src_n] = 0;
    real_src = buf;
  }
  else
    real_src = src;
  result = __strxfrm_l(dest, real_src, dest_n, (__c_locale)__loc);
  if (buf != NULL) free(buf);
  return result;
}

# ifndef _STLP_NO_WCHAR_T

size_t _Locale_strwxfrm( struct _Locale_collate *__loc,
                          wchar_t *dest, size_t dest_n,
                          const wchar_t *src, size_t src_n )
{
  const wchar_t *real_src;
  wchar_t *buf = NULL;
  size_t result;

  if (src_n == 0)
  {
    if (dest != NULL) dest[0] = 0;
    return 0;
  }
  if (src[src_n] != 0) {
    buf = malloc((src_n + 1) * sizeof(wchar_t));
    wcsncpy(buf, src, src_n);
    buf[src_n] = 0;
    real_src = buf;
  }
  else
    real_src = src;
  result = __wcsxfrm_l(dest, real_src, dest_n, (__c_locale)__loc);
  if (buf != NULL) free(buf);
  return result;
}

# endif

/* Numeric */

char _Locale_decimal_point(struct _Locale_numeric *__loc)
{
  return (__loc != 0) ? *(__nl_langinfo_l(RADIXCHAR, (__c_locale)__loc)) : '.';
}

char _Locale_thousands_sep(struct _Locale_numeric *__loc)
{
  return (__loc != 0) ? *(__nl_langinfo_l(THOUSEP, (__c_locale)__loc)) : ',';
}

const char* _Locale_grouping(struct _Locale_numeric *__loc)
{
  return (__loc != 0 && _Locale_thousands_sep(__loc) != '\0' ) ?
    (__nl_langinfo_l(GROUPING, (__c_locale)__loc)) : "";
}

const char *_Locale_true(struct _Locale_numeric *__loc)
{
  return __loc != 0 ? __nl_langinfo_l(YESSTR, (__c_locale)__loc) : "true";
}

const char *_Locale_false(struct _Locale_numeric *__loc)
{
  return __loc != 0 ? __nl_langinfo_l(NOSTR, (__c_locale)__loc) : "false";
}


/* Monetary */

const char *_Locale_int_curr_symbol(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? __nl_langinfo_l(INT_CURR_SYMBOL, (__c_locale)__loc) : 0;
}

const char *_Locale_currency_symbol(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? __nl_langinfo_l(CURRENCY_SYMBOL, (__c_locale)__loc) : 0;
}

char _Locale_mon_decimal_point(struct _Locale_monetary * __loc)
{
  return __loc != 0 ? *(__nl_langinfo_l(MON_DECIMAL_POINT,(__c_locale)__loc)) : '.';
}

char _Locale_mon_thousands_sep(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? *(__nl_langinfo_l(MON_THOUSANDS_SEP, (__c_locale)__loc)) : ',';
}

const char *_Locale_mon_grouping(struct _Locale_monetary *__loc)
{
  return (__loc != 0 && _Locale_mon_thousands_sep( __loc ) != '\0' ) ?
    __nl_langinfo_l(MON_GROUPING, (__c_locale)__loc) : _empty_str;
}

const char *_Locale_positive_sign(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? __nl_langinfo_l(POSITIVE_SIGN, (__c_locale)__loc) : _empty_str;
}

const char *_Locale_negative_sign(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? __nl_langinfo_l(NEGATIVE_SIGN, (__c_locale)__loc) : _empty_str;
}

char _Locale_int_frac_digits(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? *(__nl_langinfo_l(INT_FRAC_DIGITS, (__c_locale)__loc)) : CHAR_MAX;
}

char _Locale_frac_digits(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? *(__nl_langinfo_l(FRAC_DIGITS, (__c_locale)__loc)) : CHAR_MAX;
}

/* 1 if currency_symbol precedes a positive value, 0 if succeeds */
int _Locale_p_cs_precedes(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? *(__nl_langinfo_l(P_CS_PRECEDES, (__c_locale)__loc)) : CHAR_MAX;
}

/* 1 if a space separates currency_symbol from a positive value. */
int _Locale_p_sep_by_space(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? *(__nl_langinfo_l(P_SEP_BY_SPACE, (__c_locale)__loc)) : CHAR_MAX;
}

/*
 * 0 Parentheses surround the quantity and currency_symbol
 * 1 The sign string precedes the quantity and currency_symbol
 * 2 The sign string succeeds the quantity and currency_symbol.
 * 3 The sign string immediately precedes the currency_symbol.
 * 4 The sign string immediately succeeds the currency_symbol.
 */
int _Locale_p_sign_posn(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? *(__nl_langinfo_l(P_SIGN_POSN, (__c_locale)__loc)) : CHAR_MAX;
}

/* 1 if currency_symbol precedes a negative value, 0 if succeeds */
int _Locale_n_cs_precedes(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? *(__nl_langinfo_l(N_CS_PRECEDES, (__c_locale)__loc)) : CHAR_MAX;
}

/* 1 if a space separates currency_symbol from a negative value. */
int _Locale_n_sep_by_space(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? *(__nl_langinfo_l(N_SEP_BY_SPACE, (__c_locale)__loc)) : CHAR_MAX;
}

/*
 * 0 Parentheses surround the quantity and currency_symbol
 * 1 The sign string precedes the quantity and currency_symbol
 * 2 The sign string succeeds the quantity and currency_symbol.
 * 3 The sign string immediately precedes the currency_symbol.
 * 4 The sign string immediately succeeds the currency_symbol.
 */
int _Locale_n_sign_posn(struct _Locale_monetary *__loc)
{
  return __loc != 0 ? *(__nl_langinfo_l(N_SIGN_POSN, (__c_locale)__loc)) : CHAR_MAX;
}


/* Time */
const char *_Locale_full_monthname(struct _Locale_time *__loc, int _m )
{
  return (__loc != 0 && _m >= 0 && _m < 12) ? __nl_langinfo_l(MON_1 + _m, (__c_locale)__loc) : 0;
}

const char *_Locale_abbrev_monthname(struct _Locale_time *__loc, int _m )
{
  return (__loc != 0 && _m >= 0 && _m < 12) ? __nl_langinfo_l(ABMON_1 + _m, (__c_locale)__loc) : 0;
}

const char *_Locale_full_dayofweek(struct _Locale_time *__loc, int _d )
{
  return (__loc != 0 && _d >= 0 && _d < 7) ? __nl_langinfo_l(DAY_1 + _d, (__c_locale)__loc) : 0;
}

const char *_Locale_abbrev_dayofweek(struct _Locale_time *__loc, int _d )
{
  return (__loc != 0 && _d >= 0 && _d < 7) ? __nl_langinfo_l(ABDAY_1 + _d, (__c_locale)__loc) : 0;
}

const char *_Locale_d_t_fmt(struct _Locale_time *__loc)
{
  return __loc != 0 ? __nl_langinfo_l(D_T_FMT, (__c_locale)__loc) : 0;
}

const char *_Locale_d_fmt(struct _Locale_time *__loc )
{
  return __loc != 0 ? __nl_langinfo_l(D_FMT, (__c_locale)__loc) : 0;
}

const char *_Locale_t_fmt(struct _Locale_time *__loc )
{
  return __loc != 0 ? __nl_langinfo_l(T_FMT, (__c_locale)__loc) : 0;
}

const char *_Locale_long_d_t_fmt(struct _Locale_time *__loc )
{
  return __loc != 0 ? __nl_langinfo_l(ERA_D_T_FMT, (__c_locale)__loc) : 0;
}

const char *_Locale_long_d_fmt(struct _Locale_time *__loc )
{
  return __loc != 0 ? __nl_langinfo_l(ERA_D_FMT, (__c_locale)__loc) : 0;
}

const char *_Locale_am_str(struct _Locale_time *__loc )
{
  return __loc != 0 ? __nl_langinfo_l(AM_STR, (__c_locale)__loc) : _empty_str;
}

const char *_Locale_pm_str(struct _Locale_time* __loc )
{
  return __loc != 0 ? __nl_langinfo_l(PM_STR, (__c_locale)__loc) : _empty_str;
}

const char *_Locale_t_fmt_ampm(struct _Locale_time *__loc )
{
  return __loc != 0 ? __nl_langinfo_l(T_FMT_AMPM, (__c_locale)__loc) : 0;
}

/* Messages */

nl_catd_type _Locale_catopen(struct _Locale_messages *__loc, const char *__cat_name )
{
  return catopen( __cat_name, NL_CAT_LOCALE );
}

void _Locale_catclose(struct _Locale_messages *__loc, nl_catd_type __cat )
{
  catclose( __cat );
}

const char *_Locale_catgets(struct _Locale_messages *__loc, nl_catd_type __cat,
                            int __setid, int __msgid, const char *dfault)
{
  return catgets( __cat, __setid, __msgid, dfault );
}
