# Time-stamp: <05/09/09 21:07:10 ptr>
# $Id: targetsys.mak 1802 2005-11-01 08:25:57Z complement $

SO := nlm

ARCH := lib
AR := mwldnlm -type library -msgstyle gcc -w off
AR_INS_R := 
AR_EXTR :=
AR_OUT = -o $@

EXE := .nlm
