@echo off
rem five zeroes are default values for missing subversions
call "%~dp0scripts\BuildRCFiles.bat" %* 0 0 0 0 0
call "%~dp0scripts\BuildPlugins.bat"
