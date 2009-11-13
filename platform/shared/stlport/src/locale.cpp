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

// This file is #included into locale_impl.cpp, due to locale use many
// statics from locale_impl.cpp

_STLP_BEGIN_NAMESPACE

#if defined (_STLP_USE_MSVC6_MEM_T_BUG_WORKAROUND)
#  define locale _STLP_NO_MEM_T_NAME(loc)
#endif

locale::facet::~facet() {}

#if !defined (_STLP_MEMBER_TEMPLATES) || defined (_STLP_INLINE_MEMBER_TEMPLATES)
// members that fail to be templates
bool locale::operator()(const string& __x,
                        const string& __y) const
{ return __locale_do_operator_call(*this, __x, __y); }

#  if !defined (_STLP_NO_WCHAR_T)
bool locale::operator()(const wstring& __x,
                        const wstring& __y) const
{ return __locale_do_operator_call(*this, __x, __y); }
#  endif
#endif

void _STLP_CALL locale::_M_throw_runtime_error(const char* name) {
  char buf[256];

  if (name) {
    const char* prefix = "bad locale name: ";
#if !defined (_STLP_USE_SAFE_STRING_FUNCTIONS)
    strcpy(buf, prefix);
    strncat(buf, name, _STLP_ARRAY_SIZE(buf) - strlen(prefix));
    buf[_STLP_ARRAY_SIZE(buf) - 1] = 0;
#else
    strcpy_s(_STLP_ARRAY_AND_SIZE(buf), prefix);
    strncat_s(_STLP_ARRAY_AND_SIZE(buf), name, _TRUNCATE);
#endif
  }
  else {
#if !defined (_STLP_USE_SAFE_STRING_FUNCTIONS)
    strcpy(buf, "locale error");
#else
    strcpy_s(_STLP_ARRAY_AND_SIZE(buf), "locale error");
#endif
  }
  _STLP_THROW(runtime_error(buf));
}

// Takes a reference to a locale::id, and returns its numeric index.
// If no numeric index has yet been assigned, assigns one.  The return
// value is always positive.
static size_t _Stl_loc_get_index(locale::id& id) {
  if (id._M_index == 0) {
#if defined (_STLP_ATOMIC_INCREMENT) && \
   (!defined (_STLP_WIN32_VERSION) || (_STLP_WIN32_VERSION > 0x0400))
    static _STLP_VOLATILE __stl_atomic_t _S_index = __STATIC_CAST(__stl_atomic_t, locale::id::_S_max);
    id._M_index = _STLP_ATOMIC_INCREMENT(&_S_index);
#else
    static _STLP_STATIC_MUTEX _Index_lock _STLP_MUTEX_INITIALIZER;
    _STLP_auto_lock sentry(_Index_lock);
    size_t new_index = locale::id::_S_max++;
    id._M_index = new_index;
#endif
  }
  return id._M_index;
}

// Default constructor: create a copy of the global locale.
locale::locale() _STLP_NOTHROW
  : _M_impl(_get_Locale_impl(_Stl_get_global_locale()->_M_impl))
{}

// Copy constructor
locale::locale(const locale& L) _STLP_NOTHROW
  : _M_impl( _get_Locale_impl( L._M_impl ) )
{}

void locale::_M_insert(facet* f, locale::id& n) {
  if (f)
    _M_impl->insert(f, _Stl_loc_get_index(n));
}

locale::locale( _Locale_impl* impl ) :
  _M_impl( _get_Locale_impl( impl ) )
{}

// Create a locale from a name.
locale::locale(const char* name)
  : _M_impl(0) {
  if (!name)
    _M_throw_runtime_error(0);

  if (is_C_locale_name(name)) {
    _M_impl = _get_Locale_impl( locale::classic()._M_impl );
    return;
  }

  _Locale_impl* impl = 0;
  _STLP_TRY {
    impl = new _Locale_impl(locale::id::_S_max, name);

    // Insert categories one at a time.
    _Locale_name_hint *hint = 0;
    hint = impl->insert_ctype_facets(name, hint);
    hint = impl->insert_numeric_facets(name, hint);
    hint = impl->insert_time_facets(name, hint);
    hint = impl->insert_collate_facets(name, hint);
    hint = impl->insert_monetary_facets(name, hint);
    impl->insert_messages_facets(name, hint);
    // reassign impl
    _M_impl = _get_Locale_impl( impl );
  }
  _STLP_UNWIND(delete impl);
}

