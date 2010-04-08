///////////////////////////////////////////////////////////////////////////////
//
// Include\comtypes.h
//
// Primary common definitions for SPIRIT Library
//
// Copyright (C) 2008, SPIRIT
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __SPIRIT_INCLUDE_COMTYPES_H
#define __SPIRIT_INCLUDE_COMTYPES_H

///////////////////////////////////////////////////////////////////////////////
// CPU type detection
#include "common/spirit_cputype.h"

///////////////////////////////////////////////////////////////////////////////
// OS type detection
#include "common/spirit_ostype.h"

///////////////////////////////////////////////////////////////////////////////
// Compiler detection
#include "common/spirit_compiler.h"

///////////////////////////////////////////////////////////////////////////////
// Result codes
#include "common/spirit_result.h"

///////////////////////////////////////////////////////////////////////////////
// define SPIRIT_CPLUSPLUS

#ifndef EXTERN_C
#	ifdef __cplusplus
#		define EXTERN_C extern "C"
#	else
#		define EXTERN_C extern
#	endif // __cplusplus
#endif // EXTERN_C

#ifdef __cplusplus
#	define SPIRIT_CPLUSPLUS
#	define SPIRIT_EXTERN_C extern "C"
#	define SPIRIT_EXTERN_C_BEGIN extern "C" {
#	define SPIRIT_EXTERN_C_END }
#else
#	define SPIRIT_EXTERN_C extern
#	define SPIRIT_EXTERN_C_BEGIN
#	define SPIRIT_EXTERN_C_END
#endif // __cplusplus

#ifdef __cplusplus
#	define SPIRIT_INLINE inline
#	define SPIRIT_STATIC_INLINE static inline
#else
#	ifdef SPIRIT_COMPILER_GNUC
#		define SPIRIT_INLINE static __inline
#		define SPIRIT_STATIC_INLINE SPIRIT_INLINE
#	else
#		define SPIRIT_INLINE __inline
#		define SPIRIT_STATIC_INLINE static __inline
#	endif // SPIRIT_COMPILER_GNUC
#endif // __cplusplus

#ifndef inline_
#define inline_ __inline
#endif

///////////////////////////////////////////////////////////////////////////////
// Export/Import definitions

#ifdef SPIRIT_OS_WIN32

#define SPIRIT_EXPORT_SYMBOL   __declspec(dllexport)
#define SPIRIT_IMPORT_SYMBOL   __declspec(dllimport)

#define SPIRIT_FASTCALL  __fastcall
#define SPIRIT_CDECL     __cdecl
#define SPIRIT_STDCALL   __stdcall

#define SPIRIT_METHOD_CALL_TYPE  __cdecl

#else

#define SPIRIT_EXPORT_SYMBOL
#define SPIRIT_IMPORT_SYMBOL

#define SPIRIT_FASTCALL
#define SPIRIT_CDECL
#define SPIRIT_STDCALL

#define SPIRIT_METHOD_CALL_TYPE

#endif // SPIRIT_OS_WIN32

///////////////////////////////////////////////////////////////////////////////
// Common types

#define ALIGN8
#define ALIGN32

#if defined(__BORLANDC__)
  typedef unsigned short int  uint16;
  typedef unsigned long  int uint32;
  typedef unsigned long long uint64;
  typedef short int int16;
  typedef long  int int32;
  typedef long long int64;
  typedef float float32;
  typedef double float64;
  typedef unsigned char tBool;

#elif defined(_MSC_VER)
  #define DEF_INT_SIZE 32
  typedef unsigned __int16 uint16;
  typedef unsigned __int32 uint32;
  typedef unsigned __int64 uint64;
  typedef __int16 int16;
  typedef __int32 int32;
  typedef __int64 int64;
  typedef float float32;
  typedef double float64;
  typedef unsigned char tBool;
  #define false 0
  #define true 1
  typedef struct
  {
    float64 re, im;
  } tComplex;
  #if defined(__INTEL_COMPILER) || (_MSC_VER == 1400)
    #undef  ALIGN8
    #undef  ALIGN32
    #define ALIGN8   __declspec(align(8))
    #define ALIGN32  __declspec(align(32))
  #else
    //#error Aligin not tested!!
  #endif

