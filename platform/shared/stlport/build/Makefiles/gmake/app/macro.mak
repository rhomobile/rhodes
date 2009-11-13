# Time-stamp: <05/12/07 01:01:38 ptr>

ifneq ($(OSNAME),windows)
PRG        := $(OUTPUT_DIR)/${PRGNAME}${EXE}
PRG_DBG    := $(OUTPUT_DIR_DBG)/${PRGNAME}${EXE}
PRG_STLDBG := $(OUTPUT_DIR_STLDBG)/${PRGNAME}${EXE}
else
PRG        := $(subst /,\,$(OUTPUT_DIR)/${PRGNAME}${EXE})
PRG_DBG    := $(subst /,\,$(OUTPUT_DIR_DBG)/${PRGNAME}${EXE})
PRG_STLDBG := $(subst /,\,$(OUTPUT_DIR_STLDBG)/${PRGNAME}${EXE})
endif

LDFLAGS += ${LDSEARCH}
