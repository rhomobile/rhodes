if "%RHO_PLATFORM%" == "win32" (
call "%VS110COMNTOOLS%..\..\VC\vcvarsall.bat" x86
)

rake --trace