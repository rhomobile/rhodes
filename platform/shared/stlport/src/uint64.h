//  uint64.h
//  minimal double precision unsigned int arithmetics for numeric_facets support.
//  Written by Tsutomu Yoshida, Minokamo, Japan. 03/25/2000
#ifndef _UINT64_H
#define _UINT64_H

template <class _Tp>
class _compound_int : public _Tp {
public:
  typedef _compound_int<_Tp> _Self;

  // Constructors, destructor, assignment operator.
  _compound_int();                    // platform specific
  _compound_int(unsigned long);              // platform specific
  _compound_int(unsigned long hi, unsigned long lo);  // platform specific
  _compound_int(const _Tp& rhs) : _Tp(rhs) {}

  // Arithmetic op= operations involving two _compound_int.
  _Self& operator+= (const _Self&);            // platform specific
  _Self& operator-= (const _Self&);            // platform specific
  _Self& operator*= (const _Self&);            // platform specific
  _Self& operator/= (const _Self&);            // platform specific
  _Self& operator%= (const _Self&);            // platform specific
  _Self& operator&= (const _Self&);            // platform specific
  _Self& operator|= (const _Self&);            // platform specific
  _Self& operator^= (const _Self&);            // platform specific

  // Arithmetic op= operations involving built-in integer.
  _Self& operator<<= (unsigned int);          // platform specific
  _Self& operator>>= (unsigned int);          // platform specific

  _Self& operator=  (unsigned long rhs) { return *this =  _Self(rhs); }
  _Self& operator+= (unsigned long rhs) { return *this += _Self(rhs); }
  _Self& operator-= (unsigned long rhs) { return *this -= _Self(rhs); }
  _Self& operator*= (unsigned long rhs) { return *this *= _Self(rhs); }
  _Self& operator/= (unsigned long rhs) { return *this /= _Self(rhs); }
  _Self& operator%= (unsigned long rhs) { return *this %= _Self(rhs); }
  _Self& operator&= (unsigned long rhs) { return *this &= _Self(rhs); }
  _Self& operator|= (unsigned long rhs) { return *this |= _Self(rhs); }
  _Self& operator^= (unsigned long rhs) { return *this ^= _Self(rhs); }

  // Increment and decrement
  _Self& operator++() { return (*this) += 1; }
  _Self& operator--() { return (*this) -= 1; }
  _Self operator++(int) { _Self temp(*this); ++(*this); return temp; }
  _Self operator--(int) { _Self temp(*this); --(*this); return temp; }
};

// Comparison operators.
template <class _Tp> bool operator==(const _compound_int<_Tp>&, const _compound_int<_Tp>&);  // platform specific
template <class _Tp> bool operator<(const _compound_int<_Tp>&, const _compound_int<_Tp>&);  // platform specific

template <class _Tp> inline bool operator==(const _compound_int<_Tp>& lhs, unsigned long rhs) { return lhs == _compound_int<_Tp>(rhs); }
template <class _Tp> inline bool operator==(unsigned long lhs, const _compound_int<_Tp>& rhs) { return rhs == lhs; }

template <class _Tp> inline bool operator< (const _compound_int<_Tp>& lhs, unsigned long rhs) { return lhs < _compound_int<_Tp>(rhs); }
template <class _Tp> inline bool operator< (unsigned long lhs, const _compound_int<_Tp>& rhs) { return _compound_int<_Tp>(lhs) < rhs; }

template <class _Tp> inline bool operator!=(const _compound_int<_Tp>& lhs, unsigned long rhs) { return !(lhs == rhs); }
template <class _Tp> inline bool operator!=(unsigned long lhs, const _compound_int<_Tp>& rhs) { return !(lhs == rhs); }

template <class _Tp> inline bool operator> (const _compound_int<_Tp>& lhs, unsigned long rhs) { return rhs < lhs; }
template <class _Tp> inline bool operator> (unsigned long lhs, const _compound_int<_Tp>& rhs) { return rhs < lhs; }

