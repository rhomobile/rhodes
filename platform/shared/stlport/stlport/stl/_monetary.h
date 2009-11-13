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
// WARNING: This is an internal header file, included by other C++
// standard library headers.  You should not attempt to use this header
// file directly.


#ifndef _STLP_INTERNAL_MONETARY_H
#define _STLP_INTERNAL_MONETARY_H

#ifndef _STLP_INTERNAL_CTYPE_H
#  include <stl/_ctype.h>
#endif

#ifndef _STLP_INTERNAL_OSTREAMBUF_ITERATOR_H
#  include <stl/_ostreambuf_iterator.h>
#endif

#ifndef _STLP_INTERNAL_ISTREAMBUF_ITERATOR_H
#  include <stl/_istreambuf_iterator.h>
#endif

_STLP_BEGIN_NAMESPACE

class money_base {
public:
  enum part {none, space, symbol, sign, value};
  struct pattern {
    char field[4];
  };
};

// moneypunct facets: forward declaration
template <class _charT, _STLP_DFL_NON_TYPE_PARAM(bool, _International, false) > class moneypunct {};

// money_get facets

#if defined (_STLP_LIMITED_DEFAULT_TEMPLATES)
template <class _CharT, class _InputIter>
#else
template <class _CharT, class _InputIter = istreambuf_iterator<_CharT, char_traits<_CharT> > >
#endif
class money_get : public locale::facet {
  friend class _Locale_impl;

public:
  typedef _CharT               char_type;
  typedef _InputIter           iter_type;
  typedef basic_string<_CharT, char_traits<_CharT>, allocator<_CharT> > string_type;

  money_get(size_t __refs = 0) : locale::facet(__refs) {}
  iter_type get(iter_type __s, iter_type  __end, bool __intl,
                ios_base&  __str, ios_base::iostate&  __err,
                _STLP_LONGEST_FLOAT_TYPE& __units) const
    { return do_get(__s,  __end, __intl,  __str,  __err, __units); }
  iter_type get(iter_type __s, iter_type  __end, bool __intl,
                ios_base&  __str, ios_base::iostate& __err,
                string_type& __digits) const
    { return do_get(__s,  __end, __intl,  __str,  __err, __digits); }

  static _STLP_STATIC_MEMBER_DECLSPEC locale::id id;

protected:
  ~money_get() {}
  virtual iter_type do_get(iter_type __s, iter_type  __end, bool  __intl,
                           ios_base&  __str, ios_base::iostate& __err,
                           _STLP_LONGEST_FLOAT_TYPE& __units) const;
  virtual iter_type do_get(iter_type __s, iter_type __end, bool __intl,
                           ios_base&  __str, ios_base::iostate& __err,
                           string_type& __digits) const;
};


// moneypunct facets: definition of specializations

_STLP_TEMPLATE_NULL
class _STLP_CLASS_DECLSPEC moneypunct<char, true> : public locale::facet, public money_base {

public:
  typedef char                 char_type;
  typedef string               string_type;
  explicit moneypunct _STLP_PSPEC2(char, true) (size_t __refs = 0);

  char        decimal_point() const { return do_decimal_point(); }
  char        thousands_sep() const { return do_thousands_sep(); }
  string      grouping()      const { return do_grouping(); }
  string_type curr_symbol()   const { return do_curr_symbol(); }
  string_type positive_sign() const { return do_positive_sign(); }
  string_type negative_sign() const { return do_negative_sign(); }
  int         frac_digits()   const { return do_frac_digits(); }
  pattern     pos_format()    const { return do_pos_format(); }
  pattern     neg_format()    const { return do_neg_format(); }

  static _STLP_STATIC_MEMBER_DECLSPEC locale::id id;
# if defined (_STLP_STATIC_CONST_INIT_BUG)
  enum _IntlVal { intl = 1 } ;
# else
  static const bool intl = true;
# endif

protected:
  pattern _M_pos_format;
  pattern _M_neg_format;

  ~moneypunct _STLP_PSPEC2(char, true) ();

  virtual char        do_decimal_point() const;
  virtual char        do_thousands_sep() const;
  virtual string      do_grouping()      const;

  virtual string      do_curr_symbol()   const;

  virtual string      do_positive_sign() const;
  virtual string      do_negative_sign() const;
  virtual int         do_frac_digits()   const;
  virtual pattern     do_pos_format()    const;
  virtual pattern     do_neg_format()    const;

