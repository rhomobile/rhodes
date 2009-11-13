/* boost limits_test.cpp   test your <limits> file for important
 *
 * Copyright Jens Maurer 2000
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without fee provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * Jens Maurer makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 */

#include <limits>

#include "cppunit/cppunit_proxy.h"

#if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#endif

//
// TestCase class
//
class LimitTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(LimitTest);
  CPPUNIT_TEST(test);
#  if defined (__BORLANDC__)
  CPPUNIT_IGNORE;
#  endif
  CPPUNIT_TEST(qnan_test);
  CPPUNIT_TEST_SUITE_END();

protected:
  void test();
  void qnan_test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(LimitTest);

#  define CHECK_COND(X) if (!(X)) return false;

bool valid_sign_info(bool, bool)
{ return true; }

template <class _Tp>
bool valid_sign_info(bool limit_is_signed, const _Tp &) {
  return limit_is_signed && _Tp(-1) < 0 ||
         !limit_is_signed && _Tp(-1) > 0;
}

template <class _Tp>
bool test_integral_limits(const _Tp &, bool unknown_sign = true, bool is_signed = true) {
  typedef numeric_limits<_Tp> lim;

  CHECK_COND(lim::is_specialized);
  CHECK_COND(lim::is_integer);
  /*CHECK_COND(lim::is_modulo);*/
  CHECK_COND(lim::min() < lim::max());
  CHECK_COND((unknown_sign && ((lim::is_signed && (lim::min() != 0)) || (!lim::is_signed && (lim::min() == 0)))) ||
             (!unknown_sign && ((lim::is_signed && is_signed) || (!lim::is_signed && !is_signed))));

  if (unknown_sign) {
    CHECK_COND(valid_sign_info(lim::is_signed, _Tp()));
  }
  return true;
}

template <class _Tp>
bool test_signed_integral_limits(const _Tp &__val) {
  return test_integral_limits(__val, false, true);
}
template <class _Tp>
bool test_unsigned_integral_limits(const _Tp &__val) {
  return test_integral_limits(__val, false, false);
}

template <class _Tp>
bool test_float_limits(const _Tp &) {
  typedef numeric_limits<_Tp> lim;
  CHECK_COND(lim::is_specialized);
  CHECK_COND(!lim::is_modulo);
  CHECK_COND(!lim::is_integer);
  CHECK_COND(lim::is_signed);

  CHECK_COND(lim::max() > 1000);
  CHECK_COND(lim::min() > 0);
  CHECK_COND(lim::min() < 0.001);
  CHECK_COND(lim::epsilon() > 0);

  if (lim::is_iec559) {
    CHECK_COND(lim::has_infinity);
    CHECK_COND(lim::has_quiet_NaN);
    CHECK_COND(lim::has_signaling_NaN);
  }

  if (lim::has_infinity) {
    const _Tp infinity = lim::infinity();
    /* Make sure those values are not 0 or similar nonsense.
    * Infinity must compare as if larger than the maximum representable value.
    */
    CHECK_COND(infinity > lim::max());
    CHECK_COND(-infinity < -lim::max());
  }
  return true;
}

template <class _Tp>
bool test_qnan(const _Tp &) {
  typedef numeric_limits<_Tp> lim;
  if (lim::has_quiet_NaN) {
    const _Tp qnan = lim::quiet_NaN();

    /* NaNs shall always compare "false" when compared for equality
    * If one of these fail, your compiler may be optimizing incorrectly,
    * or the STLport is incorrectly configured.
    */
    CHECK_COND(! (qnan == 42));
    CHECK_COND(! (qnan == qnan));
    CHECK_COND(qnan != 42);
    CHECK_COND(qnan != qnan);

    /* The following tests may cause arithmetic traps.
    * CHECK_COND(! (qnan < 42));
    * CHECK_COND(! (qnan > 42));
    * CHECK_COND(! (qnan <= 42));
    * CHECK_COND(! (qnan >= 42));
    */
  }
  return true;
}
void LimitTest::test() {
  CPPUNIT_ASSERT(test_integral_limits(bool()));
  CPPUNIT_ASSERT(test_integral_limits(char()));
  typedef signed char signed_char;
  CPPUNIT_ASSERT(test_signed_integral_limits(signed_char()));
  typedef unsigned char unsigned_char;
  CPPUNIT_ASSERT(test_unsigned_integral_limits(unsigned_char()));
#  if defined (_STLP_HAS_WCHAR_T) && !defined (_STLP_WCHAR_T_IS_USHORT)
  CPPUNIT_ASSERT(test_integral_limits(wchar_t()));
#  endif
  CPPUNIT_ASSERT(test_signed_integral_limits(short()));
  typedef unsigned short unsigned_short;
  CPPUNIT_ASSERT(test_unsigned_integral_limits(unsigned_short()));
  CPPUNIT_ASSERT(test_signed_integral_limits(int()));
  typedef unsigned int unsigned_int;
  CPPUNIT_ASSERT(test_unsigned_integral_limits(unsigned_int()));
  CPPUNIT_ASSERT(test_signed_integral_limits(long()));
  typedef unsigned long unsigned_long;
  CPPUNIT_ASSERT(test_unsigned_integral_limits(unsigned_long()));
#  if defined (_STLP_LONG_LONG)
  typedef _STLP_LONG_LONG long_long;
  CPPUNIT_ASSERT(test_signed_integral_limits(long_long()));
  typedef unsigned _STLP_LONG_LONG unsigned_long_long;
  CPPUNIT_ASSERT(test_unsigned_integral_limits(unsigned_long_long()));
#endif

  CPPUNIT_ASSERT(test_float_limits(float()));
  CPPUNIT_ASSERT(test_float_limits(double()));
#  if !defined ( _STLP_NO_LONG_DOUBLE )
  typedef long double long_double;
  CPPUNIT_ASSERT(test_float_limits(long_double()));
#  endif
}

void LimitTest::qnan_test() {
  CPPUNIT_ASSERT(test_qnan(float()));
  CPPUNIT_ASSERT(test_qnan(double()));
#  if !defined ( _STLP_NO_LONG_DOUBLE )
  typedef long double long_double;
  CPPUNIT_ASSERT(test_qnan(long_double()));
#  endif
}
