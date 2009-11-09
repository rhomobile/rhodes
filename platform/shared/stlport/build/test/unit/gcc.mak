# -*- Makefile -*- Time-stamp: <05/12/27 10:55:26 ptr>

SRCROOT := ../..
COMPILER_NAME := gcc
#NOT_USE_NOSTDLIB := 1
#WITHOUT_STLPORT := 1
ALL_TAGS := release-shared stldbg-shared
STLPORT_DIR := ../../..
include Makefile.inc
include ${SRCROOT}/Makefiles/top.mak

dbg-shared:	DEFS += -D_STLP_DEBUG_UNINITIALIZED 
stldbg-shared:	DEFS += -D_STLP_DEBUG_UNINITIALIZED 

ifdef STLP_BUILD_BOOST_PATH
INCLUDES += -I${STLP_BUILD_BOOST_PATH}
endif

ifndef TARGET_OS
ifeq ($(OSNAME), sunos)
release-shared:	LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
dbg-shared:	LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
stldbg-shared:	LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
endif
ifeq ($(OSNAME), freebsd)
release-shared:	LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
dbg-shared:	LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
stldbg-shared:	LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
endif
ifeq ($(OSNAME), openbsd)
release-shared:	LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
dbg-shared:	LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
stldbg-shared:	LDFLAGS += -Wl,-R${STLPORT_LIB_DIR}
endif
ifeq ($(OSNAME), linux)
release-shared:	LDFLAGS += -Wl,-rpath=${STLPORT_LIB_DIR}
dbg-shared:	LDFLAGS += -Wl,-rpath=${STLPORT_LIB_DIR}
stldbg-shared:	LDFLAGS += -Wl,-rpath=${STLPORT_LIB_DIR}
endif
endif



