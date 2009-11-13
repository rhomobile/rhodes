# -*- Makefile -*- Time-stamp: <05/03/10 17:51:53 ptr>
# $Id: gcc.mak 1817 2005-11-09 13:59:37Z dums $

SRCROOT := ../..
COMPILER_NAME := gcc

ALL_TAGS := release-shared dbg-shared stldbg-shared
STLPORT_DIR := ../../..
include Makefile.inc
include ${SRCROOT}/Makefiles/top.mak

INCLUDES += -I${STLPORT_INCLUDE_DIR}
DEFS += -D_STLP_NO_CUSTOM_IO

ifeq ($(OSNAME), cygming)
release-shared:	DEFS += -D_STLP_USE_DYNAMIC_LIB
dbg-shared:	DEFS += -D_STLP_USE_DYNAMIC_LIB
stldbg-shared:	DEFS += -D_STLP_USE_DYNAMIC_LIB
endif

ifdef STLP_BUILD_BOOST_PATH
INCLUDES += -I${STLP_BUILD_BOOST_PATH}
endif

ifndef TARGET_OS
release-shared:	LDSEARCH = -L${STLPORT_LIB_DIR} -Wl,-R${STLPORT_LIB_DIR}
dbg-shared:	LDSEARCH = -L${STLPORT_LIB_DIR} -Wl,-R${STLPORT_LIB_DIR}
stldbg-shared:	LDSEARCH = -L${STLPORT_LIB_DIR} -Wl,-R${STLPORT_LIB_DIR}
else
release-shared:	LDSEARCH = -L${STLPORT_LIB_DIR}
dbg-shared:	LDSEARCH = -L${STLPORT_LIB_DIR}
stldbg-shared:	LDSEARCH = -L${STLPORT_LIB_DIR}
endif

dbg-shared:	DEFS += -D_STLP_DEBUG_UNINITIALIZED 
stldbg-shared:	DEFS += -D_STLP_DEBUG_UNINITIALIZED 

ifeq ($(OSNAME),cygming)
LIB_VERSION = ${LIBMAJOR}.${LIBMINOR}
release-shared : LDLIBS = -lstlport.${LIB_VERSION}
dbg-shared     : LDLIBS = -lstlportg.${LIB_VERSION}
stldbg-shared  : LDLIBS = -lstlportstlg.${LIB_VERSION}
else
ifeq ($(OSNAME),windows)
LIB_VERSION = ${LIBMAJOR}.${LIBMINOR}
release-shared : LDLIBS = -lstlport.${LIB_VERSION}
dbg-shared     : LDLIBS = -lstlportg.${LIB_VERSION}
stldbg-shared  : LDLIBS = -lstlportstlg.${LIB_VERSION}
else
release-shared : LDLIBS = -lstlport
dbg-shared     : LDLIBS = -lstlportg
stldbg-shared  : LDLIBS = -lstlportstlg
endif
endif

ifeq ($(OSNAME),sunos)
release-shared : LDLIBS = -lstlport -lrt
stldbg-shared  : LDLIBS = -lstlportstlg -lrt
dbg-shared     : LDLIBS = -lstlportg -lrt
endif

