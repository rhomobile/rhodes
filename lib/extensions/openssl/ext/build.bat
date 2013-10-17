if "%RHO_PLATFORM%" == "win32" (
call "%VS100COMNTOOLS%..\..\VC\vcvarsall.bat" x86
)

rake --trace