template <class _Tp> inline bool operator<=(const _compound_int<_Tp>& lhs, unsigned long rhs) { return !(lhs > rhs); }
template <class _Tp> inline bool operator<=(unsigned long lhs, const _compound_int<_Tp>& rhs) { return !(lhs > rhs); }

template <class _Tp> inline bool operator>=(const _compound_int<_Tp>& lhs, unsigned long rhs) { return !(lhs < rhs); }
template <class _Tp> inline bool operator>=(unsigned long lhs, const _compound_int<_Tp>& rhs) { return !(lhs < rhs); }

// Unary non-member arithmetic operators.
template <class _Tp> unsigned long to_ulong(const _compound_int<_Tp>&);      // platform specific
template <class _Tp> _compound_int<_Tp> operator~(const _compound_int<_Tp>&);  // platform specific

template <class _Tp> inline _compound_int<_Tp> operator+(const _compound_int<_Tp>& val) {return val;}
template <class _Tp> inline _compound_int<_Tp> operator-(const _compound_int<_Tp>& val) {return 0 - val;}
template <class _Tp> inline bool operator!(const _compound_int<_Tp>& val) {return val==0;}

// Non-member arithmetic operations involving two _compound_int arguments
template <class _Tp>
inline _compound_int<_Tp> operator+(const _compound_int<_Tp>& lhs, const _compound_int<_Tp>& rhs)
{ _compound_int<_Tp> temp(lhs); return temp += rhs; }
template <class _Tp>
inline _compound_int<_Tp> operator-(const _compound_int<_Tp>& lhs, const _compound_int<_Tp>& rhs)
{ _compound_int<_Tp> temp(lhs); return temp -= rhs; }
template <class _Tp>
inline _compound_int<_Tp> operator*(const _compound_int<_Tp>& lhs, const _compound_int<_Tp>& rhs)
{ _compound_int<_Tp> temp(lhs); return temp *= rhs; }
template <class _Tp>
inline _compound_int<_Tp> operator/(const _compound_int<_Tp>& lhs, const _compound_int<_Tp>& rhs)
{ _compound_int<_Tp> temp(lhs); return temp /= rhs; }
template <class _Tp>
inline _compound_int<_Tp> operator%(const _compound_int<_Tp>& lhs, const _compound_int<_Tp>& rhs)
{ _compound_int<_Tp> temp(lhs); return temp %= rhs; }
template <class _Tp>
inline _compound_int<_Tp> operator&(const _compound_int<_Tp>& lhs, const _compound_int<_Tp>& rhs)
{ _compound_int<_Tp> temp(lhs); return temp &= rhs; }
template <class _Tp>
inline _compound_int<_Tp> operator|(const _compound_int<_Tp>& lhs, const _compound_int<_Tp>& rhs)
{ _compound_int<_Tp> temp(lhs); return temp |= rhs; }
template <class _Tp>
inline _compound_int<_Tp> operator^(const _compound_int<_Tp>& lhs, const _compound_int<_Tp>& rhs)
{ _compound_int<_Tp> temp(lhs); return temp ^= rhs; }

// Non-member arithmetic operations involving one built-in integer argument.
template <class _Tp>
inline _compound_int<_Tp> operator+(const _compound_int<_Tp>& lhs, unsigned long rhs) { return lhs + _compound_int<_Tp>(rhs); }
template <class _Tp>
inline _compound_int<_Tp> operator+(unsigned long lhs, const _compound_int<_Tp>& rhs) { return _compound_int<_Tp>(lhs) + rhs; }

template <class _Tp>
inline _compound_int<_Tp> operator-(const _compound_int<_Tp>& lhs, unsigned long rhs) { return lhs - _compound_int<_Tp>(rhs); }
template <class _Tp>
inline _compound_int<_Tp> operator-(unsigned long lhs, const _compound_int<_Tp>& rhs) { return _compound_int<_Tp>(lhs) - rhs; }

template <class _Tp>
inline _compound_int<_Tp> operator*(const _compound_int<_Tp>& lhs, unsigned long rhs) { return lhs * _compound_int<_Tp>(rhs); }
template <class _Tp>
inline _compound_int<_Tp> operator*(unsigned long lhs, const _compound_int<_Tp>& rhs) { return _compound_int<_Tp>(lhs) * rhs; }

template <class _Tp>
inline _compound_int<_Tp> operator/(const _compound_int<_Tp>& lhs, unsigned long rhs) { return lhs / _compound_int<_Tp>(rhs); }
template <class _Tp>
inline _compound_int<_Tp> operator/(unsigned long lhs, const _compound_int<_Tp>& rhs) { return _compound_int<_Tp>(lhs) / rhs; }

template <class _Tp>
inline _compound_int<_Tp> operator%(const _compound_int<_Tp>& lhs, unsigned long rhs) { return lhs % _compound_int<_Tp>(rhs); }
template <class _Tp>
inline _compound_int<_Tp> operator%(unsigned long lhs, const _compound_int<_Tp>& rhs) { return _compound_int<_Tp>(lhs) % rhs; }

template <class _Tp>
inline _compound_int<_Tp> operator&(const _compound_int<_Tp>& lhs, unsigned long rhs) { return lhs & _compound_int<_Tp>(rhs); }
template <class _Tp>
inline _compound_int<_Tp> operator&(unsigned long lhs, const _compound_int<_Tp>& rhs) { return _compound_int<_Tp>(lhs) & rhs; }

template <class _Tp>
inline _compound_int<_Tp> operator|(const _compound_int<_Tp>& lhs, unsigned long rhs) { return lhs | _compound_int<_Tp>(rhs); }
template <class _Tp>
inline _compound_int<_Tp> operator|(unsigned long lhs, const _compound_int<_Tp>& rhs) { return _compound_int<_Tp>(lhs) | rhs; }

template <class _Tp>
inline _compound_int<_Tp> operator^(const _compound_int<_Tp>& lhs, unsigned long rhs) { return lhs ^ _compound_int<_Tp>(rhs); }
template <class _Tp>
inline _compound_int<_Tp> operator^(unsigned long lhs, const _compound_int<_Tp>& rhs) { return _compound_int<_Tp>(lhs) ^ rhs; }

template <class _Tp>
inline _compound_int<_Tp> operator<<(const _compound_int<_Tp>& lhs, unsigned int rhs) { _compound_int<_Tp> temp(lhs); return temp <<= rhs; }
template <class _Tp>
inline _compound_int<_Tp> operator>>(const _compound_int<_Tp>& lhs, unsigned int rhs) { _compound_int<_Tp> temp(lhs); return temp >>= rhs; }

// platform specific specializations
#if defined (__MRC__) || defined (__SC__)

_STLP_END_NAMESPACE
#  include <Math64.h>
#  include <utility>
#  undef modff    //*TY 04/06/2000 - defined in <math.h> which conflicts with <fp.h> definition
#  include <fp.h>

_STLP_BEGIN_NAMESPACE

#  if TYPE_LONGLONG
typedef UInt64 uint64;
#    define ULL(x) (U64SetU(x))

#  else
//  Apple's mpw sc compiler for 68k macintosh does not support native 64bit integer type.
//  Instead, it comes with external support library and struct data type representing 64bit int:
//
//    struct UnsignedWide {
//      UInt32   hi;
//      UInt32   lo;
//    };

typedef _compound_int<UnsignedWide> uint64;
#    define ULL(x) uint64(x)
#    define ULL2(hi,lo) {hi,lo}

// Constructors, destructor, assignment operator.
_STLP_TEMPLATE_NULL inline _compound_int<UnsignedWide>::_compound_int() { hi = 0; lo = 0; }
_STLP_TEMPLATE_NULL inline _compound_int<UnsignedWide>::_compound_int(unsigned long val) { hi = 0; lo = val; }
_STLP_TEMPLATE_NULL inline _compound_int<UnsignedWide>::_compound_int(unsigned long h, unsigned long l) { hi = h; lo = l; }