  friend class _Locale_impl;
};

_STLP_TEMPLATE_NULL
class _STLP_CLASS_DECLSPEC moneypunct<char, false> : public locale::facet, public money_base
{
public:
  typedef char                 char_type;
  typedef string               string_type;

  explicit moneypunct _STLP_PSPEC2(char, false) (size_t __refs = 0);

  char        decimal_point() const { return do_decimal_point(); }
  char        thousands_sep() const { return do_thousands_sep(); }
  string      grouping()      const { return do_grouping(); }
  string_type curr_symbol()   const { return do_curr_symbol(); }
  string_type positive_sign() const { return do_positive_sign(); }
  string_type negative_sign() const { return do_negative_sign(); }
  int         frac_digits()   const { return do_frac_digits(); }
  pattern     pos_format()    const { return do_pos_format(); }
  pattern     neg_format()    const { return do_neg_format(); }

  static _STLP_STATIC_MEMBER_DECLSPEC locale::id id;
# if defined (_STLP_STATIC_CONST_INIT_BUG)
  enum _IntlVal { intl = 0 } ;
# else
  static const bool intl = false;
# endif

protected:
  pattern _M_pos_format;
  pattern _M_neg_format;

  ~moneypunct _STLP_PSPEC2(char, false) ();

  virtual char        do_decimal_point() const;
  virtual char        do_thousands_sep() const;
  virtual string      do_grouping()      const;

  virtual string      do_curr_symbol()   const;

  virtual string      do_positive_sign() const;
  virtual string      do_negative_sign() const;
  virtual int         do_frac_digits()   const;
  virtual pattern     do_pos_format()    const;
  virtual pattern     do_neg_format()    const;

  friend class _Locale_impl;
};


# ifndef _STLP_NO_WCHAR_T

_STLP_TEMPLATE_NULL
class _STLP_CLASS_DECLSPEC moneypunct<wchar_t, true> : public locale::facet, public money_base
{
  friend class _Locale_impl;
public:
  typedef wchar_t                 char_type;
  typedef wstring                 string_type;
  explicit moneypunct _STLP_PSPEC2(wchar_t, true) (size_t __refs = 0);
  wchar_t     decimal_point() const { return do_decimal_point(); }
  wchar_t     thousands_sep() const { return do_thousands_sep(); }
  string      grouping()      const { return do_grouping(); }
  string_type curr_symbol()   const { return do_curr_symbol(); }
  string_type positive_sign() const { return do_positive_sign(); }
  string_type negative_sign() const { return do_negative_sign(); }
  int         frac_digits()   const { return do_frac_digits(); }
  pattern     pos_format()    const { return do_pos_format(); }
  pattern     neg_format()    const { return do_neg_format(); }

  static _STLP_STATIC_MEMBER_DECLSPEC locale::id id;
# if defined (_STLP_STATIC_CONST_INIT_BUG)
  enum _IntlVal { intl = 1 } ;
# else
  static const bool intl = true;
# endif

protected:
  pattern _M_pos_format;
  pattern _M_neg_format;

  ~moneypunct _STLP_PSPEC2(wchar_t, true) ();

  virtual wchar_t     do_decimal_point() const;
  virtual wchar_t     do_thousands_sep() const;
  virtual string      do_grouping()      const;

  virtual string_type do_curr_symbol()   const;

  virtual string_type do_positive_sign() const;
  virtual string_type do_negative_sign() const;
  virtual int         do_frac_digits()   const;
  virtual pattern     do_pos_format()    const;
  virtual pattern     do_neg_format()    const;
};


_STLP_TEMPLATE_NULL
class _STLP_CLASS_DECLSPEC moneypunct<wchar_t, false> : public locale::facet, public money_base
{
  friend class _Locale_impl;
public:
  typedef wchar_t                 char_type;
  typedef wstring                 string_type;
  explicit moneypunct _STLP_PSPEC2(wchar_t, false) (size_t __refs = 0);
  wchar_t     decimal_point() const { return do_decimal_point(); }
  wchar_t     thousands_sep() const { return do_thousands_sep(); }
  string      grouping()      const { return do_grouping(); }
  string_type curr_symbol()   const { return do_curr_symbol(); }
  string_type positive_sign() const { return do_positive_sign(); }
  string_type negative_sign() const { return do_negative_sign(); }
  int         frac_digits()   const { return do_frac_digits(); }
  pattern     pos_format()    const { return do_pos_format(); }
  pattern     neg_format()    const { return do_neg_format(); }

