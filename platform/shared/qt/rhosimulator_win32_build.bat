@echo off
call "%VS100COMNTOOLS%..\..\VC\vcvarsall.bat" x86
"%QTDIR%\bin\qmake" -o Makefile -r -spec win32-msvc2010 RhoSimulator.pro
nmake clean
nmake all
