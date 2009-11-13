template<class _Tp, class _Alloc>
class __simple_alloc {
  typedef _Alloc __alloc_type;
public:
  typedef typename _Alloc::value_type __alloc_value_type;
  typedef _Tp value_type;
  static size_t  _STLP_CALL __chunk(size_t __n) {
    return (sizeof(__alloc_value_type)==sizeof(value_type)) ? __n :
      ((__n*sizeof(value_type)+sizeof(__alloc_value_type)-1)/sizeof(__alloc_value_type));
  }
  static _Tp*  _STLP_CALL allocate(size_t __n) { return 0 == __n ? 0 : (_Tp*) __alloc_type::allocate(__chunk(__n)); }
  static void  _STLP_CALL deallocate(_Tp * __p, size_t __n) {
    __alloc_type::deallocate((__alloc_value_type*)__p, __chunk(__n)); }
};

// Allocator adaptor to turn an SGI-style allocator (e.g. alloc, malloc_alloc)
// into a standard-conforming allocator.   Note that this adaptor does
// *not* assume that all objects of the underlying alloc class are
// identical, nor does it assume that all of the underlying alloc's
// member functions are static member functions.  Note, also, that
// __allocator<_Tp, alloc> is essentially the same thing as allocator<_Tp>.

template <class _Tp, class _Alloc>
struct __allocator : public _Alloc {
  typedef _Alloc __underlying_alloc;

  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;
  typedef _Tp*       pointer;
  typedef const _Tp* const_pointer;
  typedef _Tp&       reference;
  typedef const _Tp& const_reference;
  typedef _Tp        value_type;

# if defined (_STLP_MEMBER_TEMPLATE_CLASSES)
  template <class _Tp1> struct rebind {
    typedef __allocator<_Tp1, _Alloc> other;
  };
# endif
  __allocator() _STLP_NOTHROW {}
  __allocator(const _Alloc& ) _STLP_NOTHROW {}
  __allocator(const __allocator<_Tp, _Alloc>& __a) _STLP_NOTHROW
    : _Alloc(__a) {}
# if defined (_STLP_MEMBER_TEMPLATES) && defined (_STLP_FUNCTION_TMPL_PARTIAL_ORDER)
  template <class _Tp1>
  __allocator(const __allocator<_Tp1, _Alloc>& __a) _STLP_NOTHROW
    : _Alloc(__a) {}
# endif
# ifdef _STLP_TRIVIAL_DESTRUCTOR_BUG
  ~__allocator() _STLP_NOTHROW {}
# endif
  pointer address(reference __x) const { return &__x; }

# if !defined (__WATCOM_CPLUSPLUS__)
  const_pointer address(const_reference __x) const { return &__x; }
# endif

  // __n is permitted to be 0.
  _Tp* allocate(size_type __n, const void* = 0) {
    if (__n > max_size())
      __THROW_BAD_ALLOC;
    return __n != 0
        ? __STATIC_CAST(_Tp*,__underlying_alloc::allocate(__n * sizeof(_Tp)))
        : 0;
  }

  // __p is not permitted to be a null pointer.
  void deallocate(pointer __p, size_type __n)
    { if (__p) __underlying_alloc::deallocate(__p, __n * sizeof(_Tp)); }

  size_type max_size() const _STLP_NOTHROW
    { return size_t(-1) / sizeof(_Tp); }

  void construct(pointer __p, const_reference __val) { _STLP_STD::_Copy_Construct(__p, __val); }
  void destroy(pointer __p) { _STLP_STD::_Destroy(__p); }

  const __underlying_alloc& __get_underlying_alloc() const { return *this; }
};

#ifdef _STLP_CLASS_PARTIAL_SPECIALIZATION
template <class _Alloc>
class __allocator<void, _Alloc> {
  typedef size_t      size_type;
  typedef ptrdiff_t   difference_type;
  typedef void*       pointer;
  typedef const void* const_pointer;
  typedef void        value_type;
#ifdef _STLP_MEMBER_TEMPLATE_CLASSES
  template <class _Tp1> struct rebind {
    typedef __allocator<_Tp1, _Alloc> other;
  };
#endif
};
#endif

template <class _Tp, class _Alloc>
inline bool  _STLP_CALL operator==(const __allocator<_Tp, _Alloc>& __a1,
                                   const __allocator<_Tp, _Alloc>& __a2)
{
  return __a1.__get_underlying_alloc() == __a2.__get_underlying_alloc();
}

#ifdef _STLP_USE_SEPARATE_RELOPS_NAMESPACE
template <class _Tp, class _Alloc>
inline bool  _STLP_CALL operator!=(const __allocator<_Tp, _Alloc>& __a1,
                                   const __allocator<_Tp, _Alloc>& __a2)
{
  return __a1.__get_underlying_alloc() != __a2.__get_underlying_alloc();
}
#endif /* _STLP_FUNCTION_TMPL_PARTIAL_ORDER */


// Comparison operators for all of the predifined SGI-style allocators.
// This ensures that __allocator<malloc_alloc> (for example) will
// work correctly.

#ifndef _STLP_NON_TYPE_TMPL_PARAM_BUG
inline bool  _STLP_CALL operator==(const __malloc_alloc&, const __malloc_alloc&)
{ return true; }

#  ifdef _STLP_FUNCTION_TMPL_PARTIAL_ORDER
inline bool  _STLP_CALL operator!=(const __malloc_alloc&, const __malloc_alloc&)
{ return false; }
#  endif

inline bool _STLP_CALL operator==(const __new_alloc&, const __new_alloc&) { return true; }

#  ifdef _STLP_USE_SEPARATE_RELOPS_NAMESPACE
inline bool _STLP_CALL operator!=(const __new_alloc&, const __new_alloc&) { return false; }
#  endif

#  if !defined (_STLP_USE_NO_IOSTREAMS)
inline bool  _STLP_CALL operator==(const __node_alloc&,
                                   const __node_alloc&)
{ return true; }

#    if defined( _STLP_FUNCTION_TMPL_PARTIAL_ORDER )

inline bool  _STLP_CALL operator!=(const __node_alloc&,
                                   const __node_alloc&)
{ return false; }
#    endif
#  endif

#endif /* _STLP_NON_TYPE_TMPL_PARAM_BUG */

template <class _Alloc>
inline bool  _STLP_CALL operator==(const __debug_alloc<_Alloc>&, const __debug_alloc<_Alloc>&) {  return true; }
# ifdef _STLP_USE_SEPARATE_RELOPS_NAMESPACE
template <class _Alloc>
inline bool  _STLP_CALL operator!=(const __debug_alloc<_Alloc>&, const __debug_alloc<_Alloc>&) {  return false; }
# endif

#if defined (_STLP_CLASS_PARTIAL_SPECIALIZATION)

// Versions for the predefined SGI-style allocators.
template <class _Tp>
struct _Alloc_traits<_Tp, __malloc_alloc> {
  typedef __allocator<_Tp, __malloc_alloc> allocator_type;
};

#  if !defined (_STLP_USE_NO_IOSTREAMS)
template <class _Tp>
struct _Alloc_traits<_Tp, __node_alloc> {
  typedef __allocator<_Tp, __node_alloc> allocator_type;
};
#  endif

template <class _Tp, class _Alloc>
struct _Alloc_traits<_Tp, __debug_alloc<_Alloc> > {
  typedef __allocator<_Tp, __debug_alloc<_Alloc> > allocator_type;
};

// Versions for the __allocator adaptor used with the predefined
// SGI-style allocators.

template <class _Tp, class _Tp1, class _Alloc>
struct _Alloc_traits<_Tp, __allocator<_Tp1, _Alloc > > {
  typedef __allocator<_Tp, _Alloc > allocator_type;
};

#endif

#if defined (_STLP_DONT_SUPPORT_REBIND_MEMBER_TEMPLATE)

// Versions for the predefined SGI-style allocators.


#  if defined (_STLP_NON_TYPE_TMPL_PARAM_BUG)

typedef __malloc_alloc __malloc_alloc_dfl;

