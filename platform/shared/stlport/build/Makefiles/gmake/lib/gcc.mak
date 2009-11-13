# -*- makefile -*- Time-stamp: <05/12/27 10:53:41 ptr>

# Oh, the commented below work for gmake 3.78.1 and above,
# but phrase without tag not work for it. Since gmake 3.79 
# tag with assignment fail, but work assignment for all tags
# (really that more correct).

ifneq ($(OSNAME), cygming)
ifneq ($(OSNAME), windows)
OPT += -fPIC
endif
endif

ifndef NOT_USE_NOSTDLIB

ifeq ($(CXX_VERSION_MAJOR),2)
# i.e. gcc before 3.x.x: 2.95, etc.
# gcc before 3.x don't had libsupc++.a and libgcc_s.so
# exceptions and operators new are in libgcc.a
#  Unfortunatly gcc before 3.x has a buggy C++ language support outside stdc++, so definition of STDLIBS below is commented
NOT_USE_NOSTDLIB := 1
#STDLIBS := $(shell ${CXX} -print-file-name=libgcc.a) -lpthread -lc -lm
endif

ifeq ($(CXX_VERSION_MAJOR),3)
# gcc before 3.3 (i.e. 3.0.x, 3.1.x, 3.2.x) has buggy libsupc++, so we should link with libstdc++ to avoid one
ifeq ($(CXX_VERSION_MINOR),0)
NOT_USE_NOSTDLIB := 1
endif
ifeq ($(CXX_VERSION_MINOR),1)
NOT_USE_NOSTDLIB := 1
endif
ifeq ($(CXX_VERSION_MINOR),2)
NOT_USE_NOSTDLIB := 1
endif
endif

endif

ifndef NOT_USE_NOSTDLIB
ifeq ($(OSNAME),linux)
_USE_NOSTDLIB := 1
endif

ifeq ($(OSNAME),openbsd)
_USE_NOSTDLIB := 1
endif

ifeq ($(OSNAME),freebsd)
_USE_NOSTDLIB := 1
endif

ifeq ($(OSNAME),netbsd)
_USE_NOSTDLIB := 1
endif

ifeq ($(OSNAME),sunos)
_USE_NOSTDLIB := 1
endif

ifeq ($(OSNAME),darwin)
_USE_NOSTDLIB := 1
endif
endif

ifdef _USE_NOSTDLIB
NOSTDLIB :=

# ifeq ($(CXX_VERSION_MAJOR),3)

# Check whether gcc builded with --disable-shared
ifeq ($(shell ${CXX} -print-file-name=libgcc_eh.a),libgcc_eh.a)
# gcc builded with --disable-shared, (no library libgcc_eh.a); all exception support in libgcc.a
_LGCC_EH :=
_LGCC_S := -lgcc
else
# gcc builded with --enable-shared (default)
ifdef USE_STATIC_LIBGCC
# if force usage of static libgcc, then exceptions support should be taken from libgcc_eh
_LGCC_EH := -lgcc_eh
_LGCC_S := -lgcc
else
# otherwise, exceptions support is in libgcc_s.so
_LGCC_EH :=
_LGCC_S := -lgcc_s
endif
endif

