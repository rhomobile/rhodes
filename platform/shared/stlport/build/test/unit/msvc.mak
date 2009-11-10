!if !EXIST(..\..\Makefiles\config.mak)
!error No config file found, please run 'configure --help' first.
!endif

!include ..\..\Makefiles\config.mak

!ifndef COMPILER_NAME
!error No compiler set, please run 'configure --help' first and chose a compiler.
!endif

!if (("$(COMPILER_NAME)" != "vc6") && \
     ("$(COMPILER_NAME)" != "vc70") && \
     ("$(COMPILER_NAME)" != "vc71") && \
     ("$(COMPILER_NAME)" != "vc8") && \
     ("$(COMPILER_NAME)" != "icl"))
!error '$(COMPILER_NAME)' not supported by this make file, please rerun 'configure' script and follow instructions.
!endif

SRCROOT=../..
STLPORT_DIR=../../..

!include Makefile.inc

#Comment following line to build unit tests without STLport
INCLUDES=$(INCLUDES) /I$(STLPORT_INCLUDE_DIR) /I$(STLPORT_DIR)/src /FI warning_disable.h
#INCLUDES=$(INCLUDES) /I$(STLPORT_DIR)/src /FI warning_disable.h

# Important in a number of builds.
OPT = /Zm800

DEFS_DBG=/D_STLP_DEBUG_UNINITIALIZED
DEFS_STLDBG=/D_STLP_DEBUG_UNINITIALIZED
DEFS_STATIC_DBG=/D_STLP_DEBUG_UNINITIALIZED
DEFS_STATIC_STLDBG=/D_STLP_DEBUG_UNINITIALIZED

LDSEARCH=$(LDSEARCH) /LIBPATH:$(STLPORT_LIB_DIR)

!include $(SRCROOT)/Makefiles/nmake/top.mak
