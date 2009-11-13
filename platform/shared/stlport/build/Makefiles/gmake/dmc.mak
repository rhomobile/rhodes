# Time-stamp: <03/11/30 11:46:14 ptr>
# $Id: dmc.mak 2484 2006-06-24 20:30:59Z dums $

#INCLUDES :=

ALL_TAGS = all-static all-shared
ifdef LIBNAME
INSTALL_TAGS = install-static install-shared
endif

CXX := dmc
# For the C compiler we force cpp build otherwise wchar_t is not
# supported
CC := dmc
RC := rcc

DEFS ?=
OPT ?=

CFLAGS = -Ae -C -p -3 -w6 -w12
CXXFLAGS = -Ae -C -p -3 -w12

DEFS += -DSTRICT

ifdef STLP_BUILD_FORCE_DYNAMIC_RUNTIME
release-static : OPT += -ND
dbg-static : OPT += -ND
stldbg-static : OPT += -ND
endif

ifndef STLP_BUILD_FORCE_STATIC_RUNTIME
release-shared : OPT += -ND
dbg-shared : OPT += -ND
stldbg-shared : OPT += -ND
endif

ifndef STLP_BUILD_NO_RTTI
OPT += -Ar
endif

OUTPUT_OPTION = -o$@
LINK_OUTPUT_OPTION = $(subst /,\,$@)
CPPFLAGS = $(DEFS) $(OPT) $(INCLUDES) 

CDEPFLAGS = -E -M
CCDEPFLAGS = -E -M
RCFLAGS = -32 -I${STLPORT_INCLUDE_DIR} -DCOMP=dmc

release-shared : RCFLAGS += -DBUILD=r -DBUILD_INFOS="-o"
dbg-shared : RCFLAGS += -DBUILD=g -DBUILD_INFOS="-gl -D_DEBUG"
stldbg-shared : RCFLAGS += -DBUILD=stlg -DBUILD_INFOS="-gl -D_STLP_DEBUG"
RC_OUTPUT_OPTION = $(OUTPUT_OPTION)

COMPILE.rc = ${RC} ${RCFLAGS}
LINK.cc = dm_link $(LDFLAGS)

LDLIBS += user32.lib kernel32.lib

# STLport DEBUG mode specific defines
dbg-static : DEFS += -D_DEBUG
dbg-shared : DEFS += -D_DEBUG
dbg-static-dep : DEFS += -D_DEBUG
dbg-shared-dep : DEFS +=  -D_DEBUG
stldbg-static :	    DEFS += -D_STLP_DEBUG
stldbg-shared :     DEFS += -D_STLP_DEBUG
stldbg-static-dep : DEFS += -D_STLP_DEBUG
stldbg-shared-dep : DEFS += -D_STLP_DEBUG

# optimization and debug compiler flags
release-static : OPT += -o
release-shared : OPT += -o
dbg-static : OPT += -gl
dbg-shared : OPT += -gl
stldbg-static : OPT += -gl
stldbg-shared : OPT += -gl

release-static : OPT += -D_MT
dbg-static : OPT += -D_MT
stldbg-static : OPT += -D_MT

release-static : AR += -p128
dbg-static : AR += -p512
stldbg-static : AR += -p512

# map output option (move map files to output dir)

ifdef LIBNAME
release-shared: MAP_OUTPUT_OPTION = $(subst /,\,$(OUTPUT_DIR))\$(SO_NAME_BASE).map
dbg-shared: MAP_OUTPUT_OPTION = $(subst /,\,$(OUTPUT_DIR_DBG))\$(SO_NAME_DBG_BASE).map
stldbg-shared: MAP_OUTPUT_OPTION = $(subst /,\,$(OUTPUT_DIR_STLDBG))\$(SO_NAME_STLDBG_BASE).map
else
release-shared: MAP_OUTPUT_OPTION = $(subst /,\,$(OUTPUT_DIR))\$(PRGNAME).map
release-static: MAP_OUTPUT_OPTION = $(subst /,\,$(OUTPUT_DIR))\$(PRGNAME).map
dbg-shared: MAP_OUTPUT_OPTION = $(subst /,\,$(OUTPUT_DIR_DBG))\$(PRGNAME).map
dbg-static: MAP_OUTPUT_OPTION = $(subst /,\,$(OUTPUT_DIR_DBG))\$(PRGNAME).map
stldbg-shared: MAP_OUTPUT_OPTION = $(subst /,\,$(OUTPUT_DIR_STLDBG))\$(PRGNAME).map
stldbg-static: MAP_OUTPUT_OPTION = $(subst /,\,$(OUTPUT_DIR_STLDBG))\$(PRGNAME).map
endif

# dependency output parser (dependencies collector)

DP_OUTPUT_DIR = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@

DP_OUTPUT_DIR_DBG = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR_DBG)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@

DP_OUTPUT_DIR_STLDBG = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR_STLDBG)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@