// Give L a name where all facets except those in category c
// are taken from name1, and those in category c are taken from name2.
static void _Stl_loc_combine_names(_Locale_impl* L,
                                   const char* name1, const char* name2,
                                   locale::category c) {
  if ((c & locale::all) == 0 || strcmp(name1, name2) == 0)
    L->name = name1;
  else if ((c & locale::all) == locale::all)
    L->name = name2;
  else {
    // Decompose the names.
    char ctype_buf[_Locale_MAX_SIMPLE_NAME];
    char numeric_buf[_Locale_MAX_SIMPLE_NAME];
    char time_buf[_Locale_MAX_SIMPLE_NAME];
    char collate_buf[_Locale_MAX_SIMPLE_NAME];
    char monetary_buf[_Locale_MAX_SIMPLE_NAME];
    char messages_buf[_Locale_MAX_SIMPLE_NAME];

    // TODO: check returnvalues?
    _Locale_extract_ctype_name((c & locale::ctype) ? name2 : name1, ctype_buf, 0);
    _Locale_extract_numeric_name((c & locale::numeric) ? name2 : name1, numeric_buf, 0);
    _Locale_extract_time_name((c & locale::time) ? name2 : name1, time_buf, 0);
    _Locale_extract_collate_name((c & locale::collate) ? name2 : name1, collate_buf, 0);
    _Locale_extract_monetary_name((c & locale::monetary) ? name2 : name1, monetary_buf, 0);
    _Locale_extract_messages_name((c & locale::messages) ? name2 : name1, messages_buf, 0);

    // Construct a new composite name.
    char composite_buf[_Locale_MAX_COMPOSITE_NAME];
    // TODO: check returnvalue?
    _Locale_compose_name(composite_buf,
                         ctype_buf, numeric_buf, time_buf,
                         collate_buf, monetary_buf, messages_buf,
                         name1);
    L->name = composite_buf;
  }
}

// Create a locale that's a copy of L, except that all of the facets
// in category c are instead constructed by name.
locale::locale(const locale& L, const char* name, locale::category c)
  : _M_impl(0) {
  if (name == 0 || (_Nameless == name))
    _M_throw_runtime_error(name);

  _Locale_impl* impl = 0;

  _STLP_TRY {
    impl = new _Locale_impl(*L._M_impl);
    _Stl_loc_combine_names(impl, L._M_impl->name.c_str(), name, c);

    _Locale_name_hint *hint = 0;
    if (c & locale::ctype)
      hint = impl->insert_ctype_facets(name, hint);
    if (c & locale::numeric)
      hint = impl->insert_numeric_facets(name, hint);
    if (c & locale::time)
      hint = impl->insert_time_facets(name, hint);
    if (c & locale::collate)
      hint = impl->insert_collate_facets(name, hint);
    if (c & locale::monetary)
      hint = impl->insert_monetary_facets(name, hint);
    if (c & locale::messages)
      impl->insert_messages_facets(name, hint);
    _M_impl = _get_Locale_impl( impl );
  }
  _STLP_UNWIND(delete impl)
}

