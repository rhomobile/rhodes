# -*- Makefile -*- Time-stamp: <05/06/03 21:45:55 ptr>
# $Id: mwccnlm.mak 1871 2005-11-26 20:55:00Z dums $

SRCROOT := ../..
COMPILER_NAME := mwccnlm

ALL_TAGS := release-shared dbg-shared stldbg-shared
STLPORT_DIR := ../../..
include Makefile.inc
include ${SRCROOT}/Makefiles/top.mak

INCLUDES += -I${STLPORT_INCLUDE_DIR}

dbg-shared:	DEFS += -D_STLP_DEBUG_UNINITIALIZED
stldbg-shared:	DEFS += -D_STLP_DEBUG_UNINITIALIZED

ifdef STLP_BUILD_BOOST_PATH
INCLUDES += -I${STLP_BUILD_BOOST_PATH}
endif

ifndef TARGET_OS
release-shared:	LDSEARCH = -L+${STLPORT_LIB_DIR} -Wl,-R${STLPORT_LIB_DIR}
dbg-shared:	LDSEARCH = -L${STLPORT_LIB_DIR} -Wl,-R${STLPORT_LIB_DIR}
stldbg-shared:	LDSEARCH = -L${STLPORT_LIB_DIR} -Wl,-R${STLPORT_LIB_DIR}
else
#release-shared:	LDSEARCH = -Le:/STLport/netware-lib
release-shared:	LDSEARCH = -L${STLPORT_LIB_DIR}
dbg-shared:	LDSEARCH = -L${STLPORT_LIB_DIR}
stldbg-shared:	LDSEARCH = -L${STLPORT_LIB_DIR}
endif


LIB_VERSION = ${LIBMAJOR}${LIBMINOR}
#release-shared : LDLIBS = e:/STLport/netware-lib/stlport_r${LIB_VERSION}_static.lib
release-shared : LDLIBS = ${STLPORT_LIB_DIR}/stlport_r${LIB_VERSION}_static.lib
dbg-shared     : LDLIBS = -lstlport_d${LIB_VERSION}
stldbg-shared  : LDLIBS = -lstlport_stld${LIB_VERSION}

LDFLAGS += -commandfile novelltest.def "$(MWCW_NOVELL)/Libraries/Runtime/mwcrtl.lib"  "$(MWCW_NOVELL)/Libraries/Runtime/Output/CLib/mwcrtl.lib"
