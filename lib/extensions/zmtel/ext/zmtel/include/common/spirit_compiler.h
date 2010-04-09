///////////////////////////////////////////////////////////////////////////////
//
// Include\compiler.h
//
// Primary OS detection for SPIRIT Library
//
// Copyright (C) 2006, SPIRIT
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __SPIRIT_INCLUDE_COMPILER_H
#define __SPIRIT_INCLUDE_COMPILER_H

#include "common/spirit_ostype.h"

#if defined(__BORLANDC__)
  #define SPIRIT_COMPILER_BORLANDC
#elif defined(__INTEL_COMPILER)
  #define SPIRIT_COMPILER_INTEL
#elif defined(_MSC_VER)
  #define SPIRIT_COMPILER_MSVC
#elif defined(__GNUC__)
  #define SPIRIT_COMPILER_GNUC
  #define SPIRIT_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif defined(__WATCOMC__)
  #define SPIRIT_COMPILER_WATCOM
#elif defined(_TMS320C6X) //what we should specify here??!
  #define SPIRIT_COMPILER_TI
#elif defined(__CC_ARM)  
  #define SPIRIT_COMPILER_ARMCC
#else
  //#pragma message( "undefined compiler!" )
  #error "undefined compiler!"
#endif

#ifdef SPIRIT_COMPILER_MSVC
  #ifdef _DEBUG
    #pragma warning(disable:4786)
  #endif // _DEBUG
  
  #pragma warning(disable:4190)
  #pragma warning(disable:4530)
  #pragma warning(disable:4355)
  
  #ifndef __FUNCTION__
    #define __FUNCTION__ "undefined macro __FUNCTION__"
  #endif
#endif // SPIRIT_COMPILER_MSVC

#ifdef SPIRIT_COMPILER_INTEL
#pragma warning(disable:1572)
#pragma warning(disable:1172)
#endif // SPIRIT_COMPILER_INTEL

#if defined(SPIRIT_COMPILER_GNUC) || defined(SPIRIT_OS_LINUX) || defined(SPIRIT_OS_MACOSX)
#define SPIRIT_GNU_ASM
#endif // defined(SPIRIT_COMPILER_GNUC)

#endif //__SPIRIT_INCLUDE_COMPILER_H