// Contruct a new locale where all facets that aren't in category c
// come from L1, and all those that are in category c come from L2.
locale::locale(const locale& L1, const locale& L2, category c)
  : _M_impl(0) {
  _Locale_impl* impl = new _Locale_impl(*L1._M_impl);

  _Locale_impl* i2 = L2._M_impl;

  if (L1.name() != _Nameless && L2.name() != _Nameless)
    _Stl_loc_combine_names(impl, L1._M_impl->name.c_str(), L2._M_impl->name.c_str(), c);
  else {
    impl->name = _Nameless;
  }

  if (c & collate) {
    impl->insert( i2, _STLP_STD::collate<char>::id);
# ifndef _STLP_NO_WCHAR_T
    impl->insert( i2, _STLP_STD::collate<wchar_t>::id);
# endif
  }
  if (c & ctype) {
    impl->insert( i2, _STLP_STD::ctype<char>::id);
    impl->insert( i2, _STLP_STD::codecvt<char, char, mbstate_t>::id);
# ifndef _STLP_NO_WCHAR_T
    impl->insert( i2, _STLP_STD::ctype<wchar_t>::id);
    impl->insert( i2, _STLP_STD::codecvt<wchar_t, char, mbstate_t>::id);
# endif
  }
  if (c & monetary) {
    impl->insert( i2, _STLP_STD::moneypunct<char, true>::id);
    impl->insert( i2, _STLP_STD::moneypunct<char, false>::id);
    impl->insert( i2, _STLP_STD::money_get<char, istreambuf_iterator<char, char_traits<char> > >::id);
    impl->insert( i2, _STLP_STD::money_put<char, ostreambuf_iterator<char, char_traits<char> > >::id);
# ifndef _STLP_NO_WCHAR_T
    impl->insert( i2, _STLP_STD::moneypunct<wchar_t, true>::id);
    impl->insert( i2, _STLP_STD::moneypunct<wchar_t, false>::id);
    impl->insert( i2, _STLP_STD::money_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id);
    impl->insert( i2, _STLP_STD::money_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id);
# endif
  }
  if (c & numeric) {
    impl->insert( i2, _STLP_STD::numpunct<char>::id);
    impl->insert( i2, _STLP_STD::num_get<char, istreambuf_iterator<char, char_traits<char> > >::id);
    impl->insert( i2, _STLP_STD::num_put<char, ostreambuf_iterator<char, char_traits<char> > >::id);
# ifndef _STLP_NO_WCHAR_T
    impl->insert( i2, _STLP_STD::numpunct<wchar_t>::id);
    impl->insert( i2, num_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id);
    impl->insert( i2, num_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id);
# endif
  }
  if (c & time) {
    impl->insert( i2, _STLP_STD::time_get<char, istreambuf_iterator<char, char_traits<char> > >::id);
    impl->insert( i2, _STLP_STD::time_put<char, ostreambuf_iterator<char, char_traits<char> > >::id);
# ifndef _STLP_NO_WCHAR_T
    impl->insert( i2, _STLP_STD::time_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id);
    impl->insert( i2, _STLP_STD::time_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >::id);
# endif
  }
  if (c & messages) {
    impl->insert( i2, _STLP_STD::messages<char>::id);
# ifndef _STLP_NO_WCHAR_T
    impl->insert( i2, _STLP_STD::messages<wchar_t>::id);
# endif
  }
  _M_impl = _get_Locale_impl( impl );
}

// Destructor.
locale::~locale() _STLP_NOTHROW {
  if (_M_impl)
    _release_Locale_impl(_M_impl);
}

// Assignment operator.  Much like the copy constructor: just a bit of
// pointer twiddling.
const locale& locale::operator=(const locale& L) _STLP_NOTHROW {
  if (this->_M_impl != L._M_impl) {
    if (this->_M_impl)
      _release_Locale_impl(this->_M_impl);
    this->_M_impl = _get_Locale_impl(L._M_impl);
  }
  return *this;
}

locale::facet* locale::_M_get_facet(const locale::id& n) const {
  return n._M_index < _M_impl->size() ? _M_impl->facets_vec[n._M_index] : 0;
}

locale::facet* locale::_M_use_facet(const locale::id& n) const {
  locale::facet* f = (n._M_index < _M_impl->size() ? _M_impl->facets_vec[n._M_index] : 0);
  if (!f)
    _M_impl->_M_throw_bad_cast();
  return f;
}

string locale::name() const {
  return _M_impl->name;
}

// Compare two locales for equality.
bool locale::operator==(const locale& L) const {
  return this->_M_impl == L._M_impl ||
         (this->name() == L.name() && this->name() != _Nameless);
}

bool locale::operator!=(const locale& L) const {
  return !(*this == L);
}

// static data members.

const locale& _STLP_CALL locale::classic() {
  return *_Stl_get_classic_locale();
}

locale _STLP_CALL locale::global(const locale& L) {
  locale old(_Stl_get_global_locale()->_M_impl);
  if (_Stl_get_global_locale()->_M_impl != L._M_impl) {
    _release_Locale_impl(_Stl_get_global_locale()->_M_impl);
    // this assign should be atomic, should be fixed here:
    _Stl_get_global_locale()->_M_impl = _get_Locale_impl(L._M_impl);

    // Set the global C locale, if appropriate.
#if !defined(_STLP_NO_LOCALE_SUPPORT)
    if (L.name() != _Nameless)
      setlocale(LC_ALL, L.name().c_str());
#endif
  }

  return old;
}

# if !defined (_STLP_STATIC_CONST_INIT_BUG) && ! defined (_STLP_USE_DECLSPEC)

const locale::category locale::none;
const locale::category locale::collate;
const locale::category locale::ctype;
const locale::category locale::monetary;
const locale::category locale::numeric;
const locale::category locale::time;
const locale::category locale::messages;
const locale::category locale::all;

# endif

_STLP_END_NAMESPACE

