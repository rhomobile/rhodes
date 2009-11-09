# -*- makefile -*- Time-stamp: <05/12/07 01:01:49 ptr>

LDFLAGS += -type generic -w off -nostdlib -msgstyle gcc

STDLIBS = -L"$(NWSDK_DIR)/imports" \
          -L"$(MWCW_NOVELL)/Libraries/Runtime/Output/CLib" \
          -L"$(NWSDK_DIR)/../libc/imports" \
          -lclib.imp -lthreads.imp -lmwcrtl.lib -lnlmlib.imp -llibc.imp
