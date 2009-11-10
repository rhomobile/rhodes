# Time-stamp: <05/12/13 23:24:02 ptr>

ifdef TARGET_OS
TARGET_NAME := ${TARGET_OS}-
else
TARGET_NAME :=
endif

ifeq ($(PRE_OUTPUT_DIR),)
  PRE_OUTPUT_DIR         := obj/$(TARGET_NAME)$(COMPILER_NAME)
endif
OUTPUT_DIR             := $(PRE_OUTPUT_DIR)/so$(EXTRA_DIRS)
OUTPUT_DIR_DBG         := $(PRE_OUTPUT_DIR)/so_g$(EXTRA_DIRS)
OUTPUT_DIR_STLDBG      := $(PRE_OUTPUT_DIR)/so_stlg$(EXTRA_DIRS)

# file to store generated dependencies for make:
DEPENDS_COLLECTION     := obj/$(TARGET_NAME)$(COMPILER_NAME)/.make.depend

# I use the same catalog, as for shared:
ifneq (cygming, $(OSNAME))
ifneq (windows, $(OSNAME))
OUTPUT_DIR_A           := $(OUTPUT_DIR)
OUTPUT_DIR_A_DBG       := $(OUTPUT_DIR_DBG)
OUTPUT_DIR_A_STLDBG    := $(OUTPUT_DIR_STLDBG)
endif
endif

OUTPUT_DIR_A           ?= $(PRE_OUTPUT_DIR)/ar$(EXTRA_DIRS)
OUTPUT_DIR_A_DBG       ?= $(PRE_OUTPUT_DIR)/ar_g$(EXTRA_DIRS)
OUTPUT_DIR_A_STLDBG    ?= $(PRE_OUTPUT_DIR)/ar_stlg$(EXTRA_DIRS)

INSTALL_LIB_DIR        ?= ${SRCROOT}/../$(TARGET_NAME)lib
INSTALL_LIB_DIR_DBG    ?= ${SRCROOT}/../$(TARGET_NAME)lib
INSTALL_LIB_DIR_STLDBG ?= ${SRCROOT}/../$(TARGET_NAME)lib
INSTALL_BIN_DIR        ?= ${SRCROOT}/../$(TARGET_NAME)bin
# install catalog will be same in case of cross-compilation or under
# CygWin or MinGW environments; but you can specify install dir you
# want---if one is defined it will not be overlaped.
ifeq ("${TARGET_NAME}","")
#ifneq (${OSNAME},cygming)
#ifneq ($(OSNAME),windows)
INSTALL_BIN_DIR_DBG    ?= ${SRCROOT}/../$(TARGET_NAME)bin-g
INSTALL_BIN_DIR_STLDBG ?= ${SRCROOT}/../$(TARGET_NAME)bin-stlg
#endif
#endif
endif
ifndef INSTALL_BIN_DIR_DBG
INSTALL_BIN_DIR_DBG    ?= ${INSTALL_BIN_DIR}
INSTALL_BIN_DIR_STLDBG ?= ${INSTALL_BIN_DIR}
endif

OUTPUT_DIRS := $(OUTPUT_DIR) $(OUTPUT_DIR_DBG) $(OUTPUT_DIR_STLDBG) \
               $(OUTPUT_DIR_A) $(OUTPUT_DIR_A_DBG) $(OUTPUT_DIR_A_STLDBG)

INSTALL_LIB_DIRS := $(INSTALL_LIB_DIR) $(INSTALL_LIB_DIR_DBG) $(INSTALL_LIB_DIR_STLDBG)
INSTALL_BIN_DIRS := $(INSTALL_BIN_DIR) $(INSTALL_BIN_DIR_DBG) $(INSTALL_BIN_DIR_STLDBG)

# sort will remove duplicates:
OUTPUT_DIRS := $(sort $(OUTPUT_DIRS))
INSTALL_LIB_DIRS := $(sort $(INSTALL_LIB_DIRS))
INSTALL_BIN_DIRS := $(sort $(INSTALL_BIN_DIRS))
INSTALL_DIRS := $(sort $(INSTALL_LIB_DIRS) $(INSTALL_BIN_DIRS))

PHONY += $(OUTPUT_DIRS) $(INSTALL_DIRS)

ifneq (${OSNAME},windows)
$(OUTPUT_DIRS):
	@for d in $@ ; do \
	  if ${EXT_TEST} -e $$d -a -f $$d ; then \
	    echo "ERROR: Regular file $$d present, directory instead expected" ; \
	    exit 1; \
	  elif [ ! -d $$d ] ; then \
	    mkdir -p $$d ; \
	  fi ; \
	done

$(INSTALL_DIRS):
	@for d in $@ ; do \
	  if ${EXT_TEST} -e $$d -a -f $$d ; then \
	    echo "ERROR: Regular file $$d present, directory instead expected" ; \
	    exit 1; \
	  elif [ ! -d $$d ] ; then \
	    mkdir -p $$d ; \
	  fi ; \
	done
else
$(OUTPUT_DIRS):
	@if not exist $@ mkdir $(subst /,\,$@)

$(INSTALL_DIRS):
	@if not exist $@ mkdir $(subst /,\,$@)
endif
