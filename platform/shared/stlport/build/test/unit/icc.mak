# -*- Makefile -*- Time-stamp: <04/03/14 18:55:49 ptr>
# $Id: icc.mak 1802 2005-11-01 08:25:57Z complement $

SRCROOT := ../..
COMPILER_NAME := icc

ALL_TAGS := release-shared stldbg-shared
STLPORT_DIR := ../../..
include Makefile.inc
include ${SRCROOT}/Makefiles/top.mak


INCLUDES += -I${STLPORT_INCLUDE_DIR}

ifdef STLP_BUILD_BOOST_PATH
INCLUDES += -I${STLP_BUILD_BOOST_PATH}
endif

release-shared:	LDSEARCH = -L${STLPORT_LIB_DIR} -Wl,-R${STLPORT_LIB_DIR}
stldbg-shared:	LDSEARCH = -L${STLPORT_LIB_DIR} -Wl,-R${STLPORT_LIB_DIR}
dbg-shared:	LDSEARCH = -L${STLPORT_LIB_DIR} -Wl,-R${STLPORT_LIB_DIR}

ifeq ($(OSNAME),linux)
ifeq ($(CXX_VERSION_MAJOR),8)
ifeq ($(CXX_VERSION_MINOR),0)
# 8.0 build 20031016Z
release-shared:	LDLIBS = -lpthread -lstlport
stldbg-shared:	LDLIBS = -lpthread -lstlportstlg
dbg-shared:	LDLIBS = -lpthread -lstlportg
else
# 8.1 build 028
release-shared:	LDLIBS = -lpthread -lstlport -lcprts -lunwind
stldbg-shared:	LDLIBS = -lpthread -lstlportstlg -lcprts -lunwind
dbg-shared:	LDLIBS = -lpthread -lstlportg -lcprts -lunwind
endif
else
ifeq ($(CXX_VERSION_MAJOR),9)
# 9.0 build 20050430
release-shared:	LDLIBS = -lpthread -lstlport -lcprts -lunwind
stldbg-shared:	LDLIBS = -lpthread -lstlportstlg -lcprts -lunwind
dbg-shared:	LDLIBS = -lpthread -lstlportg -lcprts -lunwind
else
# 7.1 build 20030307Z
release-shared:	LDLIBS = -lpthread -lstlport
stldbg-shared:	LDLIBS = -lpthread -lstlportstlg
dbg-shared:	LDLIBS = -lpthread -lstlportg
endif
endif
endif