# Include whole language support archive (libsupc++.a) into libstlport:
# all C++ issues are in libstlport now.
ifeq ($(OSNAME),linux)
START_OBJ := $(shell for o in crt{i,beginS}.o; do ${CXX} -print-file-name=$$o; done)
#START_A_OBJ := $(shell for o in crt{i,beginT}.o; do ${CXX} -print-file-name=$$o; done)
END_OBJ := $(shell for o in crt{endS,n}.o; do ${CXX} -print-file-name=$$o; done)
STDLIBS := -Wl,--whole-archive -lsupc++ ${_LGCC_EH} -Wl,--no-whole-archive ${_LGCC_S} -lpthread -lc -lm
endif
ifeq ($(OSNAME),openbsd)
START_OBJ := $(shell for o in crtbeginS.o; do ${CXX} -print-file-name=$$o; done)
END_OBJ := $(shell for o in crtendS.o; do ${CXX} -print-file-name=$$o; done)
STDLIBS := -Wl,--whole-archive -lsupc++ ${_LGCC_EH} -Wl,--no-whole-archive ${_LGCC_S} -lpthread -lc -lm
endif
ifeq ($(OSNAME),freebsd)
# FreeBSD < 5.3 should use -lc_r, while FreeBSD >= 5.3 use -lpthread
PTHR := $(shell if [ ${OSREL_MAJOR} -gt 5 ] ; then echo "pthread" ; else if [ ${OSREL_MAJOR} -lt 5 ] ; then echo "c_r" ; else if [ ${OSREL_MINOR} -lt 3 ] ; then echo "c_r" ; else echo "pthread"; fi ; fi ; fi)
START_OBJ := $(shell for o in crti.o crtbeginS.o; do ${CXX} -print-file-name=$$o; done)
END_OBJ := $(shell for o in crtendS.o crtn.o; do ${CXX} -print-file-name=$$o; done)
STDLIBS := -Wl,--whole-archive -lsupc++ ${_LGCC_EH} -Wl,--no-whole-archive ${_LGCC_S} -l${PTHR} -lc -lm
endif
ifeq ($(OSNAME),netbsd)
START_OBJ := $(shell for o in crt{i,beginS}.o; do ${CXX} -print-file-name=$$o; done)
END_OBJ := $(shell for o in crt{endS,n}.o; do ${CXX} -print-file-name=$$o; done)
STDLIBS := -Wl,--whole-archive -lsupc++ ${_LGCC_EH} -Wl,--no-whole-archive ${_LGCC_S} -lpthread -lc -lm
endif
ifeq ($(OSNAME),sunos)
START_OBJ := $(shell for o in crti.o crtbegin.o; do ${CXX} -print-file-name=$$o; done)
END_OBJ := $(shell for o in crtend.o crtn.o; do ${CXX} -print-file-name=$$o; done)
STDLIBS := -Wl,-zallextract -lsupc++ ${_LGCC_EH} -Wl,-zdefaultextract ${_LGCC_S} -lpthread -lc -lm
endif
ifeq ($(OSNAME),darwin)
START_OBJ := 
END_OBJ := 
ifdef GCC_APPLE_CC
STDLIBS := -lgcc -lc -lm -all_load -lsupc++
else
LDFLAGS += -single_module
STDLIBS := ${_LGCC_S} -lc -lm -all_load -lsupc++ ${_LGCC_EH}
endif
endif
#END_A_OBJ := $(shell for o in crtn.o; do ${CXX} -print-file-name=$$o; done)

NOSTDLIB := -nostdlib
# endif

endif

ifeq ($(OSNAME),hp-ux)
dbg-shared:	LDFLAGS += -shared -Wl,-C20 -Wl,-dynamic  -Wl,+h$(SO_NAME_DBGxx) ${LDSEARCH}
stldbg-shared:	LDFLAGS += -shared -Wl,-C20 -Wl,-dynamic  -Wl,+h$(SO_NAME_STLDBGxx) ${LDSEARCH}
release-shared:	LDFLAGS += -shared -Wl,-C20 -Wl,-dynamic -Wl,+h$(SO_NAMExx) ${LDSEARCH}
endif

ifeq ($(OSNAME),sunos)
dbg-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAME_DBGxx) ${NOSTDLIB} ${LDSEARCH}
stldbg-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAME_STLDBGxx) ${NOSTDLIB} ${LDSEARCH}
release-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAMExx) ${NOSTDLIB} ${LDSEARCH}
dbg-static:	LDFLAGS += ${LDSEARCH}
stldbg-static:	LDFLAGS += ${LDSEARCH}
release-static:	LDFLAGS += ${LDSEARCH}
endif

ifeq ($(OSNAME),linux)
dbg-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAME_DBGxx) ${NOSTDLIB} ${LDSEARCH}
stldbg-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAME_STLDBGxx) ${NOSTDLIB} ${LDSEARCH}
release-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAMExx) ${NOSTDLIB} ${LDSEARCH}
dbg-static:	LDFLAGS += ${LDSEARCH}
stldbg-static:	LDFLAGS += ${LDSEARCH}
release-static:	LDFLAGS += ${LDSEARCH}
endif

ifeq ($(OSNAME),cygming)
dbg-shared:	LDFLAGS += -shared -Wl,--out-implib=${LIB_NAME_OUT_DBG},--enable-auto-image-base
stldbg-shared:	LDFLAGS += -shared -Wl,--out-implib=${LIB_NAME_OUT_STLDBG},--enable-auto-image-base
release-shared:	LDFLAGS += -shared -Wl,--out-implib=${LIB_NAME_OUT},--enable-auto-image-base
dbg-static:	LDFLAGS += -static ${LDSEARCH}
stldbg-static:	LDFLAGS += -static ${LDSEARCH}
release-static:	LDFLAGS += -static ${LDSEARCH}
endif

ifeq ($(OSNAME),windows)
dbg-shared:	LDFLAGS += -shared -Wl,--out-implib=${LIB_NAME_OUT_DBG},--enable-auto-image-base
stldbg-shared:	LDFLAGS += -shared -Wl,--out-implib=${LIB_NAME_OUT_STLDBG},--enable-auto-image-base
release-shared:	LDFLAGS += -shared -Wl,--out-implib=${LIB_NAME_OUT},--enable-auto-image-base
dbg-static:	LDFLAGS += -static ${LDSEARCH}
stldbg-static:	LDFLAGS += -static ${LDSEARCH}
release-static:	LDFLAGS += -static ${LDSEARCH}
endif

ifeq ($(OSNAME),freebsd)
dbg-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAME_DBGxx) ${NOSTDLIB} ${LDSEARCH}
stldbg-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAME_STLDBGxx) ${NOSTDLIB} ${LDSEARCH}
release-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAMExx) ${NOSTDLIB} ${LDSEARCH}
dbg-static:	LDFLAGS += ${LDSEARCH}
stldbg-static:	LDFLAGS += ${LDSEARCH}
release-static:	LDFLAGS += ${LDSEARCH}
endif

ifeq ($(OSNAME),darwin)
CURRENT_VERSION := ${MAJOR}.${MINOR}.${PATCH}
COMPATIBILITY_VERSION := $(CURRENT_VERSION)

dbg-shared:	LDFLAGS += -dynamic -dynamiclib -compatibility_version $(COMPATIBILITY_VERSION) -current_version $(CURRENT_VERSION) -install_name $(SO_NAME_DBGxx) ${LDSEARCH} ${NOSTDLIB}
stldbg-shared:	LDFLAGS += -dynamic -dynamiclib -compatibility_version $(COMPATIBILITY_VERSION) -current_version $(CURRENT_VERSION) -install_name $(SO_NAME_STLDBGxx) ${LDSEARCH} ${NOSTDLIB}
release-shared:	LDFLAGS += -dynamic -dynamiclib -compatibility_version $(COMPATIBILITY_VERSION) -current_version $(CURRENT_VERSION) -install_name $(SO_NAMExx) ${LDSEARCH} ${NOSTDLIB}

dbg-static:	LDFLAGS += -staticlib ${LDSEARCH}
stldbg-static:	LDFLAGS += -staticlib ${LDSEARCH}
release-static:	LDFLAGS += -staticlib ${LDSEARCH}
endif

ifeq ($(OSNAME),openbsd)
dbg-shared:	LDFLAGS += -shared -Wl,-soname -Wl,$(SO_NAME_DBGxx) ${NOSTDLIB} ${LDSEARCH}
stldbg-shared:	LDFLAGS += -shared -Wl,-soname -Wl,$(SO_NAME_STLDBGxx) ${NOSTDLIB} ${LDSEARCH}
release-shared:	LDFLAGS += -shared -Wl,-soname -Wl,$(SO_NAMExx) ${NOSTDLIB} ${LDSEARCH}
dbg-static:	LDFLAGS += ${LDSEARCH}
stldbg-static:	LDFLAGS += ${LDSEARCH}
release-static:	LDFLAGS += ${LDSEARCH}
endif
