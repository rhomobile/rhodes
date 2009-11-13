# -*- Makefile -*- Time-stamp: <04/03/14 23:50:57 ptr>
# $Id: gcc.mak 1460 2005-04-18 21:37:58Z ptr $

SRCROOT := ../../src/explore
COMPILER_NAME := gcc

ALL_TAGS := compile-only
STLPORT_DIR := ../..
include Makefile.inc
include ${SRCROOT}/Makefiles/top.mak
compile-only:	$(OBJ)
