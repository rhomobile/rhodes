# Time-stamp: <05/03/21 10:42:37 ptr>
# $Id: aCC.mak 2415 2006-06-08 19:47:40Z dums $

#INCLUDES = -I$(SRCROOT)/include
INCLUDES :=

CXX := aCC
CC := aCC

ifdef TARGET_OS
CXX := ${TARGET_OS}-${CXX}
CC := ${TARGET_OS}-${CC}
endif

CXX_VERSION := $(shell ${CXX} --version 2>&1 | grep ${CXX} | awk '{ print $$6; }')
ifeq ($(CXX_VERSION),)
CXX_VERSION := $(shell ${CXX} --version)
endif
CXX_VERSION_MAJOR := $(shell echo ${CXX_VERSION} | awk 'BEGIN { FS = "."; } { print $$2; }')
CXX_VERSION_MINOR := $(shell echo ${CXX_VERSION} | awk 'BEGIN { FS = "."; } { print $$3; }')

DEFS ?=
OPT ?=

OUTPUT_OPTION = -o $@
LINK_OUTPUT_OPTION = ${OUTPUT_OPTION}
CPPFLAGS = $(DEFS) $(INCLUDES)

CFLAGS = -Aa +z $(OPT)
CXXFLAGS = -Aa +z -mt $(OPT)

CDEPFLAGS = -E +Md
CCDEPFLAGS = -E +Md

# STLport DEBUG mode specific defines
stldbg-static :	    DEFS += -D_STLP_DEBUG
stldbg-shared :     DEFS += -D_STLP_DEBUG
stldbg-static-dep : DEFS += -D_STLP_DEBUG
stldbg-shared-dep : DEFS += -D_STLP_DEBUG

# optimization and debug compiler flags
release-static : OPT += +O2
release-shared : OPT += +O2

dbg-static : OPT += -g
dbg-shared : OPT += -g
#dbg-static-dep : OPT += -g
#dbg-shared-dep : OPT += -g

stldbg-static : OPT += -g
stldbg-shared : OPT += -g
#stldbg-static-dep : OPT += -g
#stldbg-shared-dep : OPT += -g

OPT += +W495,749,2186,2191,2340,2430,2550

# dependency output parser (dependencies collector)

DP_OUTPUT_DIR = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@

DP_OUTPUT_DIR_DBG = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR_DBG)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@

DP_OUTPUT_DIR_STLDBG = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR_STLDBG)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@

