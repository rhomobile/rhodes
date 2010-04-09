//------------------------------------------------------------------------------
//
// spirit_debug.h
//
// common definitions of debug macro for SPIRIT Library
//
// Copyright (C) 2007, SPIRIT
//
//------------------------------------------------------------------------------

#ifndef __SPIRIT_DEBUG_H
  #define __SPIRIT_DEBUG_H

#include "spirit_comtypes.h"

#include <assert.h>

#ifdef _DEBUG
#  ifndef ASSERT
#    define ASSERT( expr ) assert( expr )
#  endif
#  ifndef VERIFY
#    define VERIFY( f )    ASSERT( f )
#  endif
#else
#  ifndef ASSERT
#    define ASSERT         /* nothing */
#  endif
#  ifndef VERIFY
#    define VERIFY( f )    ( ( void )( f ) )
#  endif
#endif // _DEBUG

#ifdef SPIRIT_ARCHITECTURE_X86
#  ifdef SPIRIT_GNU_ASM
#    define DEBUG_INT __asm__(".byte 0xcc")
#  else
#    define DEBUG_INT __asm { int 3 }
#  endif
#else
#  define DEBUG_INT   assert(0)
#endif // SPIRIT_ARCHITECTURE_X86

#endif // __SPIRIT_DEBUG_H
