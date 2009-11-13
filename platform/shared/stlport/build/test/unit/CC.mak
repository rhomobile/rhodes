# -*- Makefile -*- Time-stamp: <05/09/04 22:01:11 ptr>

SRCROOT := ../..
COMPILER_NAME := CC

ALL_TAGS := release-shared stldbg-shared
STLPORT_DIR := ../../..
STLPORT_ETC_DIR = ${STLPORT_DIR}/etc
include Makefile.inc
include ${SRCROOT}/Makefiles/top.mak

INCLUDES += -I. -I${STLPORT_INCLUDE_DIR}
DEFS += -D_STLP_NO_CUSTOM_IO

dbg-shared:	DEFS += -D_STLP_DEBUG_UNINITIALIZED 
stldbg-shared:	DEFS += -D_STLP_DEBUG_UNINITIALIZED -D_STLP_DEBUG

ifdef STLP_BUILD_BOOST_PATH
INCLUDES += -I${STLP_BUILD_BOOST_PATH}
endif

release-shared:	LDSEARCH = -L${INSTALL_LIB_DIR}
dbg-shared:	LDSEARCH = -L${INSTALL_LIB_DIR}
stldbg-shared:	LDSEARCH = -L${INSTALL_LIB_DIR}

release-shared:	LDFLAGS += -R${INSTALL_LIB_DIR}
dbg-shared:	LDFLAGS += -R${INSTALL_LIB_DIR}
stldbg-shared:	LDFLAGS += -R${INSTALL_LIB_DIR}

release-shared : LDLIBS = -lstlport
dbg-shared     : LDLIBS = -lstlportg
stldbg-shared  : LDLIBS = -lstlportstlg

