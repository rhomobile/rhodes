/*
 * Copyright (c) 1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999
 * Boris Fomitchev
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

/* This is a "stub" implementation of the "c_locale.h" interface,
   intended for operating systems where we have not yet written
   a real implementation.  A C++ library using this stub implementation
   is still standard-conforming, since the C++ standard does not require
   that any locales other than "C" be supported.
*/

/* Framework functions */

void _Locale_init()
{}

void _Locale_final()
{}

void *_Locale_ctype_create(const char * __DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

void *_Locale_numeric_create(const char * __DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

void *_Locale_time_create(const char * __DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

void *_Locale_collate_create(const char *__DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

void *_Locale_monetary_create(const char * __DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

void *_Locale_messages_create(const char *__DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

const char *_Locale_ctype_default(char* __DUMMY_PAR)    { return 0; }
const char *_Locale_numeric_default(char * __DUMMY_PAR) { return 0; }
const char *_Locale_time_default(char* __DUMMY_PAR)     { return 0; }
const char *_Locale_collate_default(char* __DUMMY_PAR)  { return 0; }
const char *_Locale_monetary_default(char* __DUMMY_PAR) { return 0; }
const char *_Locale_messages_default(char* __DUMMY_PAR) { return 0; }

char const* _Locale_ctype_name(const void* __DUMMY_PAR1, char* __DUMMY_PAR)
{ return 0; }

char const* _Locale_numeric_name(const void* __DUMMY_PAR1, char* __DUMMY_PAR)
{ return 0; }

char const* _Locale_time_name(const void* __DUMMY_PAR1, char* __DUMMY_PAR)
{ return 0; }

char const* _Locale_collate_name(const void* __DUMMY_PAR1, char* __DUMMY_PAR)
{ return 0; }

char const* _Locale_monetary_name(const void* __DUMMY_PAR1, char* __DUMMY_PAR)
{ return 0; }

char const* _Locale_messages_name(const void* __DUMMY_PAR1, char* __DUMMY_PAR)
{ return 0; }

void _Locale_ctype_destroy(void* __DUMMY_PAR)    {}
void _Locale_numeric_destroy(void* __DUMMY_PAR)  {}
void _Locale_time_destroy(void* __DUMMY_PAR)     {}
void _Locale_collate_destroy(void* __DUMMY_PAR)  {}
void _Locale_monetary_destroy(void* __DUMMY_PAR) {}
void _Locale_messages_destroy(void* __DUMMY_PAR) {}


char const* _Locale_extract_ctype_name(const char* __DUMMY_PAR1, char* __DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

char const* _Locale_extract_numeric_name(const char*__DUMMY_PAR1, char* __DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

char const* _Locale_extract_time_name(const char*__DUMMY_PAR1, char* __DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

char const* _Locale_extract_collate_name(const char*__DUMMY_PAR1, char* __DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

char const* _Locale_extract_monetary_name(const char*__DUMMY_PAR1, char* __DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

char const* _Locale_extract_messages_name(const char*__DUMMY_PAR1, char* __DUMMY_PAR, struct _Locale_name_hint* hint)
{ return 0; }

char const* _Locale_compose_name(char*__DUMMY_PAR1, const char*__DUMMY_PAR2, const char*__DUMMY_PAR3,
                           const char*__DUMMY_PAR4,
                           const char*__DUMMY_PAR5, const char*__DUMMY_PAR6, const char*__DUMMY_PAR7, const char*__DUMMY_PAR8)
{ return 0; }


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

const  _Locale_mask_t* _Locale_ctype_table(struct _Locale_ctype* __DUMMY_PAR)
{ return 0; }

int _Locale_toupper(struct _Locale_ctype*__DUMMY_PAR1, int __DUMMY_PAR) { return 0; }

int _Locale_tolower(struct _Locale_ctype*__DUMMY_PAR1, int __DUMMY_PAR) { return 0; }

# ifndef _STLP_NO_WCHAR_T
_Locale_mask_t _Locale_wchar_ctype(struct _Locale_ctype*__DUMMY_PAR1, wint_t __DUMMY_PAR2, _Locale_mask_t __DUMMY_PAR3)
{ return 0; }

wint_t _Locale_wchar_tolower(struct _Locale_ctype*__DUMMY_PAR1, wint_t __DUMMY_PAR)
{ return 0; }

wint_t _Locale_wchar_toupper(struct _Locale_ctype*__DUMMY_PAR1, wint_t __DUMMY_PAR)
{ return 0; }
# endif

# ifndef _STLP_NO_MBSTATE_T

int _Locale_mb_cur_max (struct _Locale_ctype * __DUMMY_PAR) { return 0; }
int _Locale_mb_cur_min (struct _Locale_ctype * __DUMMY_PAR) { return 0; }
int _Locale_is_stateless (struct _Locale_ctype * __DUMMY_PAR) { return 1; }
#ifndef _STLP_NO_WCHAR_T
wint_t _Locale_btowc(struct _Locale_ctype * __DUMMY_PAR1, int __DUMMY_PAR) { return 0; }

int _Locale_wctob(struct _Locale_ctype * __DUMMY_PAR2, wint_t __DUMMY_PAR) { return 0; }

size_t _Locale_mbtowc(struct _Locale_ctype *__DUMMY_PAR1,
                      wchar_t *__DUMMY_PAR2,
                      const char *__DUMMY_PAR3, size_t __DUMMY_PAR4,
                      mbstate_t *__DUMMY_PAR5)
{ return (size_t) -1; }

size_t _Locale_wctomb(struct _Locale_ctype *__DUMMY_PAR1,
                      char *__DUMMY_PAR2, size_t __DUMMY_PAR3,
                      const wchar_t __DUMMY_PAR4,
                      mbstate_t *__DUMMY_PAR5)
{ return (size_t) -1; }
# endif

size_t _Locale_unshift(struct _Locale_ctype *__DUMMY_PAR1,
                       mbstate_t *__DUMMY_PAR2,
                       char *__DUMMY_PAR3, size_t __DUMMY_PAR4, char ** __DUMMY_PAR5)
{ return (size_t) -1; }

# endif /*  _STLP_NO_MBSTATE_T */


/* Collate */
 int _Locale_strcmp(struct _Locale_collate* __DUMMY_PAR1,
                    const char* __DUMMY_PAR2, size_t __DUMMY_PAR3, const char* __DUMMY_PAR4,
                    size_t __DUMMY_PAR5)
{ return 0; }

# ifndef _STLP_NO_WCHAR_T

int _Locale_strwcmp(struct _Locale_collate* __DUMMY_PAR1,
                    const wchar_t* __DUMMY_PAR2, size_t __DUMMY_PAR3,
                    const wchar_t* __DUMMY_PAR4, size_t __DUMMY_PAR5)
{ return 0; }

# endif

size_t _Locale_strxfrm(struct _Locale_collate* __DUMMY_PAR1,
                       char* __DUMMY_PAR2, size_t __DUMMY_PAR3,
                       const char* __DUMMY_PAR4, size_t __DUMMY_PAR5)
{ return 0; }

# ifndef _STLP_NO_WCHAR_T

size_t _Locale_strwxfrm(struct _Locale_collate* __DUMMY_PAR1,
                        wchar_t* __DUMMY_PAR2, size_t __DUMMY_PAR3,
                        const wchar_t* __DUMMY_PAR4, size_t __DUMMY_PAR5)
{ return 0; }

# endif

/* Numeric */

char _Locale_decimal_point(struct _Locale_numeric* __DUMMY_PAR) { return '.'; }
char _Locale_thousands_sep(struct _Locale_numeric* __DUMMY_PAR) { return ','; }
const char* _Locale_grouping(struct _Locale_numeric * __DUMMY_PAR)
{ return ""; }

const char * _Locale_true(struct _Locale_numeric * __DUMMY_PAR)  { return 0; }
const char * _Locale_false(struct _Locale_numeric * __DUMMY_PAR) { return 0; }


/* Monetary */

const char* _Locale_int_curr_symbol(struct _Locale_monetary * __DUMMY_PAR)
{ return 0; }
const char* _Locale_currency_symbol(struct _Locale_monetary * __DUMMY_PAR)
{ return 0; }
char        _Locale_mon_decimal_point(struct _Locale_monetary * __DUMMY_PAR)
{ return '.'; }
char        _Locale_mon_thousands_sep(struct _Locale_monetary * __DUMMY_PAR)
{ return ','; }
const char* _Locale_mon_grouping(struct _Locale_monetary * __DUMMY_PAR)
{ return ""; }
const char* _Locale_positive_sign(struct _Locale_monetary * __DUMMY_PAR)
{ return ""; }
const char* _Locale_negative_sign(struct _Locale_monetary * __DUMMY_PAR)
{ return ""; }
char        _Locale_int_frac_digits(struct _Locale_monetary * __DUMMY_PAR)
{ return CHAR_MAX; }
char        _Locale_frac_digits(struct _Locale_monetary * __DUMMY_PAR)
{ return CHAR_MAX; }
int         _Locale_p_cs_precedes(struct _Locale_monetary * __DUMMY_PAR)
{ return CHAR_MAX; }
int         _Locale_p_sep_by_space(struct _Locale_monetary * __DUMMY_PAR)
{ return CHAR_MAX; }
int         _Locale_p_sign_posn(struct _Locale_monetary * __DUMMY_PAR)
{ return CHAR_MAX; }
int         _Locale_n_cs_precedes(struct _Locale_monetary * __DUMMY_PAR)
{ return CHAR_MAX; }
int          _Locale_n_sep_by_space(struct _Locale_monetary * __DUMMY_PAR)
{ return CHAR_MAX; }
int          _Locale_n_sign_posn(struct _Locale_monetary * __DUMMY_PAR)
{ return CHAR_MAX; }


/* Time */
const char * _Locale_full_monthname(struct _Locale_time * __DUMMY_PAR1, int __DUMMY_PAR2)
{ return 0; }
const char * _Locale_abbrev_monthname(struct _Locale_time * __DUMMY_PAR1, int __DUMMY_PAR2)
{ return 0; }
const char * _Locale_full_dayofweek(struct _Locale_time * __DUMMY_PAR1, int __DUMMY_PAR2)
{ return 0; }
const char * _Locale_abbrev_dayofweek(struct _Locale_time * __DUMMY_PAR1, int __DUMMY_PAR2)
{ return 0; }

const char* _Locale_d_t_fmt(struct _Locale_time* __DUMMY_PAR)    { return 0; }
const char* _Locale_d_fmt(struct _Locale_time* __DUMMY_PAR)      { return 0; }
const char* _Locale_t_fmt(struct _Locale_time* __DUMMY_PAR)      { return 0; }
const char* _Locale_long_d_t_fmt(struct _Locale_time* __DUMMY_PAR){ return 0; }
const char* _Locale_long_d_fmt(struct _Locale_time* __DUMMY_PAR)   { return 0; }
const char* _Locale_am_str(struct _Locale_time* __DUMMY_PAR)     { return 0; }
const char* _Locale_pm_str(struct _Locale_time* __DUMMY_PAR)     { return 0; }
const char* _Locale_t_fmt_ampm(struct _Locale_time* __DUMMY_PAR) { return 0; }

/* Messages */

nl_catd_type _Locale_catopen(struct _Locale_messages* __DUMMY_PAR1, const char* __DUMMY_PAR)
{ return -1; }
void _Locale_catclose(struct _Locale_messages* __DUMMY_PAR1, nl_catd_type __DUMMY_PAR) {}
const char* _Locale_catgets(struct _Locale_messages* __DUMMY_PAR1, nl_catd_type __DUMMY_PAR2,
                            int __DUMMY_PAR3, int __DUMMY_PAR4,
                            const char *dfault)
{ return dfault; }