  static _STLP_STATIC_MEMBER_DECLSPEC locale::id id;
# if defined (_STLP_STATIC_CONST_INIT_BUG)
  enum _IntlVal { intl = 0 } ;
# else
  static const bool intl = false;
# endif

protected:
  pattern _M_pos_format;
  pattern _M_neg_format;

  ~moneypunct _STLP_PSPEC2(wchar_t, false) ();

  virtual wchar_t     do_decimal_point() const;
  virtual wchar_t     do_thousands_sep() const;
  virtual string      do_grouping()      const;

  virtual string_type do_curr_symbol()   const;

  virtual string_type do_positive_sign() const;
  virtual string_type do_negative_sign() const;
  virtual int         do_frac_digits()   const;
  virtual pattern     do_pos_format()    const;
  virtual pattern     do_neg_format()    const;
};

# endif

template <class _charT, _STLP_DFL_NON_TYPE_PARAM(bool , _International , false) > class moneypunct_byname {};

_STLP_TEMPLATE_NULL
class _STLP_CLASS_DECLSPEC moneypunct_byname<char, true> : public moneypunct<char, true> {
public:
  typedef money_base::pattern   pattern;
  typedef char                  char_type;
  typedef string                string_type;

  explicit moneypunct_byname _STLP_PSPEC2(char, true) (const char * __name, size_t __refs = 0,
                                                       _Locale_name_hint* __hint = 0);

protected:
  _Locale_monetary* _M_monetary;
  ~moneypunct_byname _STLP_PSPEC2(char, true) ();
  virtual char        do_decimal_point() const;
  virtual char        do_thousands_sep() const;
  virtual string      do_grouping()      const;

  virtual string_type do_curr_symbol()   const;

  virtual string_type do_positive_sign() const;
  virtual string_type do_negative_sign() const;
  virtual int         do_frac_digits()   const;

private:
  typedef moneypunct_byname<char, true> _Self;
  //explicitely defined as private to avoid warnings:
  moneypunct_byname(_Self const&);
  _Self& operator = (_Self const&);
};

_STLP_TEMPLATE_NULL
class _STLP_CLASS_DECLSPEC moneypunct_byname<char, false> : public moneypunct<char, false>
{
public:
  typedef money_base::pattern   pattern;
  typedef char                  char_type;
  typedef string                string_type;

  explicit moneypunct_byname _STLP_PSPEC2(char, false) (const char * __name, size_t __refs = 0,
                                                        _Locale_name_hint* __hint = 0);

protected:
  _Locale_monetary* _M_monetary;
  ~moneypunct_byname _STLP_PSPEC2(char, false) ();
  virtual char        do_decimal_point() const;
  virtual char        do_thousands_sep() const;
  virtual string      do_grouping()      const;

  virtual string_type do_curr_symbol()   const;

  virtual string_type do_positive_sign() const;
  virtual string_type do_negative_sign() const;
  virtual int         do_frac_digits()   const;

private:
  typedef moneypunct_byname<char, false> _Self;
  //explicitely defined as private to avoid warnings:
  moneypunct_byname(_Self const&);
  _Self& operator = (_Self const&);
  friend _Locale_name_hint* _Locale_extract_hint(moneypunct_byname<char, false>*);
};

#if !defined (_STLP_NO_WCHAR_T)
_STLP_TEMPLATE_NULL
class _STLP_CLASS_DECLSPEC moneypunct_byname<wchar_t, true> : public moneypunct<wchar_t, true>
{
public:
  typedef money_base::pattern   pattern;
  typedef wchar_t               char_type;
  typedef wstring               string_type;

  explicit moneypunct_byname _STLP_PSPEC2(wchar_t, true) (const char * __name, size_t __refs = 0,
                                                          _Locale_name_hint* __hint = 0);

protected:
  _Locale_monetary* _M_monetary;
  ~moneypunct_byname _STLP_PSPEC2(wchar_t, true) ();
  virtual wchar_t     do_decimal_point() const;
  virtual wchar_t     do_thousands_sep() const;
  virtual string      do_grouping()      const;

  virtual string_type do_curr_symbol()   const;

