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

#include <limits>
#include <locale>
#include <istream>

#if defined (__GNUC__) && !defined (__sun) || \
    defined (__DMC__)
#  include <stdint.h>
#endif

#if defined (__linux__)
#  include <ieee754.h>

union _ll {
  uint64_t i64;
  struct {
#  if defined (_STLP_BIG_ENDIAN)
    uint32_t hi;
    uint32_t lo;
#  elif defined (_STLP_LITTLE_ENDIAN)
    uint32_t lo;
    uint32_t hi;
#  else
#    error Unknown endianess
#  endif
  } i32;
};
#endif

#if defined (N_PLAT_NLM)
#  include <nlm/nwintxx.h>

#  if defined (INT64)
typedef unsigned INT64 uint64_t;
#  else
// #error "Can't find INT64"
// 64-bit int really not defined in headers
// (_INTEGRAL_MAX_BITS < 64 in any case?), but compiler indeed know __int64
//        - ptr, 2005-05-06
typedef unsigned __int64 uint64_t;
#  endif

#  if defined (INT32)
typedef unsigned INT32 uint32_t;
#  else
#    error Can not find INT32
#  endif

union _ll {
  uint64_t i64;
  struct {
    uint32_t lo;
    uint32_t hi;
  } i32;
};
#endif

_STLP_BEGIN_NAMESPACE
_STLP_MOVE_TO_PRIV_NAMESPACE

//----------------------------------------------------------------------
// num_get

// Helper functions for _M_do_get_float.

#if !defined (_STLP_NO_WCHAR_T)
void  _STLP_CALL
_Initialize_get_float( const ctype<wchar_t>& ct,
                       wchar_t& Plus, wchar_t& Minus,
                       wchar_t& pow_e, wchar_t& pow_E,
                       wchar_t* digits) {
  char ndigits[11] = "0123456789";
  Plus  = ct.widen('+');
  Minus = ct.widen('-');
  pow_e = ct.widen('e');
  pow_E = ct.widen('E');
  ct.widen(ndigits + 0, ndigits + 10, digits);
}
#endif /* WCHAR_T */

/*
 * __string_to_double is just lifted from atof, the difference being
 * that we just use '.' for the decimal point, rather than let it
 * be taken from the current C locale, which of course is not accessible
 * to us.
 */
#if defined (_STLP_MSVC) || defined (__BORLANDC__) || defined (__ICL)
typedef unsigned long uint32;
typedef unsigned __int64 uint64;
#  define ULL(x) x##Ui64
#elif defined (__MRC__) || defined (__SC__)
typedef unsigned long uint32;
#  include "uint64.h"    //*TY 03/25/2000 - added 64bit math type definition
#elif defined (__unix) || defined (__MINGW32__) || defined (N_PLAT_NLM) || \
      (defined (__DMC__) && (__LONGLONG)) || defined(__PIPS__) || \
      defined(OS_ANDROID)
typedef uint32_t uint32;
typedef uint64_t uint64;
#  define ULL(x) x##ULL
#else
#  error There should be some unsigned 64-bit integer on the system!
#endif

// Multiplication of two 64-bit integers, giving a 128-bit result.
// Taken from Algorithm M in Knuth section 4.3.1, with the loop
// hand-unrolled.
static void _Stl_mult64(const uint64 u, const uint64 v,
                        uint64& high, uint64& low) {
  const uint64 low_mask = ULL(0xffffffff);
  const uint64 u0 = u & low_mask;
  const uint64 u1 = u >> 32;
  const uint64 v0 = v & low_mask;
  const uint64 v1 = v >> 32;

  uint64 t = u0 * v0;
  low = t & low_mask;

  t = u1 * v0 + (t >> 32);
  uint64 w1 = t & low_mask;
  uint64 w2 = t >> 32;

  uint64 x = u0 * v1 + w1;
  low += (x & low_mask) << 32;
  high = u1 * v1 + w2 + (x >> 32);
}

#define bit11 ULL(0x7ff)
#define exponent_mask (bit11 << 52)

#if !defined (__GNUC__) || (__GNUC__ != 3) || (__GNUC_MINOR__ != 4) || \
    (!defined (__CYGWIN__) && !defined (__MINGW32__))
//Generate bad code when compiled with -O2 option.
inline
#endif
void _Stl_set_exponent(uint64 &val, uint64 exp)
{ val = (val & ~exponent_mask) | ((exp & bit11) << 52); }

/* Power of ten fractions for tenscale*/
/* The constants are factored so that at most two constants
 * and two multiplies are needed. Furthermore, one of the constants
 * is represented exactly - 10**n where 1<= n <= 27.
 */

