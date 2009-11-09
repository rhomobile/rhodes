# Time-stamp: <05/09/09 20:59:42 ptr>
# $Id: targetsys.mak 1802 2005-11-01 08:25:57Z complement $

SO := so

ARCH := a
ifeq ($(AR),)
  ifdef TARGET_OS
    AR := ${TARGET_OS}-ar
  else
    AR := ar
  endif
endif
AR_INS_R := -rs
AR_EXTR := -x
AR_OUT = $@