#elif ((defined(__GNUC__) || defined(__DJGPP__)) && !defined(TEAK))
  #define DEF_INT_SIZE 32
  typedef long long int64;
  typedef unsigned long long uint64;
  typedef unsigned long uint32;
  typedef unsigned short uint16;
  typedef long int32;
  typedef short int16;
  typedef unsigned char tBool;
  #define false 0
  #define true 1
 // typedef longlong int64;
  #undef  ALIGN8
  #undef  ALIGN32
  #define ALIGN8   __attribute__ ((aligned(8)))
  #define ALIGN32  __attribute__ ((aligned(32)))

#elif defined(_TMS320C6X)
  #define DEF_INT_SIZE 32

  typedef unsigned long long int uint64;
  typedef long long int int64;

  typedef unsigned int uint32;
  typedef unsigned short uint16;
  typedef int int32;
  typedef short int16;
  typedef float float32;
  typedef double float64;
  typedef unsigned char tBool;
  #define false 0
  #define true 1

#elif defined(_TMS320C5XX)
  typedef float float32;
  typedef unsigned long uint32;
  typedef long int32;
  typedef unsigned int  uint16;
  typedef int int16;

#elif defined(__TMS320C55X__)
  typedef float float32;
  typedef unsigned long uint32;
  typedef long int32;
  typedef unsigned int  uint16;
  typedef int int16;

#elif defined(__TCS__)
  #define DEF_INT_SIZE 32
  typedef unsigned long uint32;
  typedef unsigned short uint16;
  typedef long int32;
  typedef short int16;
  typedef float float32;
  typedef double float64;
  typedef unsigned char tBool;
  #define false 0
  #define true 1

#elif defined(__arm)
  #define DEF_INT_SIZE 32
  typedef long long int64;
  typedef unsigned long long uint64;
  typedef unsigned long uint32;
  typedef unsigned short uint16;
  typedef signed long int32;
  typedef signed short int16;
  typedef float float32;
  typedef double float64;
  typedef unsigned char tBool;

  typedef struct sDIV2{
    int32 res1;
    int32 res2;
  }sDIV2;
  #define false 0
  #define true 1

  #undef  ALIGN8
  #undef  ALIGN32
  #define ALIGN8   __align(8)
  #define ALIGN32  __align(32)

#elif defined(TEAK)
  #define DEF_INT_SIZE 16
  typedef unsigned long uint32;
  typedef unsigned short uint16;
  typedef long int32;
  typedef short int16;

#elif defined(_SC100_)
  #define DEF_INT_SIZE 32
  typedef long long int64;
  typedef unsigned long long uint64;
  typedef unsigned long uint32;
  typedef unsigned short uint16;
  typedef signed long int32;
  typedef signed short int16;
  typedef float float32;
  typedef double float64;
  typedef unsigned char tBool;

#else

  typedef enum {
    false, true, z
  } bool_enum;
  typedef unsigned char tBool;

#endif //__BORLANDC__

  /* Common types */
  typedef signed char int8;
  typedef unsigned char uint8;

typedef /*ALIGN32*/ struct
{
  int16 re, im;
} tComplex16;

typedef /*ALIGN32*/ struct
{
  int32 re, im;
} tComplex32;

#ifndef INT8_MIN
#define INT8_MIN   (-128)
#endif
#ifndef INT8_MAX
#define INT8_MAX   127
#endif
#ifndef UINT8_MAX
#define UINT8_MAX  255
#endif
#ifndef INT16_MIN
#define INT16_MIN   (-32768)
#endif
#ifndef INT16_MAX
#define INT16_MAX   32767
#endif
#ifndef UINT16_MAX
#define UINT16_MAX  65535
#endif
#ifndef INT32_MIN
#define INT32_MIN   (-2147483648)
#endif
#ifndef INT32_MAX
#define INT32_MAX   2147483647
#endif
#ifndef UINT32_MAX
#define UINT32_MAX  4294967295
#endif
#ifndef FLOAT32_MAX
#define FLOAT32_MAX   3.402823466e+38F
#endif
#ifndef FLOAT64_MAX
#define FLOAT64_MAX   1.7976931348623158e+308
#endif

///////////////////////////////////////////////////////////////////////////////
// Spirit common types

typedef char              tChar;