#if !defined (__SC__)    //*TY 03/25/2000 - no native 64bit integer under SCpp
static const uint64 _Stl_tenpow[80] = {
ULL(0xa000000000000000), /* _Stl_tenpow[0]=(10**1)/(2**4) */
ULL(0xc800000000000000), /* _Stl_tenpow[1]=(10**2)/(2**7) */
ULL(0xfa00000000000000), /* _Stl_tenpow[2]=(10**3)/(2**10) */
ULL(0x9c40000000000000), /* _Stl_tenpow[3]=(10**4)/(2**14) */
ULL(0xc350000000000000), /* _Stl_tenpow[4]=(10**5)/(2**17) */
ULL(0xf424000000000000), /* _Stl_tenpow[5]=(10**6)/(2**20) */
ULL(0x9896800000000000), /* _Stl_tenpow[6]=(10**7)/(2**24) */
ULL(0xbebc200000000000), /* _Stl_tenpow[7]=(10**8)/(2**27) */
ULL(0xee6b280000000000), /* _Stl_tenpow[8]=(10**9)/(2**30) */
ULL(0x9502f90000000000), /* _Stl_tenpow[9]=(10**10)/(2**34) */
ULL(0xba43b74000000000), /* _Stl_tenpow[10]=(10**11)/(2**37) */
ULL(0xe8d4a51000000000), /* _Stl_tenpow[11]=(10**12)/(2**40) */
ULL(0x9184e72a00000000), /* _Stl_tenpow[12]=(10**13)/(2**44) */
ULL(0xb5e620f480000000), /* _Stl_tenpow[13]=(10**14)/(2**47) */
ULL(0xe35fa931a0000000), /* _Stl_tenpow[14]=(10**15)/(2**50) */
ULL(0x8e1bc9bf04000000), /* _Stl_tenpow[15]=(10**16)/(2**54) */
ULL(0xb1a2bc2ec5000000), /* _Stl_tenpow[16]=(10**17)/(2**57) */
ULL(0xde0b6b3a76400000), /* _Stl_tenpow[17]=(10**18)/(2**60) */
ULL(0x8ac7230489e80000), /* _Stl_tenpow[18]=(10**19)/(2**64) */
ULL(0xad78ebc5ac620000), /* _Stl_tenpow[19]=(10**20)/(2**67) */
ULL(0xd8d726b7177a8000), /* _Stl_tenpow[20]=(10**21)/(2**70) */
ULL(0x878678326eac9000), /* _Stl_tenpow[21]=(10**22)/(2**74) */
ULL(0xa968163f0a57b400), /* _Stl_tenpow[22]=(10**23)/(2**77) */
ULL(0xd3c21bcecceda100), /* _Stl_tenpow[23]=(10**24)/(2**80) */
ULL(0x84595161401484a0), /* _Stl_tenpow[24]=(10**25)/(2**84) */
ULL(0xa56fa5b99019a5c8), /* _Stl_tenpow[25]=(10**26)/(2**87) */
ULL(0xcecb8f27f4200f3a), /* _Stl_tenpow[26]=(10**27)/(2**90) */

ULL(0xd0cf4b50cfe20766), /* _Stl_tenpow[27]=(10**55)/(2**183) */
ULL(0xd2d80db02aabd62c), /* _Stl_tenpow[28]=(10**83)/(2**276) */
ULL(0xd4e5e2cdc1d1ea96), /* _Stl_tenpow[29]=(10**111)/(2**369) */
ULL(0xd6f8d7509292d603), /* _Stl_tenpow[30]=(10**139)/(2**462) */
ULL(0xd910f7ff28069da4), /* _Stl_tenpow[31]=(10**167)/(2**555) */
ULL(0xdb2e51bfe9d0696a), /* _Stl_tenpow[32]=(10**195)/(2**648) */
ULL(0xdd50f1996b947519), /* _Stl_tenpow[33]=(10**223)/(2**741) */
ULL(0xdf78e4b2bd342cf7), /* _Stl_tenpow[34]=(10**251)/(2**834) */
ULL(0xe1a63853bbd26451), /* _Stl_tenpow[35]=(10**279)/(2**927) */
ULL(0xe3d8f9e563a198e5), /* _Stl_tenpow[36]=(10**307)/(2**1020) */

ULL(0xfd87b5f28300ca0e), /* _Stl_tenpow[37]=(10**-28)/(2**-93) */
ULL(0xfb158592be068d2f), /* _Stl_tenpow[38]=(10**-56)/(2**-186) */
ULL(0xf8a95fcf88747d94), /* _Stl_tenpow[39]=(10**-84)/(2**-279) */
ULL(0xf64335bcf065d37d), /* _Stl_tenpow[40]=(10**-112)/(2**-372) */
ULL(0xf3e2f893dec3f126), /* _Stl_tenpow[41]=(10**-140)/(2**-465) */
ULL(0xf18899b1bc3f8ca2), /* _Stl_tenpow[42]=(10**-168)/(2**-558) */
ULL(0xef340a98172aace5), /* _Stl_tenpow[43]=(10**-196)/(2**-651) */
ULL(0xece53cec4a314ebe), /* _Stl_tenpow[44]=(10**-224)/(2**-744) */
ULL(0xea9c227723ee8bcb), /* _Stl_tenpow[45]=(10**-252)/(2**-837)     */
ULL(0xe858ad248f5c22ca), /* _Stl_tenpow[46]=(10**-280)/(2**-930) */
ULL(0xe61acf033d1a45df), /* _Stl_tenpow[47]=(10**-308)/(2**-1023)    */
ULL(0xe3e27a444d8d98b8), /* _Stl_tenpow[48]=(10**-336)/(2**-1116) */
ULL(0xe1afa13afbd14d6e)  /* _Stl_tenpow[49]=(10**-364)/(2**-1209) */

#else    //*TY 03/20/2000 - added support for SCpp which lacks native 64bit integer type
static const UnsignedWide _Stl_tenpow[80] = {
ULL2(0xa0000000,0x00000000), /* _Stl_tenpow[0]=(10**1)/(2**4) */
ULL2(0xc8000000,0x00000000), /* _Stl_tenpow[1]=(10**2)/(2**7) */
ULL2(0xfa000000,0x00000000), /* _Stl_tenpow[2]=(10**3)/(2**10) */
ULL2(0x9c400000,0x00000000), /* _Stl_tenpow[3]=(10**4)/(2**14) */
ULL2(0xc3500000,0x00000000), /* _Stl_tenpow[4]=(10**5)/(2**17) */
ULL2(0xf4240000,0x00000000), /* _Stl_tenpow[5]=(10**6)/(2**20) */
ULL2(0x98968000,0x00000000), /* _Stl_tenpow[6]=(10**7)/(2**24) */
ULL2(0xbebc2000,0x00000000), /* _Stl_tenpow[7]=(10**8)/(2**27) */
ULL2(0xee6b2800,0x00000000), /* _Stl_tenpow[8]=(10**9)/(2**30) */
ULL2(0x9502f900,0x00000000), /* _Stl_tenpow[9]=(10**10)/(2**34) */
ULL2(0xba43b740,0x00000000), /* _Stl_tenpow[10]=(10**11)/(2**37) */
ULL2(0xe8d4a510,0x00000000), /* _Stl_tenpow[11]=(10**12)/(2**40) */
ULL2(0x9184e72a,0x00000000), /* _Stl_tenpow[12]=(10**13)/(2**44) */
ULL2(0xb5e620f4,0x80000000), /* _Stl_tenpow[13]=(10**14)/(2**47) */
ULL2(0xe35fa931,0xa0000000), /* _Stl_tenpow[14]=(10**15)/(2**50) */
ULL2(0x8e1bc9bf,0x04000000), /* _Stl_tenpow[15]=(10**16)/(2**54) */
ULL2(0xb1a2bc2e,0xc5000000), /* _Stl_tenpow[16]=(10**17)/(2**57) */
ULL2(0xde0b6b3a,0x76400000), /* _Stl_tenpow[17]=(10**18)/(2**60) */
ULL2(0x8ac72304,0x89e80000), /* _Stl_tenpow[18]=(10**19)/(2**64) */
ULL2(0xad78ebc5,0xac620000), /* _Stl_tenpow[19]=(10**20)/(2**67) */
ULL2(0xd8d726b7,0x177a8000), /* _Stl_tenpow[20]=(10**21)/(2**70) */
ULL2(0x87867832,0x6eac9000), /* _Stl_tenpow[21]=(10**22)/(2**74) */
ULL2(0xa968163f,0x0a57b400), /* _Stl_tenpow[22]=(10**23)/(2**77) */
ULL2(0xd3c21bce,0xcceda100), /* _Stl_tenpow[23]=(10**24)/(2**80) */
ULL2(0x84595161,0x401484a0), /* _Stl_tenpow[24]=(10**25)/(2**84) */
ULL2(0xa56fa5b9,0x9019a5c8), /* _Stl_tenpow[25]=(10**26)/(2**87) */
ULL2(0xcecb8f27,0xf4200f3a), /* _Stl_tenpow[26]=(10**27)/(2**90) */

ULL2(0xd0cf4b50,0xcfe20766), /* _Stl_tenpow[27]=(10**55)/(2**183) */
ULL2(0xd2d80db0,0x2aabd62c), /* _Stl_tenpow[28]=(10**83)/(2**276) */
ULL2(0xd4e5e2cd,0xc1d1ea96), /* _Stl_tenpow[29]=(10**111)/(2**369) */
ULL2(0xd6f8d750,0x9292d603), /* _Stl_tenpow[30]=(10**139)/(2**462) */
ULL2(0xd910f7ff,0x28069da4), /* _Stl_tenpow[31]=(10**167)/(2**555) */
ULL2(0xdb2e51bf,0xe9d0696a), /* _Stl_tenpow[32]=(10**195)/(2**648) */
ULL2(0xdd50f199,0x6b947519), /* _Stl_tenpow[33]=(10**223)/(2**741) */
ULL2(0xdf78e4b2,0xbd342cf7), /* _Stl_tenpow[34]=(10**251)/(2**834) */
ULL2(0xe1a63853,0xbbd26451), /* _Stl_tenpow[35]=(10**279)/(2**927) */
ULL2(0xe3d8f9e5,0x63a198e5), /* _Stl_tenpow[36]=(10**307)/(2**1020) */

ULL2(0xfd87b5f2,0x8300ca0e), /* _Stl_tenpow[37]=(10**-28)/(2**-93) */
ULL2(0xfb158592,0xbe068d2f), /* _Stl_tenpow[38]=(10**-56)/(2**-186) */
ULL2(0xf8a95fcf,0x88747d94), /* _Stl_tenpow[39]=(10**-84)/(2**-279) */
ULL2(0xf64335bc,0xf065d37d), /* _Stl_tenpow[40]=(10**-112)/(2**-372) */
ULL2(0xf3e2f893,0xdec3f126), /* _Stl_tenpow[41]=(10**-140)/(2**-465) */
ULL2(0xf18899b1,0xbc3f8ca2), /* _Stl_tenpow[42]=(10**-168)/(2**-558) */
ULL2(0xef340a98,0x172aace5), /* _Stl_tenpow[43]=(10**-196)/(2**-651) */
ULL2(0xece53cec,0x4a314ebe), /* _Stl_tenpow[44]=(10**-224)/(2**-744) */
ULL2(0xea9c2277,0x23ee8bcb), /* _Stl_tenpow[45]=(10**-252)/(2**-837)     */
ULL2(0xe858ad24,0x8f5c22ca), /* _Stl_tenpow[46]=(10**-280)/(2**-930) */
ULL2(0xe61acf03,0x3d1a45df), /* _Stl_tenpow[47]=(10**-308)/(2**-1023)    */
ULL2(0xe3e27a44,0x4d8d98b8), /* _Stl_tenpow[48]=(10**-336)/(2**-1116) */
ULL2(0xe1afa13a,0xfbd14d6e)  /* _Stl_tenpow[49]=(10**-364)/(2**-1209) */
#endif
};

static const short _Stl_twoexp[80] = {
4,7,10,14,17,20,24,27,30,34,37,40,44,47,50,54,57,60,64,67,70,74,77,80,84,87,90,
183,276,369,462,555,648,741,834,927,1020,
-93,-186,-279,-372,-465,-558,-651,-744,-837,-930,-1023,-1116,-1209
};

#define  TEN_1  0           /* offset to 10 **   1 */
#define  TEN_27   26        /* offset to 10 **  27 */
#define  TEN_M28  37        /* offset to 10 ** -28 */
#define  NUM_HI_P 11
#define  NUM_HI_N 13

#define _Stl_HIBITULL (ULL(1) << 63)

static void _Stl_norm_and_round(uint64& p, int& norm, uint64 prodhi, uint64 prodlo) {
  norm = 0;
  if ((prodhi & _Stl_HIBITULL) == 0) {
                                /* leading bit is a zero
                                 * may have to normalize
                                 */
    if ((prodhi == ~_Stl_HIBITULL) &&
        ((prodlo >> 62) == 0x3)) {  /* normalization followed by round
                                     * would cause carry to create
                                     * extra bit, so don't normalize
                                     */
      p = _Stl_HIBITULL;
      return;
    }
    p = (prodhi << 1) | (prodlo >> 63); /* normalize */
    norm = 1;
    prodlo <<= 1;
  }
  else {
    p = prodhi;
  }

  if ((prodlo & _Stl_HIBITULL) != 0) {     /* first guard bit a one */    //*TY 03/25/2000 - added explicit comparison to zero to avoid reliance to the implicit conversion from uint64 to bool
#if !defined (__SC__)                  //*TY 03/25/2000 -
    if (((p & 0x1) != 0) ||
        prodlo != _Stl_HIBITULL ) {    /* not borderline for round to even */
#else                                 //*TY 03/25/2000 - added workaround for SCpp compiler
    bool b1 = ((p & 0x1) != 0);
    if (b1 || prodlo != _Stl_HIBITULL) { //*TY 03/25/2000 - SCpp confuses on this particular original boolean expression
#endif                                    //*TY 03/25/2000 -
      /* round */
      ++p;
      if (p == 0)
        ++p;
    }
  }

  return;
}

// Convert a 64-bitb fraction * 10^exp to a 64-bit fraction * 2^bexp.
// p:    64-bit fraction
// exp:  base-10 exponent
// bexp: base-2 exponent (output parameter)
static void _Stl_tenscale(uint64& p, int exp, int& bexp) {
  uint64 prodhi, prodlo;        /* 128b product */
  int exp_hi, exp_lo;           /* exp = exp_hi*32 + exp_lo */
  int hi, lo, tlo, thi;         /* offsets in power of ten table */
  int norm;                     /* number of bits of normalization */
  int num_hi;                   /* number of high exponent powers */

  bexp = 0;
  if (exp > 0) {                 /* split exponent */
    exp_lo = exp;
    exp_hi = 0;
    if (exp_lo > 27) {
      exp_lo++;
      while (exp_lo > 27) {
        exp_hi++;
        exp_lo -= 28;
      }
    }
    tlo = TEN_1;
    thi = TEN_27;
    num_hi = NUM_HI_P;
  }
  else if (exp < 0) {
    exp_lo = exp;
    exp_hi = 0;
    while (exp_lo < 0) {
      exp_hi++;
      exp_lo += 28;
    }
    tlo = TEN_1;
    thi = TEN_M28;
    num_hi = NUM_HI_N;
  }
  else {                        /* no scaling needed */
    return;
  }
  while (exp_hi) {               /* scale */
    hi = (min) (exp_hi, num_hi);    /* only a few large powers of 10 */
    exp_hi -= hi;               /* could iterate in extreme case */
    hi += thi-1;
    _Stl_mult64(p, _Stl_tenpow[hi], prodhi, prodlo);
    _Stl_norm_and_round(p, norm, prodhi, prodlo);
    bexp += _Stl_twoexp[hi] - norm;
  }
  if (exp_lo) {
    lo = tlo + exp_lo -1;
    _Stl_mult64(p, _Stl_tenpow[lo], prodhi, prodlo);
    _Stl_norm_and_round(p, norm, prodhi, prodlo);
    bexp += _Stl_twoexp[lo] - norm;
  }

  return;
}

// First argument is a buffer of values from 0 to 9, NOT ascii.
// Second argument is number of digits in buffer, 1 <= digits <= 17.
// Third argument is base-10 exponent.

#if defined (__SC__) || defined (__MRC__)

//*TY 04/06/2000 - powermac's 68K emulator utilizes apple's SANE floating point, which is not compatible with IEEE format.
_STLP_MOVE_TO_STD_NAMESPACE
_STLP_END_NAMESPACE

#  include <fp.h>

_STLP_BEGIN_NAMESPACE
_STLP_MOVE_TO_PRIV_NAMESPACE

static inline double _Stl_atod(char *buffer, int ndigit, int dexp) {
  decimal d;  // ref. inside macintosh powerpc numerics p.9-13

  d.sgn = 0;
  d.exp = dexp;
  d.sig.length = ndigit;
  for (int i = 0; i < ndigit; ++i) {
    d.sig.text[i] = buffer[i] + '0';
  }
  return dec2num(&d);
}

#else  /* IEEE representation */

#  if !defined (__linux__)
static double _Stl_atod(char *buffer, int ndigit, int dexp) {
  uint64 value;         /* Value develops as follows:
                                 * 1) decimal digits as an integer
                                 * 2) left adjusted fraction
                                 * 3) right adjusted fraction
                                 * 4) exponent and fraction
                                 */

  uint32 guard;         /* First guard bit */
  uint64 rest;          /* Remaining guard bits */

  int bexp;             /* binary exponent */
  int nzero;            /* number of non-zero bits */
  int sexp;             /* scaling exponent */

  char *bufferend;              /* pointer to char after last digit */

  /* Check for zero and treat it as a special case */
  if (buffer == 0){
    return 0.0;
  }

  /* Convert the decimal digits to a binary integer. */

  bufferend = buffer + ndigit;
  value = 0;

  while (buffer < bufferend) {
    value *= 10;
    value += *buffer++;
  }

  /* Check for zero and treat it as a special case */
  if (value == 0) {
    return 0.0;
  }

  /* Normalize value */
  bexp = 64;                    /* convert from 64b int to fraction */

  /* Count number of non-zeroes in value */
  nzero = 0;
  if ((value >> 32) != 0) { nzero  = 32; }    //*TY 03/25/2000 - added explicit comparison to zero to avoid uint64 to bool conversion operator
  if ((value >> (16 + nzero)) != 0) { nzero += 16; }
  if ((value >> ( 8 + nzero)) != 0) { nzero +=  8; }
  if ((value >> ( 4 + nzero)) != 0) { nzero +=  4; }
  if ((value >> ( 2 + nzero)) != 0) { nzero +=  2; }
  if ((value >> ( 1 + nzero)) != 0) { nzero +=  1; }
  if ((value >> (     nzero)) != 0) { nzero +=  1; }

  /* Normalize */
  value <<= /*(uint64)*/ (64 - nzero);    //*TY 03/25/2000 - removed extraneous cast to uint64
  bexp -= 64 - nzero;

  /* At this point we have a 64b fraction and a binary exponent
   * but have yet to incorporate the decimal exponent.
   */

  /* multiply by 10^dexp */
  _Stl_tenscale(value, dexp, sexp);
  bexp += sexp;

  if (bexp <= -1022) {          /* HI denorm or underflow */
    bexp += 1022;
    if (bexp < -53) {          /* guaranteed underflow */
      value = 0;
    }
    else {                      /* denorm or possible underflow */
      int lead0 = 12 - bexp;          /* 12 sign and exponent bits */

      /* we must special case right shifts of more than 63 */
      if (lead0 > 64) {
        rest = value;
        guard = 0;
        value = 0;
      }
      else if (lead0 == 64) {
        rest = value & ((ULL(1)<< 63)-1);
#if !defined(__SC__)
        guard = (uint32) ((value>> 63) & 1 );
#else
        guard = to_ulong((value>> 63) & 1 );   //*TY 03/25/2000 - use member function instead of problematic conversion operator utilization
#endif
        value = 0;
      }
      else {
        rest = value & (((ULL(1) << lead0)-1)-1);
#if !defined(__SC__)
        guard = (uint32) (((value>> lead0)-1) & 1);
#else     //*TY 03/25/2000 -
        guard = to_ulong(((value>> lead0)-1) & 1);
#endif    //*TY 03/25/2000 -
        value >>= /*(uint64)*/ lead0; /* exponent is zero */
      }

      /* Round */
      if (guard && ((value & 1) || rest) ) {
        ++value;
        if (value == (ULL(1) << 52)) { /* carry created normal number */
          value = 0;
          _Stl_set_exponent(value, 1);
        }
      }
    }
  }
  else {                        /* not zero or denorm */
    /* Round to 53 bits */
    rest = value & (1<<10)-1;
    value >>= 10;
#if !defined(__SC__)
    guard = (uint32) value & 1;
#else    //*TY 03/25/2000 -
    guard = to_ulong(value & 1);
#endif
    value >>= 1;

    /*  value&1 guard   rest    Action
     *
     *  dc      0       dc      none
     *  1       1       dc      round
     *  0       1       0       none
     *  0       1       !=0     round
     */
    if (guard) {
      if (((value&1)!=0) || (rest!=0)) {
        ++value;                        /* round */
        if ((value >> 53) != 0) {       /* carry all the way across */
          value >>= 1;          /* renormalize */
          ++bexp;
        }
      }
    }
    /*
     * Check for overflow
     * IEEE Double Precision Format
     * (From Table 7-8 of Kane and Heinrich)
     *
     * Fraction bits               52
     * Emax                     +1023
     * Emin                     -1022
     * Exponent bias            +1023
     * Exponent bits               11
     * Integer bit             hidden
     * Total width in bits         64
     */

    if (bexp > 1024) {          /* overflow */
      return numeric_limits<double>::infinity();
    }
    else {                      /* value is normal */
      value &= ~(ULL(1) << 52);   /* hide hidden bit */
      _Stl_set_exponent(value, bexp + 1022); /* add bias */
    }
  }

  _STLP_STATIC_ASSERT(sizeof(value) == sizeof(double))
  return *((double *) &value);
}

#  else // __linux__

static double _Stl_atod(char *buffer, int ndigit, int dexp) {
  ieee754_double v;

  char *bufferend;              /* pointer to char after last digit */

  /* Check for zero and treat it as a special case */

  if (buffer == 0) {
    return 0.0;
  }

  /* Convert the decimal digits to a binary integer. */

  bufferend = buffer + ndigit;
  _ll vv;
  vv.i64 = 0L;

  while (buffer < bufferend) {
    vv.i64 *= 10;
    vv.i64 += *buffer++;
  }

  /* Check for zero and treat it as a special case */
  if (vv.i64 == 0){
    return 0.0;
  }

  /* Normalize value */
  int bexp = 64;                    /* convert from 64b int to fraction */

  /* Count number of non-zeroes in value */
  int nzero = 0;
  if ((vv.i64 >> 32) !=0 ) { nzero  = 32; }    //*TY 03/25/2000 - added explicit comparison to zero to avoid uint64 to bool conversion operator
  if ((vv.i64 >> (16 + nzero)) != 0) { nzero += 16; }
  if ((vv.i64 >> ( 8 + nzero)) != 0) { nzero +=  8; }
  if ((vv.i64 >> ( 4 + nzero)) != 0) { nzero +=  4; }
  if ((vv.i64 >> ( 2 + nzero)) != 0) { nzero +=  2; }
  if ((vv.i64 >> ( 1 + nzero)) != 0) { nzero +=  1; }
  if ((vv.i64 >> (     nzero)) != 0) { nzero +=  1; }

  /* Normalize */
  nzero = 64 - nzero;
  vv.i64 <<= nzero;    //*TY 03/25/2000 - removed extraneous cast to uint64
  bexp -= nzero;

  /* At this point we have a 64b fraction and a binary exponent
   * but have yet to incorporate the decimal exponent.
   */

  /* multiply by 10^dexp */
  int sexp;
  _Stl_tenscale(vv.i64, dexp, sexp);
  bexp += sexp;

  if (bexp <= -1022) {          /* HI denorm or underflow */
    bexp += 1022;
    if (bexp < -53) {           /* guaranteed underflow */
      vv.i64 = 0;
    }
    else {                      /* denorm or possible underflow */
      int lead0;
      uint64_t rest;
      uint32_t guard;

      lead0 = 12-bexp;          /* 12 sign and exponent bits */

      /* we must special case right shifts of more than 63 */
      if (lead0 > 64) {
        rest = vv.i64;
        guard = 0;
        vv.i64 = 0;
      }
      else if (lead0 == 64) {
        rest = vv.i64 & ((ULL(1) << 63)-1);
#if !defined(__SC__)
        guard = (uint32) ((vv.i64 >> 63) & 1 );
#else
        guard = to_ulong((vv.i64 >> 63) & 1 );   //*TY 03/25/2000 - use member function instead of problematic conversion operator utilization
#endif
        vv.i64 = 0;
      }
      else {
        rest = vv.i64 & (((ULL(1) << lead0)-1)-1);
#if !defined(__SC__)
        guard = (uint32) (((vv.i64 >> lead0)-1) & 1);
#else     //*TY 03/25/2000 -
        guard = to_ulong(((vv.i64 >> lead0)-1) & 1);
#endif    //*TY 03/25/2000 -
        vv.i64 >>= /*(uint64)*/ lead0; /* exponent is zero */
      }

      /* Round */
      if (guard && ( (vv.i64 & 1) || rest)) {
        vv.i64++;
        if (vv.i64 == (ULL(1) << 52)) { /* carry created normal number */
          v.ieee.mantissa0 = 0;
          v.ieee.mantissa1 = 0;
          v.ieee.negative = 0;
          v.ieee.exponent = 1;
          return v.d;
        }
      }
    }
  }
  else {                        /* not zero or denorm */
    /* Round to 53 bits */
    uint64_t rest = vv.i64 & (1<<10)-1;
    vv.i64 >>= 10;
#if !defined(__SC__)
    uint32_t guard = (uint32) vv.i64 & 1;
#else    //*TY 03/25/2000 -
    uint32_t guard = to_ulong(vv.i64 & 1);
#endif
    vv.i64 >>= 1;

    /*  value&1 guard   rest    Action
     *
     *  dc      0       dc      none
     *  1       1       dc      round
     *  0       1       0       none
     *  0       1       !=0     round
     */
    if (guard) {
      if (((vv.i64&1)!=0) || (rest!=0)) {
        vv.i64++;                        /* round */
        if ((vv.i64>>53)!=0) {         /* carry all the way across */
          vv.i64 >>= 1;          /* renormalize */
          ++bexp;
        }
      }
    }
    /*
     * Check for overflow
     * IEEE Double Precision Format
     * (From Table 7-8 of Kane and Heinrich)
     *
     * Fraction bits               52
     * Emax                     +1023
     * Emin                     -1022
     * Exponent bias            +1023
     * Exponent bits               11
     * Integer bit             hidden
     * Total width in bits         64
     */

    if (bexp > 1024) {          /* overflow */
      return numeric_limits<double>::infinity();
    }
    else {                      /* value is normal */
      vv.i64 &= ~(ULL(1) << 52);   /* hide hidden bit */
      v.ieee.mantissa0 = vv.i32.hi;
      v.ieee.mantissa1 = vv.i32.lo;
      v.ieee.negative = 0;
      v.ieee.exponent = bexp + 1022;
      return v.d;
    }
  }

  v.ieee.mantissa0 = vv.i32.hi;
  v.ieee.mantissa1 = vv.i32.lo;
  v.ieee.negative = 0;
  v.ieee.exponent = 0;

  return v.d;
}
#  endif // __linux__

#endif

static double _Stl_string_to_double(const char *s) {
  const int max_digits = 17;
  unsigned c;
  unsigned Negate, decimal_point;
  char *d;
  int exp;
  double x;
  int dpchar;
  char digits[max_digits];

  // Skip leading whitespace, if any.
  const ctype<char>& ct = use_facet<ctype<char> >(locale::classic());
  while (c = *s++, ct.is(ctype_base::space, char(c))) {}

  /* process sign */
  Negate = 0;
  if (c == '+') {
    c = *s++;
  }
  else if (c == '-') {
    Negate = 1;
    c = *s++;
  }
  d = digits;
  dpchar = '.' - '0';
  decimal_point = 0;
  exp = 0;
  for (;;) {
    c -= '0';
    if (c < 10) {
      if (d == digits + max_digits) {
        /* ignore more than 17 digits, but adjust exponent */
        exp += (decimal_point ^ 1);
      }
      else {
        if (c == 0 && d == digits) {
          /* ignore leading zeros */
        }
        else {
          *d++ = (char) c;
        }
        exp -= decimal_point;
      }
    }
    else if (c == (unsigned int) dpchar && !decimal_point) {    /* INTERNATIONAL */
      decimal_point = 1;
    }
    else {
      break;
    }
    c = *s++;
  }
  /* strtod cant return until it finds the end of the exponent */
  if (d == digits) {
    return 0.0;
  }

  if (c == 'e'-'0' || c == 'E'-'0') {
    register unsigned negate_exp = 0;
    register int e = 0;
    c = *s++;
    if (c == '+' || c == ' ') {
      c = *s++;
    }
    else if (c == '-') {
      negate_exp = 1;
      c = *s++;
    }
    if (c -= '0', c < 10) {
      do {
        if (e <= 340)
          e = e * 10 + (int)c;
        else break;
        c = *s++;
      }
      while (c -= '0', c < 10);
      if (negate_exp) {
        e = -e;
      }
      if (e < -340 || e > 340)
        exp = e;
      else
        exp += e;
    }
  }

  if (exp < -340) {
    x = 0;
  }
  else if (exp > 308) {
    x = numeric_limits<double>::infinity();
  }
  else {
    /* let _Stl_atod diagnose under- and over-flows */
    /* if the input was == 0.0, we have already returned,
       so retval of +-Inf signals OVERFLOW, 0.0 UNDERFLOW
    */
    x = _Stl_atod(digits, (int)(d - digits), exp);
  }
  if (Negate) {
    x = -x;
  }
  return x;
}


#if !defined (_STLP_NO_LONG_DOUBLE)
/*
 * __string_to_long_double is just lifted from atold, the difference being
 * that we just use '.' for the decimal point, rather than let it
 * be taken from the current C locale, which of course is not accessible
 * to us.
 */

static long double
_Stl_string_to_long_double(const char * s) {
  const int max_digits = 34;
  register unsigned c;
  register unsigned Negate, decimal_point;
  register char *d;
  register int exp;
  long double x;
  register int dpchar;
  char digits[max_digits];

  const ctype<char>& ct = use_facet<ctype<char> >(locale::classic());
  while (c = *s++, ct.is(ctype_base::space, char(c)))
    ;

  /* process sign */
  Negate = 0;
  if (c == '+') {
    c = *s++;
  }
  else if (c == '-') {
    Negate = 1;
    c = *s++;
  }

  d = digits;
  dpchar = '.' - '0';
  decimal_point = 0;
  exp = 0;

  for (;;) {
    c -= '0';
    if (c < 10) {
      if (d == digits+max_digits) {
        /* ignore more than 34 digits, but adjust exponent */
        exp += (decimal_point ^ 1);
      }
      else {
        if (c == 0 && d == digits) {
          /* ignore leading zeros */
          ;
        }
        else {
          *d++ = (char)c;
        }
        exp -= decimal_point;
      }
    }
    else if ((char)c == dpchar && !decimal_point) {    /* INTERNATIONAL */
      decimal_point = 1;
    }
    else {
      break;
    }
    c = *s++;
  } /* for */

  if (d == digits) {
    return 0.0L;
  }
  if (c == 'e'-'0' || c == 'E'-'0') {
    register unsigned negate_exp = 0;
    register int e = 0;
    c = *s++;
    if (c == '+' || c == ' ') {
      c = *s++;
    }
    else if (c == '-') {
      negate_exp = 1;
      c = *s++;
    }
    if (c -= '0', c < 10) {
      do {
        if (e <= 340)
          e = e * 10 + c;
        else break;
        c = *s++;
      }
      while (c -= '0', c < 10);
      if (negate_exp) {
        e = -e;
      }
      if (e < -(323+max_digits) || e > 308)
        exp = e;
      else
        exp += e;
    }
  }

  if (exp < -(324+max_digits)) {
    x = 0;
  }
  else if (exp > 308) {
    x =  numeric_limits<long double>::infinity();
  }
  else {
    /* let _Stl_atod diagnose under- and over-flows */
    /* if the input was == 0.0, we have already returned,
           so retval of +-Inf signals OVERFLOW, 0.0 UNDERFLOW
        */

    //    x = _Stl_atod (digits, (int)(d - digits), exp); // TEMPORARY!!:1
    double tmp = _Stl_atod (digits, (int)(d - digits), exp); // TEMPORARY!!:1
    x = tmp == numeric_limits<double>::infinity()
      ? numeric_limits<long double>::infinity()
      : tmp;
  }

  if (Negate) {
    x = -x;
  }

  return x;
}
#endif

void _STLP_CALL
__string_to_float(const __iostring& v, float& val)
{ val = (float)_Stl_string_to_double(v.c_str()); }

void _STLP_CALL
__string_to_float(const __iostring& v, double& val)
{ val = _Stl_string_to_double(v.c_str()); }

#if !defined (_STLP_NO_LONG_DOUBLE)
void _STLP_CALL
__string_to_float(const __iostring& v, long double& val)
{ val = _Stl_string_to_long_double(v.c_str()); }
#endif

_STLP_MOVE_TO_STD_NAMESPACE
_STLP_END_NAMESPACE

// Local Variables:
// mode:C++
// End:
