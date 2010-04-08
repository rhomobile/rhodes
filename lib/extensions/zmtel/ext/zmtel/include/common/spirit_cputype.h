///////////////////////////////////////////////////////////////////////////////
//
// Include\cputypes.h
//
// Primary cpu definitions for SPIRIT Library
//
// Copyright (C) 2006, SPIRIT
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __SPIRIT_INCLUDE_CPUTYPE_H
#define __SPIRIT_INCLUDE_CPUTYPE_H

///////////////////////////////////////////////////////////////////////////////
// cpu type detection [inspired by C. Platov]

//----------------------------------------------------------
// arm
//----------------------------------------------------------
#if defined(__TARGET_ARCH_6) ||\
	defined(__TARGET_ARCH_6K) ||\
	defined(__TARGET_ARCH_6ZK) ||\
	defined(__TARGET_ARCH_6J) ||\
    defined( __ARM_ARCH_6ZK__ ) || \
    defined( __ARM_ARCH_6K__ ) || \
    defined( __ARM_ARCH_6J__ )
  #define SPIRIT_CPU_ARMV6
  #define SPIRIT_CPU_ARMV5E //!!! Tempotary added define
//----------------------------------------------------------
#elif defined(__TARGET_ARCH_5TE) || \
	  defined (__ARM_ARCH_5E__)  || \
      defined (__ARM_ARCH_5TE__) || \
      defined (__ARM_ARCH_5TEJ__) || \
      defined(ARMV5)             || \
      (defined(_M_ARM) && (_M_ARM == 5))
  #define SPIRIT_CPU_ARMV5E

//----------------------------------------------------------
#elif defined(__TARGET_ARCH_4T) || \
      defined (__ARM_ARCH_4__)  || \
      defined (__ARM_ARCH_4T__) || \
      defined(ARMV4) || defined (ARMV4I) || \
      (defined(_M_ARM) && (_M_ARM == 4))
  #define SPIRIT_CPU_ARMV4

//----------------------------------------------------------
#elif defined (__TARGET_ARCH_5T) || \
      defined (__ARM_ARCH_5T__)  || \
      defined (__ARM_ARCH_5__)
  #define SPIRIT_CPU_ARMV5

//----------------------------------------------------------
// intel x86
//----------------------------------------------------------
#elif (defined(_M_IX86) && (_M_IX86 == 300)) ||\
		(defined(__i386) && (__tune_i386__))
  #define SPIRIT_CPU_I386
//----------------------------------------------------------
#elif defined(_M_IX86) && (_M_IX86 == 400) ||\
		(defined(__i386) && (__tune_i486__))
  #define SPIRIT_CPU_I486

//----------------------------------------------------------
#elif defined(_M_IX86) && (_M_IX86 == 500) ||\
		(defined(__i386) && (__tune_i586__))
  #define SPIRIT_CPU_PENTIUM

//----------------------------------------------------------
#elif defined(_M_IX86) && (_M_IX86 == 600) 
  #define SPIRIT_CPU_PENTIUMPRO

//----------------------------------------------------------
#elif defined(_M_IX86) && (_M_IX86 == 700) ||\
		(defined(__i386) && (__tune_i686__))
  #define SPIRIT_CPU_I686

//----------------------------------------------------------
// power pc
// power macintosh
//----------------------------------------------------------
#elif (defined(_M_IX86) && (_M_MPPC == 601)) ||\
	  (defined(_M_IX86) && (_M_PPC == 601))
  #define SPIRIT_CPU_PPC601
#elif (defined(_M_IX86) && (_M_MPPC == 603)) ||\
	  (defined(_M_IX86) && (_M_PPC == 603))
  #define SPIRIT_CPU_PPC603
#elif (defined(_M_IX86) && (_M_MPPC == 604)) ||\
	  (defined(_M_IX86) && (_M_PPC == 604))
  #define SPIRIT_CPU_PPC604
#elif (defined(_M_IX86) && (_M_MPPC == 620)) ||\
	  (defined(_M_IX86) && (_M_PPC == 620))
  #define SPIRIT_CPU_PPC620

//----------------------------------------------------------
// sun sparc
//----------------------------------------------------------
#elif 0
  #define SPIRIT_CPU_SPARCV7
#elif defined(__sparcv8) || defined(__supersparc__) || \
		defined(__sparclet__) || defined(__sparclite__)
  #define SPIRIT_CPU_SPARCV8
#elif defined(__sparcv9)
  #define SPIRIT_CPU_SPARCV9

//----------------------------------------------------------
// apple
//----------------------------------------------------------
#elif defined(__ppc__)
  #define SPIRIT_CPU_PPC		
//----------------------------------------------------------
#elif defined(__i386__)
  #define SPIRIT_CPU_X86
  #define SPIRIT_CPU_I386

//----------------------------------------------------------
// tms320
//----------------------------------------------------------
#elif defined(_TMS320C6X) || defined( ti_targets_C64P )
//This section covers TMS320 C6200,C6400,C6700 targets
  #define SPIRIT_CPU_TMS320C6X
#elif defined(__TMS320C55XX__)
  #define SPIRIT_CPU_TMS320C55
#elif defined(_TMS320C5XX)
  #define SPIRIT_CPU_TMS320C54

//----------------------------------------------------------
// MIPS
//----------------------------------------------------------
#elif defined(__mips__) || defined(_mips)
	//----------------------------------------------------------
	// MIPS ISA for gcc
	//----------------------------------------------------------
		#if defined(_MIPS_ARCH_MIPS1)
			#define SPIRIT_CPU_MIPS1
		#elif defined(_MIPS_ARCH_R2000)
			#define SPIRIT_CPU_R2000
			#define SPIRIT_CPU_MIPS1
		#elif defined(_MIPS_ARCH_R3000)
			#define SPIRIT_CPU_R3000
			#define SPIRIT_CPU_MIPS1
		#elif defined(_MIPS_ARCH_R3900)
			#define SPIRIT_CPU_R3900
			#define SPIRIT_CPU_MIPS1
		#elif defined(_MIPS_ARCH_R8000)
			#define SPIRIT_CPU_R8000
			#define SPIRIT_CPU_MIPS1
	//----------------------------------------------------------
	//#elif _MIPS_ISA == _MIPS_ISA_MIPS2

		#elif defined(_MIPS_ARCH_MIPS2)
			#define SPIRIT_CPU_MIPS2
		#elif defined(_MIPS_ARCH_R6000)
			#define SPIRIT_CPU_R6000
			#define SPIRIT_CPU_MIPS2

	//----------------------------------------------------------
	//#elif _MIPS_ISA == _MIPS_ISA_MIPS3
		#elif defined(_MIPS_ARCH_MIPS3)
			#define SPIRIT_CPU_MIPS3
		#elif defined(_MIPS_ARCH_ORION)
			#define SPIRIT_CPU_ORION
			#define SPIRIT_CPU_MIPS3
		#elif defined(_MIPS_ARCH_R4000)
			#define SPIRIT_CPU_R4000
			#define SPIRIT_CPU_MIPS3
		#elif defined(_MIPS_ARCH_R4400)
			#define SPIRIT_CPU_R4400
			#define SPIRIT_CPU_MIPS3
		#elif defined(_MIPS_ARCH_R4600)
			#define SPIRIT_CPU_R4600
			#define SPIRIT_CPU_MIPS3
		#elif defined(_MIPS_ARCH_R4650)
			#define SPIRIT_CPU_R4650
			#define SPIRIT_CPU_MIPS3
		#elif defined(_MIPS_ARCH_VR4100)
			#define SPIRIT_CPU_VR4100
			#define SPIRIT_CPU_MIPS3
		#elif defined(_MIPS_ARCH_VR4111)
			#define SPIRIT_CPU_VR4111
			#define SPIRIT_CPU_MIPS3
		#elif defined(_MIPS_ARCH_VR4120)
			#define SPIRIT_CPU_VR4120
			#define SPIRIT_CPU_MIPS3
		#elif defined(_MIPS_ARCH_VR4300)
			#define SPIRIT_CPU_VR4300
			#define SPIRIT_CPU_MIPS3

	//----------------------------------------------------------
	//#elif _MIPS_ISA == _MIPS_ISA_MIPS4
		#elif defined(_MIPS_ARCH_MIPS4)
			#define SPIRIT_CPU_MIPS4
		#elif defined(_MIPS_ARCH_RM7000)
			#define SPIRIT_CPU_RM7000
			#define SPIRIT_CPU_MIPS4
		#elif defined(_MIPS_ARCH_RM9000)
			#define SPIRIT_CPU_RM9000
			#define SPIRIT_CPU_MIPS4
		#elif defined(_MIPS_ARCH_VR5000)
			#define SPIRIT_CPU_VR5000
			#define SPIRIT_CPU_MIPS4
		#elif defined(_MIPS_ARCH_VR5400)
			#define SPIRIT_CPU_VR5400
			#define SPIRIT_CPU_MIPS4
		#elif defined(_MIPS_ARCH_VR5500)
			#define SPIRIT_CPU_VR5500
			#define SPIRIT_CPU_MIPS4

	//----------------------------------------------------------
	//#elif _MIPS_ISA == _MIPS_ISA_MIPS32
		#elif defined(_MIPS_ARCH_MIPS32)
			#define SPIRIT_CPU_MIPS32
		#elif defined(_MIPS_ARCH_MIPS32R2)
			#define SPIRIT_CPU_MIPS32R2
			#define SPIRIT_CPU_MIPS32
		#elif defined(_MIPS_ARCH_4KC)
			#define SPIRIT_CPU_4KC
			#define SPIRIT_CPU_MIPS32
		#elif defined(_MIPS_ARCH_4KP)
			#define SPIRIT_CPU_4KP
			#define SPIRIT_CPU_MIPS32
		#elif defined(_MIPS_ARCH_M4k)
			#define SPIRIT_CPU_M4K
			#define SPIRIT_CPU_MIPS32
	//----------------------------------------------------------
	//#elif _MIPS_ISA == _MIPS_ISA_MIPS64
		#elif defined(_MIPS_ARCH_MIPS64)
			#define SPIRIT_CPU_MIPS64
		#elif defined(_MIPS_ARCH_5KC)
			#define SPIRIT_CPU_5KC
			#define SPIRIT_CPU_MIPS64
		#elif defined(_MIPS_ARCH_20KC)
			#define SPIRIT_CPU_20KC
			#define SPIRIT_CPU_MIPS64
		#elif defined(_MIPS_ARCH_SB1)
			#define SPIRIT_CPU_SB1
			#define SPIRIT_CPU_MIPS64
		#elif defined(_MIPS_ARCH_SR71000)
			#define SPIRIT_CPU_SR71000
			#define SPIRIT_CPU_MIPS64
	//----------------------------------------------------------
	//#else
	//	#error  "Unknown MIPS ISA!"
	//#endif
	//----------------------------------------------------------
		#else
			#error Undefined MIPS CPU!
		#endif

//----------------------------------------------------------
//
//----------------------------------------------------------
#else
  #error "undefined CPU type!"
#endif

///////////////////////////////////////////////////////////////////////////////
// machine architecture detection [inspired by C. Platov]
// ATTENTION: 'SPIRIT_ARCH_xxx' defines exist in /system/SpiritSystem.h '_eSpiritProcessorType'

#if defined( SPIRIT_CPU_ARMV4 ) || \
    defined( SPIRIT_CPU_ARMV5 ) || \
    defined( SPIRIT_CPU_ARMV5E ) || \
    defined( SPIRIT_CPU_ARMV6 )

#	define SPIRIT_ARCH_ARM
#	define SPIRIT_ARCHITECTURE_ARM

//----------------------------------------------------------
#elif defined( SPIRIT_CPU_I386 ) || \
      defined( SPIRIT_CPU_I486 ) || \
      defined( SPIRIT_CPU_PENTIUM ) || \
      defined( SPIRIT_CPU_PENTIUMPRO ) || \
      defined( SPIRIT_CPU_I686 ) || \
      defined( SPIRIT_CPU_X86 )

#	define SPIRIT_ARCH_X86
#	define SPIRIT_ARCHITECTURE_X86

//----------------------------------------------------------
#elif defined( SPIRIT_CPU_PPC601 ) || \
      defined( SPIRIT_CPU_PPC603 ) || \
      defined( SPIRIT_CPU_PPC604 ) || \
      defined( SPIRIT_CPU_PPC620 ) ||\
	  defined( SPIRIT_CPU_PPC ) ||\
	  defined( __POWERPC__) ||\
	  defined( _ARCH_PPC)

