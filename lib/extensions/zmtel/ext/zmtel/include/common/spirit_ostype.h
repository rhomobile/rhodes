///////////////////////////////////////////////////////////////////////////////
//
// Include\comtypes.h
//
// Primary OS detection for SPIRIT Library
//
// Copyright (C) 2006, SPIRIT
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __SPIRIT_INCLUDE_OSTYPE_H
#define __SPIRIT_INCLUDE_OSTYPE_H


#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#	define SPIRIT_OS_WIN32
#	if defined(WINCE) || defined(UNDER_CE)
#		define SPIRIT_OS_WIN32_WCE
#	elif defined(_WIN32_WCE)
#		if _WIN32_WCE > 0
#			define SPIRIT_OS_WIN32_WCE
#		endif
#	endif

#elif defined(__linux__) || defined(__linux)
#	define SPIRIT_OS_LINUX 

#elif defined(__sun) || defined(sun)
#	define SPIRIT_OS_SOLARIS

#elif defined(__APPLE__) && defined(__GNUC__)
#	define SPIRIT_OS_MACOSX  

#elif defined(__MACOSX__)
#	define SPIRIT_OS_MACOSX

#elif defined(macintosh)
#	define SPIRIT_OS_MACOS9

#elif defined(__CYGWIN__)
#	define SPIRIT_OS_CYGWIN

#elif defined(_TMS320C6X)  //what we should specify here??!	
#	if defined(_SPIRIT_DSP_BIOS) //!!manualy define
#		define SPIRIT_OS_DSP_BIOS
#	endif

#elif defined(__CC_ARM) //ARM C Compiler define pseudo OS ADS
#	define SPIRIT_OS_ADS

#elif defined(__WASSABI__) //other *nix systems
#	define SPIRIT_OS_WASSABI
  //#pragma message( "undefined OS type!" )
#else
  //#pragma message( "undefined OS type!" )
#	error undefined OS type!
#endif // OS detection


#endif //__SPIRIT_INCLUDE_OSTYPE_H



