# -*- Makefile -*- Time-stamp: <05/12/15 01:46:30 ptr>

ifndef NOT_USE_NOSTDLIB

ifeq ($(CXX_VERSION_MAJOR),2)
# i.e. gcc before 3.x.x: 2.95, etc.
# gcc before 3.x don't had libsupc++.a and libgcc_s.so
# exceptions and operators new are in libgcc.a
#  Unfortunatly gcc before 3.x has a buggy C++ language support outside stdc++, so definition of STDLIB below is commented
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

ifndef WITHOUT_STLPORT
LDSEARCH += -L${STLPORT_LIB_DIR}

release-shared:	STLPORT_LIB = -lstlport
dbg-shared:	STLPORT_LIB = -lstlportg
stldbg-shared:	STLPORT_LIB = -lstlportstlg

ifeq ($(OSNAME),cygming)
LIB_VERSION = ${LIBMAJOR}.${LIBMINOR}
release-shared : STLPORT_LIB = -lstlport.${LIB_VERSION}
dbg-shared     : STLPORT_LIB = -lstlportg.${LIB_VERSION}
stldbg-shared  : STLPORT_LIB = -lstlportstlg.${LIB_VERSION}
release-static : STLPORT_LIB = -lstlport.${LIB_VERSION}
dbg-static     : STLPORT_LIB = -lstlportg.${LIB_VERSION}
stldbg-static  : STLPORT_LIB = -lstlportstlg.${LIB_VERSION}
endif

ifeq ($(OSNAME),windows)
LIB_VERSION = ${LIBMAJOR}.${LIBMINOR}
release-shared : STLPORT_LIB = -lstlport.${LIB_VERSION}
dbg-shared     : STLPORT_LIB = -lstlportg.${LIB_VERSION}
stldbg-shared  : STLPORT_LIB = -lstlportstlg.${LIB_VERSION}
release-static : STLPORT_LIB = -lstlport.${LIB_VERSION}
dbg-static     : STLPORT_LIB = -lstlportg.${LIB_VERSION}
stldbg-static  : STLPORT_LIB = -lstlportstlg.${LIB_VERSION}
endif

endif

ifdef _USE_NOSTDLIB

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

ifeq ($(OSNAME),linux)
START_OBJ := $(shell for o in crt{1,i,begin}.o; do ${CXX} -print-file-name=$$o; done)
END_OBJ := $(shell for o in crt{end,n}.o; do ${CXX} -print-file-name=$$o; done)
STDLIBS = ${STLPORT_LIB} ${_LGCC_S} -lpthread -lc -lm
endif
ifeq ($(OSNAME),openbsd)
START_OBJ := $(shell for o in crt{0,begin}.o; do ${CXX} -print-file-name=$$o; done)
END_OBJ := $(shell for o in crtend.o; do ${CXX} -print-file-name=$$o; done)
STDLIBS = ${STLPORT_LIB} ${_LGCC_S} -lpthread -lc -lm
endif
ifeq ($(OSNAME),freebsd)
# FreeBSD < 5.3 should use -lc_r, while FreeBSD >= 5.3 use -lpthread
PTHR := $(shell if [ ${OSREL_MAJOR} -gt 5 ] ; then echo "pthread" ; else if [ ${OSREL_MAJOR} -lt 5 ] ; then echo "c_r" ; else if [ ${OSREL_MINOR} -lt 3 ] ; then echo "c_r" ; else echo "pthread"; fi ; fi ; fi)
START_OBJ := $(shell for o in crt1.o crti.o crtbegin.o; do ${CXX} -print-file-name=$$o; done)
END_OBJ := $(shell for o in crtend.o crtn.o; do ${CXX} -print-file-name=$$o; done)
STDLIBS = ${STLPORT_LIB} ${_LGCC_S} -l${PTHR} -lc -lm
endif
ifeq ($(OSNAME),netbsd)
START_OBJ := $(shell for o in crt{1,i,begin}.o; do ${CXX} -print-file-name=$$o; done)
END_OBJ := $(shell for o in crt{end,n}.o; do ${CXX} -print-file-name=$$o; done)
STDLIBS = ${STLPORT_LIB} ${_LGCC_S} -lpthread -lc -lm
endif
ifeq ($(OSNAME),sunos)
START_OBJ := $(shell for o in crt1.o crti.o crtbegin.o; do ${CXX} -print-file-name=$$o; done)
END_OBJ := $(shell for o in crtend.o crtn.o; do ${CXX} -print-file-name=$$o; done)
STDLIBS = ${STLPORT_LIB} ${_LGCC_S} -lpthread -lc -lm
endif
ifeq ($(OSNAME),darwin)
START_OBJ := -lcrt1.o -lcrt2.o
END_OBJ :=
STDLIBS = ${STLPORT_LIB} ${_LGCC_S} -lc -lm -lsupc++
#LDFLAGS += -dynamic
endif
LDFLAGS += -nostdlib
# endif
else
ifndef WITHOUT_STLPORT
STDLIBS = ${STLPORT_LIB}
else
STDLIBS =
endif
endif

# workaround for gcc 2.95.x bug:
ifeq ($(CXX_VERSION_MAJOR),2)
OPT += -fPIC
endif

ifeq ($(OSNAME),cygming)
dbg-static:     LDFLAGS += -static
stldbg-static:  LDFLAGS += -static
release-static: LDFLAGS += -static
endif

ifeq ($(OSNAME),windows)
dbg-static:     LDFLAGS += -static
stldbg-static:  LDFLAGS += -static
release-static: LDFLAGS += -static
endif