#	define SPIRIT_ARCH_PPC
#	define SPIRIT_ARCHITECTURE_PPC

//----------------------------------------------------------
#elif defined( SPIRIT_CPU_SPARCV8 ) || \
	defined( SPIRIT_CPU_SPARCV7 ) || \
	defined( SPIRIT_CPU_SPARCV9 )
#	define SPIRIT_ARCHITECTURE_SPARC

//----------------------------------------------------------
#elif defined( SPIRIT_CPU_TMS320C6X ) || \
      defined( SPIRIT_CPU_TMS320C55 ) || \
      defined( SPIRIT_CPU_TMS320C54 )

#	define SPIRIT_ARCH_TMS320
#	define SPIRIT_ARCHITECTURE_TMS320

//----------------------------------------------------------
#elif defined( SPIRIT_CPU_MIPS1 ) || \
	defined( SPIRIT_CPU_MIPS2 ) || \
	defined( SPIRIT_CPU_MIPS3 ) || \
	defined( SPIRIT_CPU_MIPS4 ) || \
	defined( SPIRIT_CPU_MIPS32 ) || \
	defined( SPIRIT_CPU_MIPS64 )

#	define SPIRIT_ARCH_MIPS
#	define SPIRIT_ARCHITECTURE_MIPS

//----------------------------------------------------------
#else

///////////////////////////////////////////////////////////////////////////////
// OLD Architect detection
//#	if defined(SPIRIT_OS_WIN32) || defined(SPIRIT_OS_LINUX) || defined(SPIRIT_OS_CYGWIN) || defined(SPIRIT_COMPILER_INTEL)
//#		define SPIRIT_ARCHITECTURE_X86
//#	elif defined(SPIRIT_OS_MACOSX) || defined(SPIRIT_OS_SOLARIS)
//# 		ifdef __LITTLE_ENDIAN__
//#			define SPIRIT_ARCHITECTURE_X86
//#		else
//#			define SPIRIT_ARCHITECTURE_PPC
//# 		endif
//#	else
//#		error "undefined machine architecture!"
//#	endif
#		error "undefined machine architecture!"
#endif



///////////////////////////////////////////////////////////////////////////////
// machine endianess detection [inspired by C. Platov]

#if ( defined(__BIG_ENDIAN) && defined(__CC_ARM) ) || \
    defined(__ARMEB__) || \
    defined (__MIPSEB__) || \
    ( defined(__BigEndian)  && defined(__ghs__) ) || \
    defined(_BIG_ENDIAN) || \
    defined(__BIG_ENDIAN__) || \
	defined(SPIRIT_ARCHITECTURE_SPARC)

  #define SPIRIT_ENDIANESS_BE

#elif defined(__LITTLE_ENDIAN) || \
      defined(__ARMEL__) || \
      defined(__MIPSEL__) || \
      defined(_WIN32_WCE) || \
      defined(_WIN32) || \
      ( defined(__i386__) && defined(__unix__) ) || \
      ( defined(__ARMCC_VERSION) && defined(__CC_ARM) && !defined(__BIG_ENDIAN) ) || \
      defined(__ARMWEL__) || \
      ( defined(__arm__) && !defined (__ARMEB__) ) || \
      ( defined(__LittleEndian) && defined(__ghs__) ) || \
      defined(_LITTLE_ENDIAN) || \
      defined(__LITTLE_ENDIAN__) 

  #define SPIRIT_ENDIANESS_LE

#else
  #error "undefined endianess!"
#endif




#endif //__SPIRIT_INCLUDE_CPUTYPE_H



