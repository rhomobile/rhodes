/*
 * Copyright (c) 2003
 * Francois Dumont
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

/*
 * All the necessary methods used for template expressions with basic_string
 * This file do not have to be macro guarded as it is only used in the _string.h
 * file and it is a part of the basic_string definition.
 */

public:
  template <class _Left, class _Right, class _StorageDir>
  basic_string(_STLP_PRIV __bstr_sum<_CharT, _Traits, _Alloc, _Left, _Right, _StorageDir> const& __s)
    : _STLP_STRING_SUM_BASE(_Reserve_t(), __s.size(), __s.get_allocator())
  { _M_append_sum(__s); }

  template <class _Left, class _Right, class _StorageDir>
  basic_string(_STLP_PRIV __bstr_sum<_CharT, _Traits, _Alloc, _Left, _Right, _StorageDir> const& __s,
               size_type __pos, size_type __n = npos,
               const allocator_type& __a = allocator_type())
    : _STLP_STRING_SUM_BASE(_Reserve_t(), (__pos <= __s.size()) ? ((min) (__n, __s.size() - __pos)) : 0, __a) {
    size_type __size = __s.size();
    if (__pos > __size)
      this->_M_throw_out_of_range();
    else
      _M_append_sum_pos(__s, __pos, (min) (__n, __size - __pos));
  }