  virtual string_type do_positive_sign() const;
  virtual string_type do_negative_sign() const;
  virtual int         do_frac_digits()   const;

private:
  typedef moneypunct_byname<wchar_t, true> _Self;
  //explicitely defined as private to avoid warnings:
  moneypunct_byname(_Self const&);
  _Self& operator = (_Self const&);
};

_STLP_TEMPLATE_NULL
class _STLP_CLASS_DECLSPEC moneypunct_byname<wchar_t, false> : public moneypunct<wchar_t, false>
{
public:
  typedef money_base::pattern   pattern;
  typedef wchar_t               char_type;
  typedef wstring               string_type;

  explicit moneypunct_byname _STLP_PSPEC2(wchar_t, false) (const char * __name, size_t __refs = 0,
                                                           _Locale_name_hint* __hint = 0);

protected:
  _Locale_monetary* _M_monetary;
  ~moneypunct_byname _STLP_PSPEC2(wchar_t, false) ();
  virtual wchar_t     do_decimal_point() const;
  virtual wchar_t     do_thousands_sep() const;
  virtual string      do_grouping()      const;

  virtual string_type do_curr_symbol()   const;

  virtual string_type do_positive_sign() const;
  virtual string_type do_negative_sign() const;
  virtual int         do_frac_digits()   const;

private:
  typedef moneypunct_byname<wchar_t, false> _Self;
  //explicitely defined as private to avoid warnings:
  moneypunct_byname(_Self const&);
  _Self& operator = (_Self const&);
};
#endif

//===== methods ======


// money_put facets

#if defined (_STLP_LIMITED_DEFAULT_TEMPLATES)
template <class _CharT, class _OutputIter>
#else
template <class _CharT, class _OutputIter = ostreambuf_iterator<_CharT, char_traits<_CharT> > >
#endif
class money_put : public locale::facet {
  friend class _Locale_impl;

public:
  typedef _CharT               char_type;
  typedef _OutputIter          iter_type;
  typedef basic_string<_CharT, char_traits<_CharT>, allocator<_CharT> > string_type;

  money_put(size_t __refs = 0) : locale::facet(__refs) {}
  iter_type put(iter_type __s, bool __intl, ios_base& __str,
                char_type  __fill, _STLP_LONGEST_FLOAT_TYPE __units) const
    { return do_put(__s, __intl, __str, __fill, __units); }
  iter_type put(iter_type __s, bool __intl, ios_base& __str,
                char_type  __fill,
                const string_type& __digits) const
    { return do_put(__s, __intl, __str, __fill, __digits); }

  static _STLP_STATIC_MEMBER_DECLSPEC locale::id id;

protected:
  ~money_put() {}
  virtual iter_type do_put(iter_type __s, bool  __intl, ios_base&  __str,
                           char_type __fill, _STLP_LONGEST_FLOAT_TYPE __units) const;
  virtual iter_type do_put(iter_type __s, bool  __intl, ios_base&  __str,
                           char_type __fill,
                           const string_type& __digits) const;
};

# if defined (_STLP_USE_TEMPLATE_EXPORT)
_STLP_EXPORT_TEMPLATE_CLASS money_get<char, istreambuf_iterator<char, char_traits<char> > >;
_STLP_EXPORT_TEMPLATE_CLASS money_put<char, ostreambuf_iterator<char, char_traits<char> > >;
//_STLP_EXPORT_TEMPLATE_CLASS money_get<char, const char* >;
//_STLP_EXPORT_TEMPLATE_CLASS money_put<char, char* >;
#  if ! defined (_STLP_NO_WCHAR_T)
_STLP_EXPORT_TEMPLATE_CLASS money_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
_STLP_EXPORT_TEMPLATE_CLASS money_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
// _STLP_EXPORT_TEMPLATE_CLASS money_get<wchar_t, const wchar_t* >;
// _STLP_EXPORT_TEMPLATE_CLASS money_put<wchar_t, wchar_t* >;
#  endif
# endif /* _STLP_USE_TEMPLATE_EXPORT */

_STLP_END_NAMESPACE

#if defined (_STLP_EXPOSE_STREAM_IMPLEMENTATION) && !defined (_STLP_LINK_TIME_INSTANTIATION)
#  include <stl/_monetary.c>
#endif

#endif /* _STLP_INTERNAL_MONETARY_H */

// Local Variables:
// mode:C++
// End:


