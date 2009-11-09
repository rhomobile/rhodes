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
#include "stlport_prefix.h"

#include <locale>
#include <typeinfo>
#include <algorithm>
#include <stdexcept>

#include "c_locale.h"
#include "aligned_buffer.h"
#include "locale_impl.h"
#include "message_facets.h"

_STLP_BEGIN_NAMESPACE

static const string _Nameless("*");

static inline bool is_C_locale_name (const char* name)
{ return ((name[0] == 'C') && (name[1] == 0)); }

_Locale_impl * _STLP_CALL _copy_Locale_impl(_Locale_impl *loc)
{
  _STLP_ASSERT( loc != 0 );
  loc->_M_incr();
  _Locale_impl *loc_new = new _Locale_impl(*loc);
  loc->_M_decr();
  return loc_new;
}

locale::facet * _STLP_CALL _get_facet(locale::facet *f)
{
  if (f != 0)
    f->_M_incr();
  return f;
}

void _STLP_CALL _release_facet(locale::facet *&f)
{
  if ((f != 0) && (f->_M_decr() == 0)) {
    delete f;
    f = 0;
  }
}

size_t locale::id::_S_max = 39;

static void _Stl_loc_assign_ids();

static _Stl_aligned_buffer<_Locale_impl::Init> __Loc_init_buf;

_Locale_impl::Init::Init() {
  if (_M_count()._M_incr() == 1) {
    _Locale_impl::_S_initialize();
  }
}

_Locale_impl::Init::~Init() {
  if (_M_count()._M_decr() == 0) {
    _Locale_impl::_S_uninitialize();
  }
}

_Refcount_Base& _Locale_impl::Init::_M_count() const {
  static _Refcount_Base _S_count(0);
  return _S_count;
}

_Locale_impl::_Locale_impl(const char* s)
  : _Refcount_Base(0), name(s), facets_vec() {
  facets_vec.reserve( locale::id::_S_max );
  new (&__Loc_init_buf) Init();
}

_Locale_impl::_Locale_impl( _Locale_impl const& locimpl )
  : _Refcount_Base(0), name(locimpl.name), facets_vec() {
  for_each( locimpl.facets_vec.begin(), locimpl.facets_vec.end(), _get_facet);
  facets_vec = locimpl.facets_vec;
  new (&__Loc_init_buf) Init();
}

_Locale_impl::_Locale_impl( size_t n, const char* s)
  : _Refcount_Base(0), name(s), facets_vec(n, 0) {
  new (&__Loc_init_buf) Init();
}

_Locale_impl::~_Locale_impl() {
  (&__Loc_init_buf)->~Init();
  for_each( facets_vec.begin(), facets_vec.end(), _release_facet);
}

// Initialization of the locale system.  This must be called before
// any locales are constructed.  (Meaning that it must be called when
// the I/O library itself is initialized.)
void _STLP_CALL _Locale_impl::_S_initialize() {
  _Stl_loc_assign_ids();
  make_classic_locale();
}

// Release of the classic locale ressources. Has to be called after the last
// locale destruction and not only after the classic locale destruction as
// the facets can be shared between different facets.
void _STLP_CALL _Locale_impl::_S_uninitialize() {
  //Not necessary anymore as classic facets are now 'normal' dynamically allocated
  //facets with a reference counter telling to _release_facet when the facet can be
  //deleted.
  //free_classic_locale();
}

// _Locale_impl non-inline member functions.
void _STLP_CALL _Locale_impl::_M_throw_bad_cast() {
  _STLP_THROW(bad_cast());
}

void _Locale_impl::insert( _Locale_impl *from, const locale::id& n ) {
  size_t index = n._M_index;
  if (index > 0 && index < from->size()) {
    this->insert( from->facets_vec[index], index);
  }
}

locale::facet* _Locale_impl::insert(locale::facet *f, size_t index) {
  if (f == 0 || index == 0)
    return 0;

  if (index >= facets_vec.size()) {
    facets_vec.resize(index + 1);
  }

  if (f != facets_vec[index])
  {
    _release_facet(facets_vec[index]);
    facets_vec[index] = _get_facet(f);
  }

  return f;
}

#if !defined (__DMC__)
_Locale_name_hint* _Locale_extract_hint(ctype_byname<char>* ct)
{ return _Locale_get_ctype_hint(ct->_M_ctype); }
_Locale_name_hint* _Locale_extract_hint(numpunct_byname<char>* punct)
{ return _Locale_get_numeric_hint(punct->_M_numeric); }
#  if defined (__GNUC__) && (__GNUC__ < 3)
template <class _Ch, class _InIt>
_Locale_name_hint* _Locale_time_extract_hint(time_get_byname<_Ch, _InIt>* tget)
#  else
_Locale_name_hint* _Locale_time_extract_hint(time_get_byname<char, istreambuf_iterator<char, char_traits<char> > >* tget)
#  endif
{ return _Locale_get_time_hint(tget->_M_time); }
_Locale_name_hint* _Locale_extract_hint(collate_byname<char>* coll)
{ return _Locale_get_collate_hint(coll->_M_collate); }
_Locale_name_hint* _Locale_extract_hint(moneypunct_byname<char, false>* money)
{ return _Locale_get_monetary_hint(money->_M_monetary); }
#endif

//
// <locale> content which is dependent on the name
//

template <class Facet>
static inline locale::facet* _Locale_insert(_Locale_impl *__that, Facet* f)
{ return __that->insert(f, Facet::id._M_index); }

/*
 * Six functions, one for each category.  Each of them takes a
 * _Locale* and a name, constructs that appropriate category
 * facets by name, and inserts them into the locale.
 */
_Locale_name_hint* _Locale_impl::insert_ctype_facets(const char* pname, _Locale_name_hint* hint) {
  char buf[_Locale_MAX_SIMPLE_NAME];
  _Locale_impl* i2 = locale::classic()._M_impl;

  if (pname == 0 || pname[0] == 0)
    pname = _Locale_ctype_default(buf);

  if (pname == 0 || pname[0] == 0 || is_C_locale_name(pname)) {
    this->insert(i2, ctype<char>::id);
#ifndef _STLP_NO_MBSTATE_T
    this->insert(i2, codecvt<char, char, mbstate_t>::id);
#endif
#ifndef _STLP_NO_WCHAR_T
    this->insert(i2, ctype<wchar_t>::id);
#  ifndef _STLP_NO_MBSTATE_T
    this->insert(i2, codecvt<wchar_t, char, mbstate_t>::id);
#  endif
#endif
  } else {
    ctype<char>*    ct                      = 0;
#ifndef _STLP_NO_MBSTATE_T
    codecvt<char, char, mbstate_t>*    cvt  = 0;
#endif
#ifndef _STLP_NO_WCHAR_T
    ctype<wchar_t>* wct                     = 0;
    codecvt<wchar_t, char, mbstate_t>* wcvt = 0;
#endif
    _STLP_TRY {
      ctype_byname<char> *ctbn = new ctype_byname<char>(pname, 0, hint);
      ct   = ctbn;
#if !defined (__DMC__)
      if (hint == 0) hint = _Locale_extract_hint(ctbn);
#endif
#ifndef _STLP_NO_MBSTATE_T
      cvt  = new codecvt_byname<char, char, mbstate_t>(pname);
#endif
#ifndef _STLP_NO_WCHAR_T
      wct  = new ctype_byname<wchar_t>(pname, 0, hint);
      wcvt = new codecvt_byname<wchar_t, char, mbstate_t>(pname, 0, hint);
#endif
    }

#ifndef _STLP_NO_WCHAR_T
#  ifdef _STLP_NO_MBSTATE_T
    _STLP_UNWIND(delete ct; delete wct; delete wcvt);
#  else
    _STLP_UNWIND(delete ct; delete wct; delete cvt; delete wcvt);
#  endif
#else
#  ifdef _STLP_NO_MBSTATE_T
    _STLP_UNWIND(delete ct);
#  else
    _STLP_UNWIND(delete ct; delete cvt);
#  endif
#endif
    _Locale_insert(this, ct);
#ifndef _STLP_NO_MBSTATE_T
    _Locale_insert(this, cvt);
#endif
#ifndef _STLP_NO_WCHAR_T
    _Locale_insert(this, wct);
    _Locale_insert(this, wcvt);
#endif
  }
  return hint;
}

_Locale_name_hint* _Locale_impl::insert_numeric_facets(const char* pname, _Locale_name_hint* hint) {
  _Locale_impl* i2 = locale::classic()._M_impl;

  numpunct<char>* punct = 0;
  num_get<char, istreambuf_iterator<char, char_traits<char> > > *get = 0;
  num_put<char, ostreambuf_iterator<char, char_traits<char> > > *put = 0;
#ifndef _STLP_NO_WCHAR_T
  numpunct<wchar_t>* wpunct = 0;
  num_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > > *wget = 0;
  num_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > > *wput = 0;
#endif

  char buf[_Locale_MAX_SIMPLE_NAME];
  if (pname == 0 || pname[0] == 0)
    pname = _Locale_numeric_default(buf);

  if (pname == 0 || pname[0] == 0 || is_C_locale_name(pname)) {
    this->insert(i2, numpunct<char>::id);
    this->insert(i2,
                 num_put<char, ostreambuf_iterator<char, char_traits<char> >  >::id);
    this->insert(i2,
                 num_get<char, istreambuf_iterator<char, char_traits<char> > >::id);
#ifndef _STLP_NO_WCHAR_T
    this->insert(i2, numpunct<wchar_t>::id);
    this->insert(i2,
                 num_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> >  >::id);
    this->insert(i2,
                 num_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id);
#endif
  }
  else {
    _STLP_TRY {
      numpunct_byname<char> *punctbn = new numpunct_byname<char>(pname, 0, hint);
      punct  = punctbn;
#if !defined (__DMC__)
      if (hint == 0) hint = _Locale_extract_hint(punctbn);
#endif
      get    = new num_get<char, istreambuf_iterator<char, char_traits<char> > >;
      put    = new num_put<char, ostreambuf_iterator<char, char_traits<char> > >;
#ifndef _STLP_NO_WCHAR_T
      wpunct = new numpunct_byname<wchar_t>(pname, 0, hint);
      wget   = new num_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
      wput   = new num_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
#endif
    }
#ifndef _STLP_NO_WCHAR_T
    _STLP_UNWIND(delete punct; delete wpunct; delete get; delete wget; delete put; delete wput);
#else
    _STLP_UNWIND(delete punct; delete get;delete put);
#endif

    _Locale_insert(this,punct);
    _Locale_insert(this,get);
    _Locale_insert(this,put);

#ifndef _STLP_NO_WCHAR_T
    _Locale_insert(this,wpunct);
    _Locale_insert(this,wget);
    _Locale_insert(this,wput);
#endif
  }
  return hint;
}

_Locale_name_hint* _Locale_impl::insert_time_facets(const char* pname, _Locale_name_hint* hint) {
  _Locale_impl* i2 = locale::classic()._M_impl;
  time_get<char, istreambuf_iterator<char, char_traits<char> > > *get = 0;
  time_put<char, ostreambuf_iterator<char, char_traits<char> > > *put = 0;
#ifndef _STLP_NO_WCHAR_T
  time_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > > *wget = 0;
  time_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > > *wput = 0;
#endif

  char buf[_Locale_MAX_SIMPLE_NAME];
  if (pname == 0 || pname[0] == 0)
    pname = _Locale_time_default(buf);

  if (pname == 0 || pname[0] == 0 || is_C_locale_name(pname)) {

    this->insert(i2,
                 time_get<char, istreambuf_iterator<char, char_traits<char> > >::id);
    this->insert(i2,
                 time_put<char, ostreambuf_iterator<char, char_traits<char> > >::id);
#ifndef _STLP_NO_WCHAR_T
    this->insert(i2,
                 time_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id);
    this->insert(i2,
                 time_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id);
#endif
  } else {
    _STLP_TRY {
      time_get_byname<char, istreambuf_iterator<char, char_traits<char> > > *getbn =
        new time_get_byname<char, istreambuf_iterator<char, char_traits<char> > >(pname, 0, hint);
      get  = getbn;
#if !defined (__DMC__)
      if (hint == 0) hint = _Locale_time_extract_hint(getbn);
#endif
      put  = new time_put_byname<char, ostreambuf_iterator<char, char_traits<char> > >(pname, 0, hint);
#ifndef _STLP_NO_WCHAR_T
      wget = new time_get_byname<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >(pname, 0, hint);
      wput = new time_put_byname<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >(pname, 0, hint);
#endif
    }
#ifndef _STLP_NO_WCHAR_T
    _STLP_UNWIND(delete get; delete wget; delete put; delete wput);
#else
    _STLP_UNWIND(delete get; delete put);
#endif
    _Locale_insert(this,get);
    _Locale_insert(this,put);
#ifndef _STLP_NO_WCHAR_T
    _Locale_insert(this,wget);
    _Locale_insert(this,wput);
#endif
  }
  return hint;
}

_Locale_name_hint* _Locale_impl::insert_collate_facets(const char* nam, _Locale_name_hint* hint) {
  _Locale_impl* i2 = locale::classic()._M_impl;

  collate<char> *col = 0;
#ifndef _STLP_NO_WCHAR_T
  collate<wchar_t> *wcol = 0;
#endif

  char buf[_Locale_MAX_SIMPLE_NAME];
  if (nam == 0 || nam[0] == 0)
    nam = _Locale_collate_default(buf);

  if (nam == 0 || nam[0] == 0 || is_C_locale_name(nam)) {
    this->insert(i2, collate<char>::id);
#ifndef _STLP_NO_WCHAR_T
    this->insert(i2, collate<wchar_t>::id);
#endif
  }
  else {
    _STLP_TRY {
      collate_byname<char> *colbn = new collate_byname<char>(nam, 0, hint);
      col   = colbn;
#if !defined (__DMC__)
      if (hint == 0) hint = _Locale_extract_hint(colbn);
#endif
#ifndef _STLP_NO_WCHAR_T
      wcol  = new collate_byname<wchar_t>(nam, 0, hint);
#endif
    }
#ifndef _STLP_NO_WCHAR_T
    _STLP_UNWIND(delete col; delete wcol);
#else
    _STLP_UNWIND(delete col);
#endif
    _Locale_insert(this,col);
#ifndef _STLP_NO_WCHAR_T
    _Locale_insert(this,wcol);
#endif
  }
  return hint;
}

_Locale_name_hint* _Locale_impl::insert_monetary_facets(const char* pname, _Locale_name_hint* hint) {
  _Locale_impl* i2 = locale::classic()._M_impl;

  moneypunct<char, false> *punct = 0;
  moneypunct<char, true> *ipunct = 0;
  money_get<char, istreambuf_iterator<char, char_traits<char> > > *get = 0;
  money_put<char, ostreambuf_iterator<char, char_traits<char> > > *put = 0;

#ifndef _STLP_NO_WCHAR_T
  moneypunct<wchar_t, false>* wpunct = 0;
  moneypunct<wchar_t, true>* wipunct = 0;
  money_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > > *wget = 0;
  money_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > > *wput = 0;
#endif

  char buf[_Locale_MAX_SIMPLE_NAME];
  if (pname == 0 || pname[0] == 0)
    pname = _Locale_monetary_default(buf);

  if (pname == 0 || pname[0] == 0 || is_C_locale_name(pname)) {
    this->insert(i2, moneypunct<char, false>::id);
    this->insert(i2, moneypunct<char, true>::id);
    this->insert(i2, money_get<char, istreambuf_iterator<char, char_traits<char> > >::id);
    this->insert(i2, money_put<char, ostreambuf_iterator<char, char_traits<char> > >::id);
#ifndef _STLP_NO_WCHAR_T
    this->insert(i2, moneypunct<wchar_t, false>::id);
    this->insert(i2, moneypunct<wchar_t, true>::id);
    this->insert(i2, money_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id);
    this->insert(i2, money_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id);
#endif
  }
  else {
    _STLP_TRY {
      moneypunct_byname<char, false>* punctbn = new moneypunct_byname<char, false>(pname, 0, hint);
      punct   = punctbn;
#if !defined (__DMC__)
      if (hint == 0) hint = _Locale_extract_hint(punctbn);
#endif
      ipunct  = new moneypunct_byname<char, true>(pname, 0, hint);
      get     = new money_get<char, istreambuf_iterator<char, char_traits<char> > >;
      put     = new money_put<char, ostreambuf_iterator<char, char_traits<char> > >;
#ifndef _STLP_NO_WCHAR_T
      wpunct  = new moneypunct_byname<wchar_t, false>(pname, 0, hint);
      wipunct = new moneypunct_byname<wchar_t, true>(pname, 0, hint);
      wget    = new money_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
      wput    = new money_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
#endif
    }
#ifndef _STLP_NO_WCHAR_T
    _STLP_UNWIND(delete punct; delete ipunct; delete wpunct; delete wipunct; delete get; delete wget; delete put; delete wput);
#else
    _STLP_UNWIND(delete punct; delete ipunct; delete get; delete put);
#endif
    _Locale_insert(this,punct);
    _Locale_insert(this,ipunct);
    _Locale_insert(this,get);
    _Locale_insert(this,put);
#ifndef _STLP_NO_WCHAR_T
    _Locale_insert(this,wget);
    _Locale_insert(this,wpunct);
    _Locale_insert(this,wipunct);
    _Locale_insert(this,wput);
#endif
  }
  return hint;
}

_Locale_name_hint* _Locale_impl::insert_messages_facets(const char* pname, _Locale_name_hint* hint) {
  _Locale_impl* i2 = locale::classic()._M_impl;
  messages<char> *msg = 0;
#ifndef _STLP_NO_WCHAR_T
  messages<wchar_t> *wmsg = 0;
#endif

  char buf[_Locale_MAX_SIMPLE_NAME];
  if (pname == 0 || pname[0] == 0)
    pname = _Locale_messages_default(buf);

  if (pname == 0 || pname[0] == 0 || is_C_locale_name(pname)) {
    this->insert(i2, messages<char>::id);
#ifndef _STLP_NO_WCHAR_T
    this->insert(i2, messages<wchar_t>::id);
#endif
  }
  else {
    _STLP_TRY {
      msg  = new messages_byname<char>(pname, 0, hint);
#ifndef _STLP_NO_WCHAR_T
      wmsg = new messages_byname<wchar_t>(pname, 0, hint);
#endif
    }
#ifndef _STLP_NO_WCHAR_T
    _STLP_UNWIND(delete msg; delete wmsg);
#else
    _STLP_UNWIND(delete msg);
#endif
    _Locale_insert(this,msg);
#ifndef _STLP_NO_WCHAR_T
    _Locale_insert(this,wmsg);
#endif
  }
  return hint;
}

static void _Stl_loc_assign_ids() {
  // This assigns ids to every facet that is a member of a category,
  // and also to money_get/put, num_get/put, and time_get/put
  // instantiated using ordinary pointers as the input/output
  // iterators.  (The default is [io]streambuf_iterator.)

  money_get<char, istreambuf_iterator<char, char_traits<char> > >::id._M_index          = 8;
  //money_get<char, const char*>::id._M_index                                             = 9;
  money_put<char, ostreambuf_iterator<char, char_traits<char> > >::id._M_index          = 10;
  //money_put<char, char*>::id._M_index                                                   = 11;

  num_get<char, istreambuf_iterator<char, char_traits<char> > >::id._M_index            = 12;
  //num_get<char, const char*>::id._M_index                                               = 13;
  num_put<char, ostreambuf_iterator<char, char_traits<char> > >::id._M_index            = 14;
  //num_put<char, char*>::id._M_index                                                     = 15;
  time_get<char, istreambuf_iterator<char, char_traits<char> > >::id._M_index           = 16;
  //time_get<char, const char*>::id._M_index                                              = 17;
  time_put<char, ostreambuf_iterator<char, char_traits<char> > >::id._M_index           = 18;
  //time_put<char, char*>::id._M_index                                                    = 19;

#ifndef _STLP_NO_WCHAR_T
  money_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id._M_index = 27;
  //money_get<wchar_t, const wchar_t*>::id._M_index                                       = 28;
  money_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id._M_index = 29;
  //money_put<wchar_t, wchar_t*>::id._M_index                                             = 30;

  num_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id._M_index   = 31;
  //num_get<wchar_t, const wchar_t*>::id._M_index                                         = 32;
  num_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > > ::id._M_index  = 33;
  //num_put<wchar_t, wchar_t*>::id._M_index                                               = 34;
  time_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id._M_index  = 35;
  //time_get<wchar_t, const wchar_t*>::id._M_index                                        = 36;
  time_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id._M_index  = 37;
  //time_put<wchar_t, wchar_t*>::id._M_index                                              = 38;
#endif
  //  locale::id::_S_max                               = 39;
}

// To access those static instance use the getter below, they guaranty
// a correct initialization.
static locale *_Stl_classic_locale = 0;
static locale *_Stl_global_locale = 0;

static locale* _Stl_get_classic_locale() {
  static _Locale_impl::Init init;
  return _Stl_classic_locale;
}

static locale* _Stl_get_global_locale() {
  static _Locale_impl::Init init;
  return _Stl_global_locale;
}

#if defined (_STLP_MSVC) || defined (__ICL) || defined (__ISCPP__)
/*
 * The following static variable needs to be initialized before STLport
 * users static variable in order for him to be able to use Standard
 * streams in its variable initialization.
 * This variable is here because MSVC do not allow to change the initialization
 * segment in a given translation unit, iostream.cpp already contains an
 * initialization segment specification.
 */
#  pragma warning (disable : 4073)
#  pragma init_seg(lib)
#endif

static ios_base::Init _IosInit;

void _Locale_impl::make_classic_locale() {
  // This funcion will be called once: during build classic _Locale_impl

  // The classic locale contains every facet that belongs to a category.
  static _Stl_aligned_buffer<_Locale_impl> _Locale_classic_impl_buf;
  _Locale_impl *classic = new(&_Locale_classic_impl_buf) _Locale_impl("C");

  locale::facet* classic_facets[] = {
    0,
    new collate<char>(1),
    new ctype<char>(0, false, 1),
#ifndef _STLP_NO_MBSTATE_T
    new codecvt<char, char, mbstate_t>(1),
#else
    0,
#endif
    new moneypunct<char, true>(1),
    new moneypunct<char, false>(1),
    new numpunct<char>(1),
    new messages<char>(new _STLP_PRIV _Messages()),
    new money_get<char, istreambuf_iterator<char, char_traits<char> > >(1),
    0,
    new money_put<char, ostreambuf_iterator<char, char_traits<char> > >(1),
    0,
    new num_get<char, istreambuf_iterator<char, char_traits<char> > >(1),
    0,
    new num_put<char, ostreambuf_iterator<char, char_traits<char> > >(1),
    0,
    new time_get<char, istreambuf_iterator<char, char_traits<char> > >(1),
    0,
    new time_put<char, ostreambuf_iterator<char, char_traits<char> > >(1),
    0,
#ifndef _STLP_NO_WCHAR_T
    new collate<wchar_t>(1),
    new ctype<wchar_t>(1),

#  ifndef _STLP_NO_MBSTATE_T
    new codecvt<wchar_t, char, mbstate_t>(1),
#  else
    0,
#  endif
    new moneypunct<wchar_t, true>(1),
    new moneypunct<wchar_t, false>(1),
    new numpunct<wchar_t>(1),
    new messages<wchar_t>(new _STLP_PRIV _Messages()),

    new money_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >(1),
    0,
    new money_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >(1),
    0,

    new num_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >(1),
    0,
    new num_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >(1),
    0,
    new time_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >(1),
    0,
    new time_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >(1),
    0,
#endif
    0
  };

  const size_t nb_classic_facets = sizeof(classic_facets) / sizeof(locale::facet *);
  classic->facets_vec.reserve(nb_classic_facets);
  classic->facets_vec.assign(&classic_facets[0], &classic_facets[0] + nb_classic_facets);

  static locale _Locale_classic(classic);
  _Stl_classic_locale = &_Locale_classic;

  static locale _Locale_global(_copy_Locale_impl(classic));
  _Stl_global_locale = &_Locale_global;
}

#if defined (__BORLANDC__) && (__BORLANDC__ < 0x564)
template <>
_STLP_DECLSPEC locale::id time_get<char, istreambuf_iterator<char, char_traits<char> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id time_get<char, const char*>::id;
*/

template <>
_STLP_DECLSPEC locale::id time_put<char, ostreambuf_iterator<char, char_traits<char> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id time_put<char, char*>::id;
*/

#  if !defined (_STLP_NO_WCHAR_T)
template <>
_STLP_DECLSPEC locale::id time_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id time_get<wchar_t, const wchar_t*>::id;
*/

template <>
_STLP_DECLSPEC locale::id time_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id time_put<wchar_t, wchar_t*>::id;
*/
#  endif /* _STLP_NO_WCHAR_T */

template <>
_STLP_DECLSPEC locale::id money_get<char, istreambuf_iterator<char, char_traits<char> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id money_get<char, const char*>::id;
*/

template <>
_STLP_DECLSPEC locale::id money_put<char, ostreambuf_iterator<char, char_traits<char> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id money_put<char, char*>::id;
*/

#  if !defined (_STLP_NO_WCHAR_T)
template <>
_STLP_DECLSPEC locale::id money_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id money_get<wchar_t, const wchar_t*>::id;
*/

template <>
_STLP_DECLSPEC locale::id money_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id money_put<wchar_t, wchar_t*>::id;
*/
#  endif

template <>
_STLP_DECLSPEC locale::id num_get<char, istreambuf_iterator<char, char_traits<char> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id num_get<char, const char*>::id;
*/

#  if !defined (STLP_NO_WCHAR_T)
template <>
_STLP_DECLSPEC locale::id num_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id num_get<wchar_t, const wchar_t*>::id;
*/
#  endif

template <>
_STLP_DECLSPEC locale::id num_put<char, ostreambuf_iterator<char, char_traits<char> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id num_put<char, char*>::id;
*/

#  if !defined (_STLP_NO_WCHAR_T)
template <>
_STLP_DECLSPEC locale::id num_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id;
/*
template <>
_STLP_DECLSPEC locale::id num_put<wchar_t, wchar_t*>::id;
*/
#  endif
#endif

// Declarations of (non-template) facets' static data members
// size_t locale::id::_S_max = 39; // made before