template <class _Tp>
inline __allocator<_Tp, __malloc_alloc_dfl >& _STLP_CALL
__stl_alloc_rebind(__malloc_alloc_dfl& __a, const _Tp*) {
  return (__allocator<_Tp, __malloc_alloc_dfl >&)__a;
}

#    if !defined (_STLP_USE_NO_IOSTREAMS)
template <class _Tp>
inline __allocator<_Tp, __node_alloc>& _STLP_CALL
__stl_alloc_rebind(__node_alloc& __a, const _Tp*) {
  return (__allocator<_Tp, __node_alloc>&)__a;
}
#    endif

template <class _Tp>
inline __allocator<_Tp, __malloc_alloc_dfl > _STLP_CALL
__stl_alloc_create(const __malloc_alloc_dfl&, const _Tp*) {
  return __allocator<_Tp, __malloc_alloc_dfl > ();
}

#    if !defined (_STLP_USE_NO_IOSTREAMS)
template <class _Tp>
inline __allocator<_Tp, __node_alloc> _STLP_CALL
__stl_alloc_create(const __node_alloc&, const _Tp*) {
  return __allocator<_Tp, __node_alloc>();
}

#    endif

#  else

template <class _Tp>
inline __allocator<_Tp, __malloc_alloc>& _STLP_CALL
__stl_alloc_rebind(__malloc_alloc& __a, const _Tp*) {
  return (__allocator<_Tp, __malloc_alloc>&)__a;
}

#    if !defined (_STLP_USE_NO_IOSTREAMS)
template <class _Tp>
inline __allocator<_Tp, __node_alloc>& _STLP_CALL
__stl_alloc_rebind(__node_alloc& __a, const _Tp*) {
  return (__allocator<_Tp, __node_alloc>&)__a;
}
#    endif

template <class _Tp>
inline __allocator<_Tp, __malloc_alloc> _STLP_CALL
__stl_alloc_create(const __malloc_alloc&, const _Tp*) {
  return __allocator<_Tp, __malloc_alloc>();
}

#    if !defined (_STLP_USE_NO_IOSTREAMS)
template <class _Tp>
inline __allocator<_Tp, __node_alloc> _STLP_CALL
__stl_alloc_create(const __node_alloc&, const _Tp*) {
  return __allocator<_Tp, __node_alloc>();
}
#    endif

#  endif

template <class _Tp, class _Alloc>
inline __allocator<_Tp, __debug_alloc<_Alloc> > _STLP_CALL
__stl_alloc_create(const __debug_alloc<_Alloc>&, const _Tp*) {
  return __allocator<_Tp, __debug_alloc<_Alloc> >();
}
template <class _Tp, class _Alloc>
inline __allocator<_Tp, __debug_alloc<_Alloc> >& _STLP_CALL
__stl_alloc_rebind(__debug_alloc<_Alloc>& __a, const _Tp*) {
  return (__allocator<_Tp, __debug_alloc<_Alloc> >&)__a;
}

template <class _Tp>
inline __allocator<_Tp, __new_alloc > _STLP_CALL
__stl_alloc_create(const __new_alloc&, const _Tp*) {
  return __allocator<_Tp, __new_alloc >();
}
template <class _Tp>
inline __allocator<_Tp, __new_alloc >&  _STLP_CALL
__stl_alloc_rebind(__new_alloc& __a, const _Tp*) {
  return (__allocator<_Tp, __new_alloc >&)__a;
}

template <class _Tp1, class _Alloc, class _Tp2>
inline __allocator<_Tp2, _Alloc>& _STLP_CALL
__stl_alloc_rebind(__allocator<_Tp1, _Alloc>& __a, const _Tp2*) {
  return (__allocator<_Tp2, _Alloc>&)__a;
}

template <class _Tp1, class _Alloc, class _Tp2>
inline __allocator<_Tp2, _Alloc> _STLP_CALL
__stl_alloc_create(const __allocator<_Tp1, _Alloc>&, const _Tp2*) {
  return __allocator<_Tp2, _Alloc>();
}
#endif
