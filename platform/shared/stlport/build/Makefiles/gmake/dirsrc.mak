# -*- Makefile -*- Time-stamp: <03/10/27 18:07:14 ptr>
# $Id: dirsrc.mak 1802 2005-11-01 08:25:57Z complement $

# Some trick to build implicit rules for sources in some different
# subdirectories. I remove catalogs from path to sources, with this
# names I build output object path, and provide dependency from
# source in directory. Due to no loops for rules definition, I should
# use recursion here;

# try take directory from list:
WORD1 := $(word 1,$(DIRS_UNIQUE_SRC))

# is still directory in the list?
ifneq "$(WORD1)" ""
include ${RULESBASE}/rules-o.mak
include ${RULESBASE}/rules-d.mak
ifeq ($(OSNAME),cygming)
include ${RULESBASE}/rules-res.mak
endif
ifeq ($(OSNAME),windows)
include ${RULESBASE}/rules-res.mak
endif
# remove processed directory from list
DIRS_UNIQUE_SRC := $(filter-out $(WORD1),$(DIRS_UNIQUE_SRC))
# recursive include here:
include ${RULESBASE}/${USE_MAKE}/dirsrc.mak
endif