#ifndef SPIRIT_OS_MACOSX
typedef unsigned char   tPureCBool;   // MSVC 6 using 1 byte to represent bool
#else
#  ifdef SPIRIT_ARCH_X86
typedef char          tPureCBool;     // MacOS X/i386 version of GCC using 1 byte to represent bool
#  else
typedef int           tPureCBool;     // MacOS X/ppc version of GCC using 4 bytes to represent bool
#  endif
#endif // !SPIRIT_OS_MACOSX

// A very handy macro, which would check the expression at compile time
// if expression is 0, then it would generate compile-time error
#define CT_ASSERT(exp) extern char __ct_assert[(exp)?1:-1];


// this would check for sizeof(tBool) is equal for both C and C++ types.
// if sizeof is different, compiler would complain because of negative array size
// DO NOT DELETE this compile-time checking!
#ifdef __cplusplus
//extern char testBoolSize[ (sizeof(tPureCBool) == sizeof(tBool)) ? 1 : -1 ];
CT_ASSERT(sizeof(tPureCBool) == sizeof(tBool))
#endif // __cplusplus

typedef const char*     tConstString;
//typedef uint32          tSourceId;

//#ifndef min
//  #define min(a,b)            (((a) < (b)) ? (a) : (b))
//#endif
//
//#ifndef max
//  #define max(a,b)            (((a) > (b)) ? (a) : (b))
//#endif

#ifdef SPIRIT_OS_LINUX
  #include <unistd.h>
  #include <linux/param.h>

  #define _MS2CLOCK( ms )     ( ms * HZ / 1000 )
  #define _CLOCK2MS( clk )    ( clk * 1000 / HZ )
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace macros

// armcc (ADS1.2) not support namespases
#if !(defined(SPIRIT_COMPILER_ARMCC) && (__ARMCC_VERSION < 200000))
#	define SPIRIT_USING_NAMESPACE
#endif

#if defined(SPIRIT_CPLUSPLUS) && defined(SPIRIT_USING_NAMESPACE)
#define SPIRIT_NAMESPACE_BEGIN( name ) namespace name {
#define SPIRIT_NAMESPACE_END( name ) }
#define SPIRIT_NAMESPACE_ID( name, id ) name::id
#define SPIRIT_NAMESPACE_USEALL( name ) using namespace name;
#define SPIRIT_NAMESPACE_USE( name, id ) using name::id;

#else
#define SPIRIT_NAMESPACE_BEGIN( name )
#define SPIRIT_NAMESPACE_END( name )
#define SPIRIT_NAMESPACE_ID( name, id ) id
#define SPIRIT_NAMESPACE_USEALL( name )
#define SPIRIT_NAMESPACE_USE( name, id )

#endif // SPIRIT_CPLUSPLUS

///////////////////////////////////////////////////////////////////////////////
// Common includes
#ifdef SPIRIT_OS_LINUX
#include <string.h> // memset etc.
typedef char TCHAR;
#define _tmain main
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#endif

///////////////////////////////////////////////////////////////////////////////
#if defined SPIRIT_OS_WIN32 || defined SPIRIT_OS_WIN32_CE ||defined (SPIRIT_OS_LINUX) || defined(SPIRIT_OS_ADS) || defined(SPIRIT_OS_WASSABI)

typedef struct
{
  uint16 Year;
  uint16 Month;
  uint16 DayOfWeek;
  uint16 Day;
  uint16 Hour;
  uint16 Minute;
  uint16 Second;
  uint16 Milliseconds;
} tTimeInfo;

#endif

///////////////////////////////////////////////////////////////////////////////
// system handle (platform/os-dependent)

#ifndef tSystemHandle_DEFINED
#define tSystemHandle_DEFINED
typedef void* tSystemHandle;
#endif // tSystemHandle_DEFINED

///////////////////////////////////////////////////////////////////////////////
// varian type
typedef union
{
  int       i;
  unsigned  u;
  uint32    u32;
  int32     i32;
  uint16    u16;
  int16     i16;
  uint8     u8;
  int8      i8;
  void     *pData;
  void    (*pFunc)();
} tVariant;

///////////////////////////////////////////////////////////////////////////////

#ifndef OUT
#define OUT
#endif // OUT

#ifndef IN
#define IN
#endif // IN

#include <stddef.h>

///////////////////////////////////////////////////////////////////////////////

#endif // __SPIRIT_INCLUDE_COMTYPES_H
