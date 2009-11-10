# -*- makefile -*- Time-stamp: <03/11/30 11:41:22 ptr>
# $Id: dmc.mak 2384 2006-05-30 20:25:17Z dums $

release-shared: OPT += -WD
dbg-shared: OPT += -WD
stldbg-shared: OPT += -WD

release-shared: LDFLAGS += /IMPLIB:$(subst /,\,$(OUTPUT_DIR))\$(SO_NAME_BASE).lib
dbg-shared: LDFLAGS += /CODEVIEW/IMPLIB:$(subst /,\,$(OUTPUT_DIR_DBG))\$(SO_NAME_DBG_BASE).lib
stldbg-shared: LDFLAGS += /CODEVIEW/IMPLIB:$(subst /,\,$(OUTPUT_DIR_STLDBG))\$(SO_NAME_STLDBG_BASE).lib

DEF_OPTION = $(subst /,\,$(OUTPUT_DIR))\$(SO_NAME_BASE).def
DEF_OPTION_DBG = $(subst /,\,$(OUTPUT_DIR_DBG))\$(SO_NAME_DBG_BASE).def
DEF_OPTION_STLDBG = $(subst /,\,$(OUTPUT_DIR_STLDBG))\$(SO_NAME_STLDBG_BASE).def
