# -*- Makefile -*- Time-stamp: <03/10/12 20:35:49 ptr>
# $Id: CC.mak 1848 2005-11-18 10:30:37Z complement $

SRCROOT := ..
COMPILER_NAME := CC

STLPORT_ETC_DIR = ../../etc
STLPORT_INCLUDE_DIR = ../../stlport
include Makefile.inc
include ${SRCROOT}/Makefiles/top.mak

INCLUDES += -I. -I$(STLPORT_INCLUDE_DIR)

# options for build with boost support
ifdef STLP_BUILD_BOOST_PATH
INCLUDES += -I$(STLP_BUILD_BOOST_PATH)
endif

