# Time-stamp: <05/12/07 01:44:42 ptr>

#INCLUDES :=

ALL_TAGS = all-static all-shared
ifdef LIBNAME
INSTALL_TAGS = install-static install-shared
else
INSTALL_TAGS = install-static
endif

CXX := bcc32 
CC := bcc32
RC := brcc32

DEFS ?=
OPT ?=

CFLAGS = -q -ff 
CXXFLAGS = -q -ff 

OPT += -w-ccc -w-rch -w-ngu -w-inl -w-eff

# release-shared : OPT += -w-inl

ifdef STLP_BUILD_FORCE_DYNAMIC_RUNTIME
release-static : OPT += -tWR
dbg-static : OPT += -tWR
stldbg-static : OPT += -tWR
endif

ifndef STLP_BUILD_FORCE_STATIC_RUNTIME
release-shared : OPT += -tWR
dbg-shared : OPT += -tWR
stldbg-shared : OPT += -tWR
endif

ifdef STLP_BUILD_NO_RTTI
OPT += -RT-
endif

ifndef STLP_BUILD_NO_THREAD
OPT += -tWM
endif

OUTPUT_OPTION = -o$@
LINK_OUTPUT_OPTION = $@
CPPFLAGS = $(DEFS) $(OPT) $(INCLUDES) 

CDEPFLAGS = -E -M
CCDEPFLAGS = -E -M
RCFLAGS = -32 -r -i${STLPORT_INCLUDE_DIR} -dCOMP=bcc

release-shared : RCFLAGS += -dBUILD_INFOS="-O2 -vi-"
dbg-shared : RCFLAGS += -dBUILD=d -dBUILD_INFOS="-R -v -y -D_DEBUG"
stldbg-shared : RCFLAGS += -dBUILD=stld -dBUILD_INFOS="-R -v -y -D_DEBUG -D_STLP_DEBUG"
RC_OUTPUT_OPTION = -fo$@

COMPILE.rc = ${RC} ${RCFLAGS}
LINK.cc = ilink32 $(subst /,\,$(LDFLAGS))

LDFLAGS += -ap -D -Gn -Gi

dbg-static : DEFS += -D_DEBUG
dbg-shared : DEFS += -D_DEBUG
stldbg-static : DEFS += -D_DEBUG
stldbg-shared : DEFS += -D_DEBUG

# STLport DEBUG mode specific defines
stldbg-static :	    DEFS += -D_STLP_DEBUG
stldbg-shared :     DEFS += -D_STLP_DEBUG
stldbg-static-dep : DEFS += -D_STLP_DEBUG
stldbg-shared-dep : DEFS += -D_STLP_DEBUG

# optimization and debug compiler flags
release-static : OPT += -O2 -vi-
release-shared : OPT += -O2 -vi-

dbg-static : OPT += -R -v -y
dbg-shared : OPT += -R -v -y
stldbg-static : OPT += -R -v -y
stldbg-shared : OPT += -R -v -y

LDLIBS += import32.lib kernel32.lib
ifndef STLP_BUILD_NO_THREAD
ifndef STLP_BUILD_FORCE_STATIC_RUNTIME
release-shared : LDLIBS += cw32mti.lib
dbg-shared : LDLIBS += cw32mti.lib
stldbg-shared : LDLIBS += cw32mti.lib
else
release-shared : LDLIBS += cw32mt.lib
dbg-shared : LDLIBS += cw32mt.lib
stldbg-shared : LDLIBS += cw32mt.lib
endif
ifndef STLP_BUILD_FORCE_DYNAMIC_RUNTIME
release-static : LDLIBS += cw32mt.lib
dbg-static : LDLIBS += cw32mt.lib
stldbg-static : LDLIBS += cw32mt.lib
else
release-static : LDLIBS += cw32mti.lib
dbg-static : LDLIBS += cw32mti.lib
stldbg-static : LDLIBS += cw32mti.lib
endif
else
ifndef STLP_BUILD_FORCE_STATIC_RUNTIME
release-shared : LDLIBS += cw32i.lib
dbg-shared : LDLIBS += cw32i.lib
stldbg-shared : LDLIBS += cw32i.lib
else
release-shared : LDLIBS += cw32.lib
dbg-shared : LDLIBS += cw32.lib
stldbg-shared : LDLIBS += cw32.lib
endif
ifndef STLP_BUILD_FORCE_DYNAMIC_RUNTIME
release-static : LDLIBS += cw32.lib
dbg-static : LDLIBS += cw32.lib
stldbg-static : LDLIBS += cw32.lib
else
release-static : LDLIBS += cw32i.lib
dbg-static : LDLIBS += cw32i.lib
stldbg-static : LDLIBS += cw32i.lib
endif
endif

ifndef LIBNAME
ifdef STLP_BUILD_FORCE_DYNAMIC_RUNTIME
release-static: DEFS += -D_STLP_USE_STATIC_LIB
dbg-static:  DEFS += -D_STLP_USE_STATIC_LIB
stldbg-static:  DEFS += -D_STLP_USE_STATIC_LIB
endif
ifdef STLP_BUILD_FORCE_STATIC_RUNTIME
release-shared: DEFS += -D_STLP_USE_DYNAMIC_LIB
dbg-shared:  DEFS += -D_STLP_USE_DYNAMIC_LIB
stldbg-shared:  DEFS += -D_STLP_USE_DYNAMIC_LIB
endif
endif

# map output option (see build/Makefiles/gmake/dmc.mak)

MAP_OUTPUT_OPTION = 

# dependency output parser (dependencies collector)

DP_OUTPUT_DIR = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@

DP_OUTPUT_DIR_DBG = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR_DBG)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@

DP_OUTPUT_DIR_STLDBG = | sed 's|\($*\)\.o[ :]*|$(OUTPUT_DIR_STLDBG)/\1.o $@ : |g' > $@; \
                           [ -s $@ ] || rm -f $@