// Arithmetic op= operations involving two _compound_int.
_STLP_TEMPLATE_NULL
inline _compound_int<UnsignedWide>& _compound_int<UnsignedWide>::operator+= (const _compound_int<UnsignedWide>& rhs)
{ *this = U64Add( *this, rhs ); return *this; }
_STLP_TEMPLATE_NULL
inline _compound_int<UnsignedWide>& _compound_int<UnsignedWide>::operator-= (const _compound_int<UnsignedWide>& rhs)
{ *this = U64Subtract( *this, rhs ); return *this; }
_STLP_TEMPLATE_NULL
inline _compound_int<UnsignedWide>& _compound_int<UnsignedWide>::operator*= (const _compound_int<UnsignedWide>& rhs)
{ *this = U64Multiply( *this, rhs ); return *this; }
_STLP_TEMPLATE_NULL
inline _compound_int<UnsignedWide>& _compound_int<UnsignedWide>::operator/= (const _compound_int<UnsignedWide>& rhs)
{ *this = U64Divide( *this, rhs, NULL ); return *this; }
_STLP_TEMPLATE_NULL
inline _compound_int<UnsignedWide>& _compound_int<UnsignedWide>::operator%= (const _compound_int<UnsignedWide>& rhs)
{ U64Divide( *this, rhs, this ); return *this; }
_STLP_TEMPLATE_NULL
inline _compound_int<UnsignedWide>& _compound_int<UnsignedWide>::operator&= (const _compound_int<UnsignedWide>& rhs)
{ *this = U64BitwiseAnd( *this, rhs ); return *this; }
_STLP_TEMPLATE_NULL
inline _compound_int<UnsignedWide>& _compound_int<UnsignedWide>::operator|= (const _compound_int<UnsignedWide>& rhs)
{ *this = U64BitwiseOr( *this, rhs ); return *this; }
_STLP_TEMPLATE_NULL
inline _compound_int<UnsignedWide>& _compound_int<UnsignedWide>::operator^= (const _compound_int<UnsignedWide>& rhs)
{ *this = U64BitwiseEor( *this, rhs ); return *this; }

// Arithmetic op= operations involving built-in integer.
_STLP_TEMPLATE_NULL
inline _compound_int<UnsignedWide>& _compound_int<UnsignedWide>::operator<<= (unsigned int rhs)
{ *this = U64ShiftLeft( *this, rhs ); return *this; }
_STLP_TEMPLATE_NULL
inline _compound_int<UnsignedWide>& _compound_int<UnsignedWide>::operator>>= (unsigned int rhs)
{ *this = U64ShiftRight( *this, rhs ); return *this; }

// Comparison operators.
_STLP_TEMPLATE_NULL
inline bool operator==(const _compound_int<UnsignedWide>& lhs, const _compound_int<UnsignedWide>& rhs)
{ return (lhs.hi == rhs.hi) && (lhs.lo == rhs.lo); }
_STLP_TEMPLATE_NULL
inline bool operator< (const _compound_int<UnsignedWide>& lhs, const _compound_int<UnsignedWide>& rhs)
{ return U64Compare( lhs, rhs ) < 0; }
_STLP_TEMPLATE_NULL
inline bool operator==(const _compound_int<UnsignedWide>& lhs, unsigned long rhs)
{ return (lhs.hi == 0) && (lhs.lo == rhs); }

// Unary non-member arithmetic operators.
_STLP_TEMPLATE_NULL
inline unsigned long to_ulong(const _compound_int<UnsignedWide>& val) { return val.lo; }
_STLP_TEMPLATE_NULL
inline _compound_int<UnsignedWide> operator~(const _compound_int<UnsignedWide>& val) { return U64BitwiseNot( val ); }
_STLP_TEMPLATE_NULL
inline bool operator!(const _compound_int<UnsignedWide>& val) { return !((val.hi == 0) && (val.lo == 0)); }

#  endif // TYPE_LONGLONG
#endif // __MRC__

#endif // _UINT64_H
