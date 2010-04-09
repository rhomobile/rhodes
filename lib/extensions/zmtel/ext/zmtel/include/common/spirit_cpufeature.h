///////////////////////////////////////////////////////////////////////////////
//
// Include/cpufeature.h
//
// Primary common definitions for SPIRIT Library
//
// Copyright (C) 2005, SPIRIT
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __SPIRIT_INCLUDE_CPUFEATURE_H
#define __SPIRIT_INCLUDE_CPUFEATURE_H

#include "common/spirit_comtypes.h"

///////////////////////////////////////////////////////////////////////////////
// CPU feature support detection

#if defined(SPIRIT_COMPILER_INTEL)

#	define SPIRIT_FEATURE_MMX
#	define SPIRIT_FEATURE_SSE

//----------------------------------------------------
// by Platov C.M.
// This part need fix!
// 
//----------------------------------------------------
#	if (__INTEL_COMPILER >= 700)
#		define SPIRIT_FEATURE_SSE2
#		if (__INTEL_COMPILER >= 800)
#			define SPIRIT_FEATURE_SSE3
#		endif // (__INTEL_COMPILER >= 800)
#	endif // (__INTEL_COMPILER >= 700)

#elif defined(SPIRIT_COMPILER_MSVC)

//----------------------------------------------------
// Fixed by Platov C.M.
// MMX/SSE must not be defined by compiler version
// else what about WinCE?
//----------------------------------------------------
//#	if (_MSC_VER >= 1200)
//#		define SPIRIT_FEATURE_MMX
//#		if (_MSC_VER >= 1300)
//#			define SPIRIT_FEATURE_SSE
//#			define SPIRIT_FEATURE_SSE2
//#		endif // (_MSC_VER >= 1300)
//#	endif // (_MSC_VER >= 1200)
//----------------------------------------------------
#	if _M_IX86 >= 600  // Pentum Pro / Pentium with MMX
#		define SPIRIT_FEATURE_MMX
#		if _M_IX86_FP >= 1
#			define SPIRIT_FEATURE_SSE
#			if _M_IX86_FP >= 2
#				define SPIRIT_FEATURE_SSE2
#				if _M_IX86_FP >= 3
#					define SPIRIT_FEATURE_SSE3
#				endif
#			endif
#		endif
#	endif

#elif defined(SPIRIT_COMPILER_GNUC)

#	ifdef __MMX__
#		define SPIRIT_FEATURE_MMX
#		ifdef __SSE__
#			define SPIRIT_FEATURE_SSE
#			ifdef __SSE2__
#				define SPIRIT_FEATURE_SSE2
#				ifdef __SSE3__
#					define SPIRIT_FEATURE_SSE3
#				endif // __SSE3__
#			endif // __SSE2__
#		endif // __SSE__
#	endif // __MMX__

#	ifdef __VEC__
#		define SPIRIT_FEATURE_VEC
#	endif // __VEC__

#endif // SPIRIT_COMPILER_GNUC

///////////////////////////////////////////////////////////////////////////////
// alignment macros

#define SPIRIT_ALIGN4(arg)  SPIRIT_ALIGN(arg, 4)
#define SPIRIT_ALIGN8(arg)  SPIRIT_ALIGN(arg, 8)
#define SPIRIT_ALIGN16(arg) SPIRIT_ALIGN(arg, 16)
#define SPIRIT_ALIGN32(arg) SPIRIT_ALIGN(arg, 32)

///////////////////////////////////////////////////////////////////////////////
// include intrinsic headers

#ifdef SPIRIT_FEATURE_MMX
#include <mmintrin.h>
#endif // SPIRIT_FEATURE_MMX

#ifdef SPIRIT_FEATURE_SSE
#	include <xmmintrin.h>

#	if defined(SPIRIT_COMPILER_GNUC) && (SPIRIT_GCC_VERSION < 30203)
#		define _mm_store_ss(p, v) __builtin_ia32_storess(p, v)
#		define _mm_store_ps(p, v) __builtin_ia32_storeaps(p, v)
#	endif // defined(SPIRIT_COMPILER_GNUC) && (SPIRIT_GCC_VERSION < 30203)

#	if defined(SPIRIT_COMPILER_INTEL) && (SPIRIT_GCC_VERSION < 40000)
		SPIRIT_STATIC_INLINE float _mm_cvtss_f32(__m128 v) { float r; _mm_store_ss(&r, v); return r; }
#	endif // SPIRIT_COMPILER_INTEL
#endif // SPIRIT_FEATURE_SSE

#ifdef SPIRIT_FEATURE_SSE2
#include <emmintrin.h>
#endif // SPIRIT_FEATURE_SSE2

#ifdef SPIRIT_FEATURE_SSE3
#include <pmmintrin.h>
#endif // SPIRIT_FEATURE_SSE3

#endif // __SPIRIT_INCLUDE_CPUFEATURE_H