private:
  _CharT* _M_append_fast(_STLP_PRIV __char_wrapper<_CharT> __c, _CharT *__buf) {
    _STLP_STD::_Copy_Construct(__buf, __c.getValue());
    return __buf + 1;
  }
  _CharT* _M_append_fast(_CharT const* __s, size_type __s_size, _CharT *__buf)
  { return uninitialized_copy(__s, __s + __s_size, __buf); }
  _CharT* _M_append_fast(_STLP_PRIV __cstr_wrapper<_CharT> const& __s, _CharT *__buf)
  { return _M_append_fast(__s.c_str(), __s.size(), __buf); }
  _CharT* _M_append_fast(_STLP_PRIV __bstr_wrapper<_CharT, _Traits, _Alloc> __s, _CharT *__buf)
  { return _M_append_fast(__s.b_str(), __buf); }
  _CharT* _M_append_fast(_Self const& __s, _CharT *__buf)
  { return _M_append_fast(__s.data(), __s.size(), __buf); }
  _CharT* _M_append_fast(_STLP_PRIV __sum_storage_elem<_CharT, _Traits, _Alloc> const&, _CharT *__buf)
  { return __buf; }
  template <class _Left, class _Right, class _StorageDir>
  _CharT* _M_append_fast(_STLP_PRIV __bstr_sum<_CharT, _Traits, _Alloc, _Left, _Right, _StorageDir> const& __s, _CharT *__buf)
  { return _M_append_fast(__s.getRhs(), _M_append_fast(__s.getLhs(), __buf)); }

  _CharT* _M_append_fast_pos(_STLP_PRIV __char_wrapper<_CharT> __c, _CharT *__buf, size_type /*__pos*/, size_type __n) {
    if (__n == 0)
      return __buf;
    _STLP_STD::_Copy_Construct(__buf, __c.getValue());
    return __buf + 1;
  }
  _CharT* _M_append_fast_pos(_CharT const* __s, size_type __s_size, _CharT *__buf,
                             size_type __pos, size_type __n)
  { return uninitialized_copy(__s + __pos, __s + __pos + (min)(__n, __s_size - __pos), __buf); }
  _CharT* _M_append_fast_pos(_STLP_PRIV __cstr_wrapper<_CharT> const& __s, _CharT *__buf,
                             size_type __pos, size_type __n)
  { return _M_append_fast_pos(__s.c_str(), __s.size(), __buf, __pos, __n); }
  _CharT* _M_append_fast_pos(_STLP_PRIV __bstr_wrapper<_CharT, _Traits, _Alloc> __s, _CharT *__buf,
                             size_type __pos, size_type __n)
  { return _M_append_fast_pos(__s.b_str(), __buf, __pos, __n); }
  _CharT* _M_append_fast_pos(_Self const& __s, _CharT *__buf,
                             size_type __pos, size_type __n)
  { return _M_append_fast_pos(__s.data(), __s.size(), __buf, __pos, __n); }
  _CharT* _M_append_fast_pos(_STLP_PRIV __sum_storage_elem<_CharT, _Traits, _Alloc> const&, _CharT *__buf,
                             size_type, size_type)
  { return __buf; }

  template <class _Left, class _Right, class _StorageDir>
  _CharT* _M_append_fast_pos(_STLP_PRIV __bstr_sum<_CharT, _Traits, _Alloc, _Left, _Right, _StorageDir> const& __s,
                             _CharT *__buf, size_type __pos, size_type __n) {
    if (__n == 0) {
      return __buf;
    }
    size_type __lhs_size = __s.getLhs().size();
    if (__pos < __lhs_size) {
      if (__n < (__lhs_size - __pos)) {
        return _M_append_fast_pos(__s.getLhs(), __buf, __pos, __n);
      } else {
        return _M_append_fast_pos(__s.getRhs(), _M_append_fast_pos(__s.getLhs(), __buf, __pos, __n),
                                  0, __n - (__lhs_size - __pos));
      }
    } else {
      return _M_append_fast_pos(__s.getRhs(), __buf, __pos - __lhs_size, __n);
    }
  }

  /* Note: We always force use of dynamic buffer if the short string optim option is activated
   * to avoid complicated code if the basic_string was instanciated with a non POD type.
   * In such a case we should use assignment for objects in the static array something that we
   * do not do.
   */
  size_type _M_get_additional_size(size_type __new_size, const __false_type& /*_Char_Is_POD*/) const {
#if defined (_STLP_USE_SHORT_STRING_OPTIM)
    //To avoid problem with the string assumptions, never allocate a dynamic buffer smaller or equal
    //than the static one:
    if (__new_size < _Base::_DEFAULT_SIZE + 1)
      return (_Base::_DEFAULT_SIZE + 1) - __new_size;
#endif /* _STLP_USE_SHORT_STRING_OPTIM */
    return 0;
  }
  size_type _M_get_additional_size(size_type, const __true_type& /*_Char_Is_POD*/) const
  { return 0; }

  template <class _Left, class _Right, class _StorageDir>
  _Self& _M_append_sum (_STLP_PRIV __bstr_sum<_CharT, _Traits, _Alloc, _Left, _Right, _StorageDir> const& __s) {
    size_type __s_size = __s.size();
    if (__s_size == 0)
      return *this;
    const size_type __old_size = this->size();
    if (__s_size > this->max_size() || __old_size > (this->max_size() - __s_size))
      this->_M_throw_length_error();
    size_type __offset_size = _M_get_additional_size(__old_size + __s_size, _Char_Is_POD());
    if (__old_size + __s_size + __offset_size > this->capacity()) {
      const size_type __len = __old_size + __offset_size + (max)(__old_size, __s_size) + 1;
      pointer __new_start = this->_M_end_of_storage.allocate(__len);
      pointer __new_finish = __new_start;
      _STLP_TRY {
        __new_finish = uninitialized_copy(this->_M_Start(), this->_M_Finish(), __new_start);
        __new_finish = this->_M_append_fast(__s, __new_finish);
        this->_M_construct_null(__new_finish);
      }
      _STLP_UNWIND((_STLP_STD::_Destroy_Range(__new_start,__new_finish),
                   this->_M_end_of_storage.deallocate(__new_start,__len)))
      this->_M_destroy_range();
      this->_M_deallocate_block();
      this->_M_reset(__new_start, __new_finish, __new_start + __len);
    }
    else {
      _M_append_sum_no_overflow(__s, 0, __s_size);
    }
    return *this;
  }

  template <class _Left, class _Right, class _StorageDir>
  _Self& _M_append_sum_pos(_STLP_PRIV __bstr_sum<_CharT, _Traits, _Alloc, _Left, _Right, _StorageDir> const& __s,
                           size_type __pos, size_type __n) {
    size_type __s_size = (min)(__s.size() - __pos, __n);
    if (__s_size == 0)
      return *this;
    const size_type __old_size = this->size();
    if (__s_size > this->max_size() || __old_size > (this->max_size() - __s_size))
      this->_M_throw_length_error();
    size_type __offset_size = _M_get_additional_size(__old_size + __s_size, _Char_Is_POD());
    if (__old_size + __s_size + __offset_size > this->capacity()) {
      const size_type __len = __old_size + __offset_size + (max)(__old_size, __s_size) + 1;
      pointer __new_start = this->_M_end_of_storage.allocate(__len);
      pointer __new_finish = __new_start;
      _STLP_TRY {
        __new_finish = uninitialized_copy(this->_M_Start(), this->_M_Finish(), __new_start);
        __new_finish = _M_append_fast_pos(__s, __new_finish, __pos, __s_size);
        this->_M_construct_null(__new_finish);
      }
      _STLP_UNWIND((_STLP_STD::_Destroy_Range(__new_start,__new_finish),
                   this->_M_end_of_storage.deallocate(__new_start,__len)))
      this->_M_destroy_range();
      this->_M_deallocate_block();
      this->_M_reset(__new_start, __new_finish, __new_start + __len);
    }
    else {
      _M_append_sum_no_overflow(__s, __pos, __s_size);
    }
    return *this;
  }

  template <class _Left, class _Right, class _StorageDir>
  void _M_append_sum_no_overflow(_STLP_PRIV __bstr_sum<_CharT, _Traits, _Alloc, _Left, _Right, _StorageDir> const& __s,
                                 size_type __pos, size_type __n) {
    pointer __finish = this->_M_Finish();
    _M_append_fast_pos(__s, __finish + 1, __pos + 1, __n - 1);
    _STLP_TRY {
      this->_M_construct_null(__finish + __n);
    }
    _STLP_UNWIND(this->_M_destroy_ptr_range(__finish + 1, __finish + __n))
    /* The call to the traits::assign method is only important for non POD types because the instance
     * pointed to by _M_finish has been constructed (default constructor) and should not be constructed
     * (copy constructor) once again. For POD it is irrelevant, uninitialized_copy could be fine,
     * but we are not going to make two implementations just for that.
     */
    _Traits::assign(*this->_M_finish, __s[__pos]);
    this->_M_finish += __n;
  }
