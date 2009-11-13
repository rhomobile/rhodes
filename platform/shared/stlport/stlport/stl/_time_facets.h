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


#ifndef _STLP_INTERNAL_TIME_FACETS_H
#define _STLP_INTERNAL_TIME_FACETS_H

#ifndef _STLP_INTERNAL_CTIME
#  include <stl/_ctime.h>                // Needed (for struct tm) by time facets
#endif

#ifndef _STLP_C_LOCALE_H
#  include <stl/c_locale.h>
#endif

#ifndef _STLP_IOS_BASE_H
#  include <stl/_ios_base.h>
#endif

_STLP_BEGIN_NAMESPACE

_STLP_MOVE_TO_PRIV_NAMESPACE

// Template functions used by time_get

// Matching input against a list of names

// Alphabetic input of the names of months and the names
// of weekdays requires matching input against a list of names.
// We use a simple generic algorithm to accomplish this.  This
// algorithm is not very efficient, especially for longer lists
// of names, but it probably does not matter for the initial
// implementation and it may never matter, since we do not expect
// this kind of input to be used very often.  The algorithm
// could be improved fairly simply by creating a new list of
// names still in the running at each iteration.  A more sophisticated
// approach would be to build a trie to do the matching.
//
// We compare each character of the input to the corresponding
// character of each name on the list that has not been eliminated,
// either because every character in the name has already been
// matched, or because some character has not been matched.  We
// continue only as long as there are some names that have not been
// eliminated.

// We do not really need a random access iterator (a forward iterator
// would do), but the extra generality makes the notation clumsier,
// and we don't really need it.

// We can recognize a failed match by the fact that the second
// component of the return value will be __name_end.

#define _MAXNAMES        64
#define _MAX_NAME_LENGTH 64

// Both time_get and time_put need a structure of type _Time_Info
// to provide names and abbreviated names for months and days,
// as well as the am/pm designator.  The month and weekday tables
// have the all the abbreviated names before all the full names.
// The _Time_Info tables are initialized using the non-template
// function _Init_timeinfo, which has two overloadings:  one
// with a single reference parameter for the table to be initialized,
// and one with a second _Locale_time * parameter.  The first form
// is called by the default constructor and the second by a special
// constructor invoked from the _byname subclass constructor to
// construct the base class.

class _STLP_CLASS_DECLSPEC _Time_Info {
public:
  string _M_dayname[14];
  string _M_monthname[24];
  string _M_am_pm[2];
  string _M_time_format;
  string _M_date_format;
  string _M_date_time_format;
  string _M_long_date_format;
  string _M_long_date_time_format;
};

void _STLP_CALL _Init_timeinfo(_Time_Info&);
void _STLP_CALL _Init_timeinfo(_Time_Info&, _Locale_time*);

_STLP_MOVE_TO_STD_NAMESPACE

class _STLP_CLASS_DECLSPEC time_base {
public:
  enum dateorder {no_order, dmy, mdy, ymd, ydm};
};

#if defined (_STLP_LIMITED_DEFAULT_TEMPLATES)
template <class _Ch, class _InIt>
#else
template <class _Ch, class _InIt = istreambuf_iterator<_Ch, char_traits<_Ch> > >
#endif
class time_get : public locale::facet, public time_base {
  friend class _Locale_impl;

public:
  typedef _Ch   char_type;
  typedef _InIt iter_type;

  explicit time_get(size_t __refs = 0) : locale::facet(__refs)
  { _STLP_PRIV _Init_timeinfo(_M_timeinfo); }
  dateorder date_order() const { return do_date_order(); }
  iter_type get_time(iter_type __s, iter_type  __end, ios_base&  __str,
                     ios_base::iostate&  __err, tm* __t) const
  { return do_get_time(__s,  __end,  __str,  __err, __t); }
  iter_type get_date(iter_type __s, iter_type  __end, ios_base&  __str,
                     ios_base::iostate&  __err, tm* __t) const
  { return do_get_date(__s,  __end,  __str,  __err, __t); }
  iter_type get_weekday(iter_type __s, iter_type  __end, ios_base&  __str,
                        ios_base::iostate&  __err, tm* __t) const
  { return do_get_weekday(__s,  __end,  __str,  __err, __t); }
  iter_type get_monthname(iter_type __s, iter_type  __end, ios_base&  __str,
                          ios_base::iostate&  __err, tm* __t) const
  { return do_get_monthname(__s,  __end,  __str,  __err, __t); }
  iter_type get_year(iter_type __s, iter_type  __end, ios_base&  __str,
                     ios_base::iostate&  __err, tm* __t) const
  { return do_get_year(__s,  __end,  __str,  __err, __t); }

  static _STLP_STATIC_MEMBER_DECLSPEC locale::id id;

protected:
  time_get(_Locale_time *, size_t __refs) : locale::facet(__refs) {}

  ~time_get() {}

  virtual dateorder do_date_order() const {return no_order;}

  virtual iter_type do_get_time(iter_type __s, iter_type  __end,
                                ios_base&, ios_base::iostate&  __err,
                                tm* __t) const;

  virtual iter_type do_get_date(iter_type __s, iter_type  __end,
                                ios_base&, ios_base::iostate& __err,
                                tm* __t) const;

  virtual iter_type do_get_weekday(iter_type __s, iter_type  __end,
                                   ios_base&,
                                   ios_base::iostate& __err,
                                   tm* __t) const;
  virtual iter_type do_get_monthname(iter_type __s, iter_type  __end,
                                     ios_base&,
                                     ios_base::iostate& __err,
                                     tm* __t) const;

  virtual iter_type do_get_year(iter_type __s, iter_type  __end,
                                ios_base&, ios_base::iostate& __err,
                                tm* __t) const;

  _STLP_PRIV _Time_Info _M_timeinfo;
};

_STLP_MOVE_TO_PRIV_NAMESPACE

time_base::dateorder _STLP_CALL __get_date_order(_Locale_time*);
_Locale_time* _STLP_CALL __acquire_time(const char* __name, _Locale_name_hint*);
void _STLP_CALL __release_time(_Locale_time* __time);

_STLP_MOVE_TO_STD_NAMESPACE

template <class _Ch, class _InIt>
class time_get_byname;

#if defined (__GNUC__) && (__GNUC__ < 3)
template <class _Ch, class _InIt>
_Locale_name_hint* _Locale_time_extract_hint(time_get_byname<_Ch, _InIt>*);
#else
_Locale_name_hint* _Locale_time_extract_hint(time_get_byname<char, istreambuf_iterator<char, char_traits<char> > >*);
#endif

#if defined (_STLP_LIMITED_DEFAULT_TEMPLATES)
template <class _Ch, class _InIt>
#else
template <class _Ch, class _InIt = istreambuf_iterator<_Ch, char_traits<_Ch> > >
#endif
class time_get_byname : public time_get<_Ch, _InIt> {
public:
  typedef  time_base::dateorder dateorder;
  typedef _InIt                 iter_type;

  explicit time_get_byname(const char* __name, size_t __refs = 0, _Locale_name_hint* __hint = 0)
    : time_get<_Ch, _InIt>((_Locale_time*) 0, __refs),
      _M_time(_STLP_PRIV __acquire_time(__name, __hint))
  { _STLP_PRIV _Init_timeinfo(this->_M_timeinfo, this->_M_time); }

protected:
  ~time_get_byname() { _STLP_PRIV __release_time(_M_time); }
  dateorder do_date_order() const { return _STLP_PRIV __get_date_order(_M_time); }

private:
  _Locale_time* _M_time;

  typedef time_get_byname<_Ch, _InIt> _Self;
  //explicitely defined as private to avoid warnings:
  time_get_byname(_Self const&);
  _Self& operator = (_Self const&);
#if defined (__GNUC__) && (__GNUC__ < 3)
  friend _Locale_name_hint* _Locale_time_extract_hint<>(_Self*);
#else
  friend _Locale_name_hint* _Locale_time_extract_hint(time_get_byname<char, istreambuf_iterator<char, char_traits<char> > >*);
#endif
};

// time_put facet

// For the formats 'x, 'X', and 'c', do_put calls the first form of
// put with the pattern obtained from _M_timeinfo._M_date_format or
// _M_timeinfo._M_time_format.

// Helper function:  __  takes a single-character
// format.  As indicated by the foregoing remark, this will never be
// 'x', 'X', or 'c'.

_STLP_MOVE_TO_PRIV_NAMESPACE

char * _STLP_CALL
__write_formatted_time(char *__buf, size_t __buf_size, char __format, char __modifier,
                       const _Time_Info& __table, const tm* __t);

template <class _OuIt>
inline _OuIt _STLP_CALL __put_time(char * __first, char * __last, _OuIt __out_ite,
                                   const ios_base& /* __loc */, char)
{ return copy(__first, __last, __out_ite); }

#if !defined (_STLP_NO_WCHAR_T)
template <class _OuIt>
_OuIt _STLP_CALL __put_time(char * __first, char * __last, _OuIt __out_ite,
                            const ios_base& __s, wchar_t);
#endif

_STLP_MOVE_TO_STD_NAMESPACE

#if defined (_STLP_LIMITED_DEFAULT_TEMPLATES)
template <class _Ch, class _OutIt>
#else
template <class _Ch, class _OutIt = ostreambuf_iterator<_Ch, char_traits<_Ch> > >
#endif
class time_put : public locale::facet, public time_base {
  friend class _Locale_impl;
public:
  typedef _Ch      char_type;
  typedef _OutIt iter_type;

  explicit time_put(size_t __refs = 0) : locale::facet(__refs)
  { _STLP_PRIV _Init_timeinfo(_M_timeinfo); }

  _OutIt put(iter_type __s, ios_base& __f, _Ch __fill,
                  const tm* __tmb,
                  const _Ch* __pat, const _Ch* __pat_end) const;

  _OutIt put(iter_type __s, ios_base& __f, _Ch  __fill,
                  const tm* __tmb, char __format, char __modifier = 0) const
  { return do_put(__s, __f,  __fill, __tmb, __format, __modifier); }

  static _STLP_STATIC_MEMBER_DECLSPEC locale::id id;

protected:
  time_put(_Locale_time* /*__time*/, size_t __refs) : locale::facet(__refs)
  {} //_STLP_PRIV _Init_timeinfo(_M_timeinfo, __time); }

  ~time_put() {}
  virtual iter_type do_put(iter_type __s, ios_base& __f,
                           char_type  /* __fill */, const tm* __tmb,
                           char __format, char /* __modifier */) const;

  _STLP_PRIV _Time_Info _M_timeinfo;
};

#if defined (_STLP_LIMITED_DEFAULT_TEMPLATES)
template <class _Ch, class _OutIt>
#else
template <class _Ch, class _OutIt = ostreambuf_iterator<_Ch, char_traits<_Ch> > >
#endif
class time_put_byname : public time_put<_Ch, _OutIt> {
  friend class _Locale_impl;
public:
  typedef time_base::dateorder dateorder;
  typedef _OutIt iter_type;
  typedef _Ch   char_type;

  explicit time_put_byname(const char * __name, size_t __refs = 0, _Locale_name_hint* __hint = 0)
    : time_put<_Ch, _OutIt>((_Locale_time*) 0, __refs),
    _M_time(_STLP_PRIV __acquire_time(__name, __hint))
  { _STLP_PRIV _Init_timeinfo(this->_M_timeinfo, this->_M_time); }

protected:
  ~time_put_byname() { _STLP_PRIV __release_time(_M_time); }

private:
  _Locale_time* _M_time;

  typedef time_put_byname<_Ch, _OutIt> _Self;
  //explicitely defined as private to avoid warnings:
  time_put_byname(_Self const&);
  _Self& operator = (_Self const&);
};

#if defined (_STLP_USE_TEMPLATE_EXPORT)
_STLP_EXPORT_TEMPLATE_CLASS time_get<char, istreambuf_iterator<char, char_traits<char> > >;
_STLP_EXPORT_TEMPLATE_CLASS time_put<char, ostreambuf_iterator<char, char_traits<char> > >;
// _STLP_EXPORT_TEMPLATE_CLASS time_get<char, const char*>;
// _STLP_EXPORT_TEMPLATE_CLASS time_put<char, char*>;
#  if !defined (_STLP_NO_WCHAR_T)
_STLP_EXPORT_TEMPLATE_CLASS time_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
_STLP_EXPORT_TEMPLATE_CLASS time_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
// _STLP_EXPORT_TEMPLATE_CLASS time_get<wchar_t, const wchar_t*>;
// _STLP_EXPORT_TEMPLATE_CLASS time_put<wchar_t, wchar_t*>;
#  endif

#endif

_STLP_END_NAMESPACE

#if defined (_STLP_EXPOSE_STREAM_IMPLEMENTATION) && !defined (_STLP_LINK_TIME_INSTANTIATION)
#  include <stl/_time_facets.c>
#endif

#endif /* _STLP_INTERNAL_TIME_FACETS_H */

// Local Variables:
// mode:C++
// End:
