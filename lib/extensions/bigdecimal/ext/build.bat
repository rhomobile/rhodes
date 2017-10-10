if "%RHO_PLATFORM%" == "win32" (
call "%RHO_VSCMNTOOLS%..\..\VC\vcvarsall.bat" x86
)

rake --trace