_STLP_STATIC_MEMBER_DECLSPEC locale::id collate<char>::id = { 1 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id ctype<char>::id = { 2 };

#ifndef _STLP_NO_MBSTATE_T
_STLP_STATIC_MEMBER_DECLSPEC locale::id codecvt<char, char, mbstate_t>::id = { 3 };
#  ifndef _STLP_NO_WCHAR_T
_STLP_STATIC_MEMBER_DECLSPEC locale::id codecvt<wchar_t, char, mbstate_t>::id = { 22 };
#  endif
#endif

_STLP_STATIC_MEMBER_DECLSPEC locale::id moneypunct<char, true>::id = { 4 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id moneypunct<char, false>::id = { 5 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id numpunct<char>::id = { 6 } ;
_STLP_STATIC_MEMBER_DECLSPEC locale::id messages<char>::id = { 7 };

#if defined (__BORLANDC__) && (__BORLANDC__ >= 0x564)
_STLP_STATIC_MEMBER_DECLSPEC locale::id money_get<char, istreambuf_iterator<char, char_traits<char> > >::id = { 8 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id money_put<char, ostreambuf_iterator<char, char_traits<char> > >::id = { 10 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id num_get<char, istreambuf_iterator<char, char_traits<char> > >::id = { 12 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id num_put<char, ostreambuf_iterator<char, char_traits<char> > >::id = { 14 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id time_get<char, istreambuf_iterator<char, char_traits<char> > >::id = { 16 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id time_put<char, ostreambuf_iterator<char, char_traits<char> > >::id = { 18 };
/*
_STLP_STATIC_MEMBER_DECLSPEC locale::id money_get<char, const char*>::id = { 9 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id money_put<char, char*>::id = { 11 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id num_get<char, const char*>::id = { 13 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id num_put<char, char*>::id = { 15 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id time_get<char, const char*>::id = { 17 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id time_put<char, char*>::id = { 19 };
*/
#endif

#ifndef _STLP_NO_WCHAR_T
_STLP_STATIC_MEMBER_DECLSPEC locale::id collate<wchar_t>::id = { 20 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id ctype<wchar_t>::id = { 21 };

_STLP_STATIC_MEMBER_DECLSPEC locale::id moneypunct<wchar_t, true>::id = { 23 } ;
_STLP_STATIC_MEMBER_DECLSPEC locale::id moneypunct<wchar_t, false>::id = { 24 } ;

_STLP_STATIC_MEMBER_DECLSPEC locale::id numpunct<wchar_t>::id = { 25 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id messages<wchar_t>::id = { 26 };

#if defined (__BORLANDC__) && (__BORLANDC__ >= 0x564)
_STLP_STATIC_MEMBER_DECLSPEC locale::id money_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id = { 27 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id money_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id = { 29 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id num_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id = { 31 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id num_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > > ::id = { 33 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id time_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id = { 35 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id time_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id = { 37 };
/*
_STLP_STATIC_MEMBER_DECLSPEC locale::id money_get<wchar_t, const wchar_t*>::id = { 28 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id money_put<wchar_t, wchar_t*>::id = { 30 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id num_get<wchar_t, const wchar_t*>::id = { 32 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id num_put<wchar_t, wchar_t*>::id = { 34 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id time_get<wchar_t, const wchar_t*>::id = { 36 };
_STLP_STATIC_MEMBER_DECLSPEC locale::id time_put<wchar_t, wchar_t*>::id = { 38 };
*/
#  endif
#endif

_STLP_DECLSPEC _Locale_impl* _STLP_CALL _get_Locale_impl(_Locale_impl *loc)
{
  _STLP_ASSERT( loc != 0 );
  loc->_M_incr();
  return loc;
}

void _STLP_CALL _release_Locale_impl(_Locale_impl *& loc)
{
  _STLP_ASSERT( loc != 0 );
  if (loc->_M_decr() == 0) {
    if (*loc != *_Stl_classic_locale)
      delete loc;
    else
      loc->~_Locale_impl();
    loc = 0;
  }
}

_STLP_DECLSPEC _Locale_impl* _STLP_CALL _copy_Nameless_Locale_impl(_Locale_impl *loc)
{
  _STLP_ASSERT( loc != 0 );
  loc->_M_incr();
  _Locale_impl *loc_new = new _Locale_impl(*loc);
  loc->_M_decr();
  loc_new->name = _Nameless;
  return loc_new;
}

_STLP_END_NAMESPACE


// locale use many static functions/pointers from this file:
// to avoid making ones extern, simple #include implementation of locale

#include "locale.cpp"

