@ECHO OFF
REM **************************************************************************
REM *
REM * configure.bat for setting up compiling STLport under Windows
REM * to see available options, call with option --help
REM *
REM * Copyright (C) 2004,2005 Michael Fink
REM *
REM **************************************************************************

REM Attention! Batch file labels only have 8 significant characters!

echo STLport Configuration Tool for Windows
echo.

REM no options at all?
if NOT "%1xyz123" == "xyz123" goto init

echo Please specify some options or use "configure --help" to see the
echo available options.
goto skp_comp

:init

REM initially create/overwrite config.mak
echo # STLport Configuration Tool for Windows > ..\Makefiles\config.mak
echo # >> ..\Makefiles\config.mak
echo # config.mak generated with command line: >> ..\Makefiles\config.mak
echo # configure %1 %2 %3 %4 %5 %6 %7 %8 %9 >> ..\Makefiles\config.mak
echo # >> ..\Makefiles\config.mak

REM
REM option loop
REM
:loop

REM help option
if "%1" == "-?" goto opt_help
if "%1" == "-h" goto opt_help
if "%1" == "/?" goto opt_help
if "%1" == "/h" goto opt_help
if "%1" == "--help" goto opt_help

REM compiler option
if "%1" == "-c" goto opt_comp
if "%1" == "/c" goto opt_comp
if "%1" == "--compiler" goto opt_comp

REM cross compiling
if "%1" == "-x" goto opt_x
if "%1" == "/x" goto opt_x
if "%1" == "--cross" goto opt_x

REM C runtime library
if "%1" == "--rtl-static" goto opt_rtl
if "%1" == "--rtl-dynamic" goto opt_rtl

REM boost support
if "%1" == "--use-boost" goto opt_bst

REM multithreading support
if "%1" == "--not-thread-safe" goto opt_st

REM rtti support
if "%1" == "--no-rtti" goto opt_rtti

REM additional compiler options
if "%1" == "--extra-cxxflag" goto opt_xtra

REM library name customization
if "%1" == "--lib-motif" goto opt_motf

REM clean rule
if "%1" == "--clean" goto opt_cln

echo Unknown option: %1

:cont_lp
echo.
shift

REM no more options?
if "%1xyz123" == "xyz123" goto end_loop

goto loop


REM **************************************************************************
REM *
REM * Help
REM *
REM **************************************************************************
:opt_help
echo The following options are available:
echo.
echo "-c <compiler>" or "--compiler <compiler>"
echo    Uses specified compiler to compile STLport. The following keywords
echo    are available:
echo    msvc6    Microsoft Visual C++ 6.0
echo    msvc7    Microsoft Visual C++ .NET 2002
echo    msvc71   Microsoft Visual C++ .NET 2003
echo    msvc8    Microsoft Visual C++ 2005
echo    icl      Intel C++ Compiler
echo    evc3     Microsoft eMbedded Visual C++ 3 (*)
echo    evc4     Microsoft eMbedded Visual C++ .NET (*)
echo    evc8     Microsoft Visual C++ 2005 compiling for CE
echo    gcc      GNU C++ Compiler (MinGW package)
echo    dmc      Digital Mars Compiler
echo    bcc      Borland C++ Compiler
echo  (*) For these compilers the target processor is determined automatically.
echo      You must run the WCE*.BAT file you wish to build STLport for before
echo      running configure.
echo.
echo "-x"
echo    Enables cross-compiling; the result is that all built files that are
echo    normally put under "bin" and "lib" get extra subfolders depending on
echo    the compiler name.
echo.
echo "--rtl-static"
echo "--rtl-dynamic"
echo    Enables usage of static (libc.lib family) or dynamic (msvcrt.lib family)
echo    C/C++ runtime library when linking with STLport. If you want your appli/dll
echo    to link statically with STLport but using the dynamic C runtime use
echo    --rtl-dynamic; if you want to link dynamicaly with STLport but using the
echo    static C runtime use --rtl-static. See README.options for details.
echo    Don't forget to signal the link method when building your appli or dll, in
echo    stlport/stl/config/host.h set the following macro depending on the configure
echo    option:
echo    "--rtl-dynamic -> _STLP_USE_STATIC_LIB"
echo    "--rtl-static  -> _STLP_USE_DYNAMIC_LIB"
echo    This is a Microsoft Visual Studio only option.
echo.
echo "--use-boost <boost install path>"
echo    Request use of boost support (www.boost.org). For the moment only the boost
echo    type_traits library is used to get type information and to implement some
echo    specific workaround not directly implemented by STLport. To use the same
echo    support using STLport don't forget to define _STLP_USE_BOOST_SUPPORT in
echo    stlport/stl/config/user_config.h file.
echo.
echo "--not-thread-safe"
echo    Per default STLport libraries are built in order to be usable in a multithreaded
echo    context. If you don't need this you can ask for a not thread safe version with
echo    this option.
echo.
echo "--no-rtti"
echo    Remove rtti (run time type information) support if available.
echo.
echo "--extra-cxxflag <additional compilation options>"
echo    Use this option to add any compilation flag to the build system. For instance
echo    it can be used to activate a specific processor optimization depending on your
echo    processor. For Visual C++ .Net 2003, to activate pentium 3 optim you will use:
echo    --extra-cxxflag /G7
echo    If you have several options use several --extra-cxxflag options. For instance
echo    to also force use of wchar_t as an intrinsic type:
echo    --extra-cxxflag /G7 --extra-cxxflag /Zc:wchar_t
echo.
echo "--lib-motif <motif>"
echo   Use this option to customize the generated library name. The motif will be used
echo   in the last place before version information, separated by an underscore, ex:
echo   stlportd_MOTIF.5.0.lib
echo   stlportstld_static_MOTIF.5.1.lib
echo   Do not forget to define _STLP_LIB_NAME_MOTIF macro in STLport configuration file
echo   to the same value if you want to keep the auto link feature.
echo.
echo "--clean"
echo    Removes the build configuration file.
goto skp_comp

REM **************************************************************************
REM *
REM * Compiler configuration
REM *
REM **************************************************************************
:opt_comp

if "%2" == "msvc6" goto oc_msvc6
if "%2" == "msvc71" goto oc_msv71
if "%2" == "msvc7" goto oc_msvc7
if "%2" == "msvc8" goto oc_msvc8
if "%2" == "icl"   goto oc_icl

if "%2" == "evc3" goto oc_evc3
if "%2" == "evc4" goto oc_evc4
if "%2" == "evc8" goto oc_evc8

if "%2" == "gcc" goto oc_gcc
if "%2" == "dmc" goto oc_dmc
if "%2" == "bcc" goto oc_bcc

echo Unknown compiler: %2
goto oc_end

:oc_msvc6
echo Setting compiler: Microsoft Visual C++ 6.0
echo COMPILER_NAME=vc6 >> ..\Makefiles\config.mak
set SELECTED_COMPILER_VERSION=60
goto oc_msvc

:oc_msvc7
echo Setting compiler: Microsoft Visual C++ .NET 2002
echo COMPILER_NAME=vc70 >> ..\Makefiles\config.mak
set SELECTED_COMPILER_VERSION=70
goto oc_msvc

:oc_msv71
echo Setting compiler: Microsoft Visual C++ .NET 2003
echo COMPILER_NAME=vc71 >> ..\Makefiles\config.mak
set SELECTED_COMPILER_VERSION=71
goto oc_msvc

:oc_msvc8
echo Setting compiler: Microsoft Visual C++ 2005
echo COMPILER_NAME=vc8 >> ..\Makefiles\config.mak
set SELECTED_COMPILER_VERSION=80
goto oc_msvc

:oc_msvc
echo TARGET_OS=x86 >> ..\Makefiles\config.mak
set STLPORT_COMPILE_COMMAND=nmake /fmsvc.mak
set SELECTED_COMPILER=msvc
goto oc_end

:oc_icl
echo Setting compiler: Intel C++ Compiler
echo COMPILER_NAME=icl >> ..\Makefiles\config.mak
echo TARGET_OS=x86 >> ..\Makefiles\config.mak
set STLPORT_COMPILE_COMMAND=nmake /ficl.mak
set SELECTED_COMPILER=icl
goto oc_end

:oc_evc3
echo Setting compiler: Microsoft eMbedded Visual C++ 3
echo COMPILER_NAME=evc3 >> ..\Makefiles\config.mak
rem TODO: branch on OSVERSION like below?
echo CEVERSION=300 >> ..\Makefiles\config.mak
set SELECTED_COMPILER_VERSION=3
goto oc_evc

:oc_evc4
echo Setting compiler: Microsoft eMbedded Visual C++ .NET
echo COMPILER_NAME=evc4 >> ..\Makefiles\config.mak
if "%OSVERSION%"=="" (
    echo OSVERSION not set, assuming target is CE 4.2
    echo CEVERSION=420 >> ..\Makefiles\config.mak
) else if "%OSVERSION%"=="WCE400" (
    echo CEVERSION=400 >> ..\Makefiles\config.mak
) else if "%OSVERSION%"=="WCE420" (
    echo CEVERSION=420 >> ..\Makefiles\config.mak
) else if "%OSVERSION%"=="WCE500" (
    echo CEVERSION=500 >> ..\Makefiles\config.mak
) else (
    echo Unknown value for OSVERSION.
    exit /b 1
)
set SELECTED_COMPILER_VERSION=4
goto oc_evc

:oc_evc8
echo Setting compiler: Microsoft Visual C++ .NET 2005 for Windows CE
echo COMPILER_NAME=evc8 >> ..\Makefiles\config.mak
set SELECTED_COMPILER_VERSION=80
if "%OSVERSION%"=="" (
    echo OSVERSION not set, assuming target is CE 5.0
    echo CEVERSION=500 >> ..\Makefiles\config.mak
) else if "%OSVERSION%"=="WCE400" (
    echo CEVERSION=400 >> ..\Makefiles\config.mak
) else if "%OSVERSION%"=="WCE420" (
    echo CEVERSION=420 >> ..\Makefiles\config.mak
) else if "%OSVERSION%"=="WCE500" (
    echo CEVERSION=500 >> ..\Makefiles\config.mak
) else (
    echo Unknown value for OSVERSION.
    exit /b 1
)
set STLPORT_COMPILE_COMMAND=nmake /fevc.mak
set SELECTED_COMPILER=msvc
goto proc

:oc_evc
set STLPORT_COMPILE_COMMAND=nmake /fevc.mak
set SELECTED_COMPILER=evc
goto proc

:oc_gcc
echo Setting compiler: GNU C++ Compiler
set STLPORT_COMPILE_COMMAND=make -fgcc.mak
set SELECTED_COMPILER=gcc
shift
goto oc_gmake

:oc_dmc
echo Setting compiler: Digital Mars C++ Compiler
set STLPORT_COMPILE_COMMAND=make -fdmc.mak
set SELECTED_COMPILER=dmc
shift
goto oc_gmake

:oc_bcc
echo Setting compiler: Borland C++ Compiler
set STLPORT_COMPILE_COMMAND=make -fbcc.mak
set SELECTED_COMPILER=bcc
shift
goto oc_gmake

:oc_gmake
echo In order to build STLport with this compiler you need a GNU make tool.
echo You can get one from www.mingw.org or www.cygwin.com
echo Setting up for building using GNU make.
echo include $(SRCROOT)\Makefiles\gmake\windows\sysid.mak >> ..\Makefiles\config.mak
goto cont_lp

:oc_end
shift

goto cont_lp


REM **************************************************************************
REM *
REM * Target processor configuration (automatic)
REM *
REM **************************************************************************
:proc

if "%TARGETCPU%" == "ARM" goto pr_arm
if "%TARGETCPU%" == "ARMV4" goto pr_arm
if "%TARGETCPU%" == "ARMV4I" goto pr_arm
if "%TARGETCPU%" == "ARMV4T" goto pr_arm

if "%TARGETCPU%" == "X86" goto pr_x86
REM Type from evc3 and/or PocketPC 2002 SDK reported here
REM to correctly check the platform:
if "%TARGETCPU%" == "X86EMnset CFG=none" goto pr_emul
if "%TARGETCPU%" == "x86" goto pr_x86
if "%TARGETCPU%" == "emulator" goto pr_emul

if "%TARGETCPU%" == "R4100" goto pr_mips
if "%TARGETCPU%" == "R4111" goto pr_mips
if "%TARGETCPU%" == "R4300" goto pr_mips
if "%TARGETCPU%" == "MIPS16" goto pr_mips
if "%TARGETCPU%" == "MIPSII" goto pr_mips
if "%TARGETCPU%" == "MIPSII_FP" goto pr_mips
if "%TARGETCPU%" == "MIPSIV" goto pr_mips
if "%TARGETCPU%" == "MIPSIV_FP" goto pr_mips

if "%TARGETCPU%" == "SH3" goto pr_sh3
if "%TARGETCPU%" == "SH4" goto pr_sh4

:pr_err
echo Unknown target CPU: %TARGETCPU%
goto pr_end

:pr_arm
echo Target processor: ARM
echo TARGET_PROC=arm >> ..\Makefiles\config.mak
echo TARGET_PROC_SUBTYPE=%TARGETCPU% >> ..\Makefiles\config.mak
goto pr_end

:pr_x86
echo Target processor: x86
echo TARGET_PROC=x86 >> ..\Makefiles\config.mak
goto pr_end

:pr_emul
echo Target processor: Emulator
echo TARGET_PROC=x86 >> ..\Makefiles\config.mak
echo TARGET_PROC_SUBTYPE=emulator >> ..\Makefiles\config.mak
goto pr_end

:pr_mips
echo Target processor: MIPS
echo TARGET_PROC=mips >> ..\Makefiles\config.mak
echo TARGET_PROC_SUBTYPE=%TARGETCPU% >> ..\Makefiles\config.mak

goto pr_end

:pr_sh3
echo Target processor: %TARGETCPU%
echo TARGET_PROC=sh3 >> ..\Makefiles\config.mak
goto pr_end

:pr_sh4
echo Target processor: %TARGETCPU%
echo TARGET_PROC=sh4 >> ..\Makefiles\config.mak
goto pr_end

:pr_end
goto oc_end


REM **************************************************************************
REM *
REM * Cross Compiling option
REM *
REM **************************************************************************

:opt_x
echo Setting up for cross compiling.
echo CROSS_COMPILING=1 >> ..\Makefiles\config.mak
goto cont_lp


REM **************************************************************************
REM *
REM * C runtime library selection
REM *
REM **************************************************************************

:opt_rtl
if "%SELECTED_COMPILER%" == "" goto or_err1
if "%SELECTED_COMPILER%" == "msvc" goto or_ok
if "%SELECTED_COMPILER%" == "bcc" goto or_ok
if "%SELECTED_COMPILER%" == "dmc" goto or_ok
goto or_err2

:or_err1
echo Error: Please give used compiler first in order to check other options correctness.
goto or_end

:or_err2
echo Error: Setting C runtime library for compiler other than microsoft ones!
goto or_end

:or_ok

if "%1" == "--rtl-static" echo Selecting static C runtime library for STLport
if "%1" == "--rtl-static" echo STLP_BUILD_FORCE_STATIC_RUNTIME=1 >> ..\Makefiles\config.mak

if "%1" == "--rtl-dynamic" echo Selecting dynamic C runtime library for STLport
if "%1" == "--rtl-dynamic" echo STLP_BUILD_FORCE_DYNAMIC_RUNTIME=1 >> ..\Makefiles\config.mak

:or_end
goto cont_lp

REM **************************************************************************
REM *
REM * boost support
REM *
REM **************************************************************************
:opt_bst
REM if (Exists("%2")) goto ob_ok
REM if !("%2" == "") goto ob_ok
goto ob_ok

echo Error: Invalid boost intallation folder ("%2").
goto ob_end

:ob_ok
echo Activating boost support using "%2" path
echo STLP_BUILD_BOOST_PATH="%2" >> ..\Makefiles\config.mak

:ob_end
shift

goto cont_lp

REM **************************************************************************
REM *
REM * Multithreading support
REM *
REM **************************************************************************
:opt_st
echo Removing thread safety support
echo STLP_BUILD_NO_THREAD=1 >> ..\Makefiles\config.mak
goto cont_lp

REM **************************************************************************
REM *
REM * rtti support
REM *
REM **************************************************************************
:opt_rtti
echo Removing rtti support
echo STLP_BUILD_NO_RTTI=1 >> ..\Makefiles\config.mak
goto cont_lp

REM **************************************************************************
REM *
REM * Extra compilation flags
REM *
REM **************************************************************************
:opt_xtra
echo Adding '%2' compilation option
if "%ONE_OPTION_ADDED%" == "1" goto ox_n

echo DEFS = %2 >> ..\Makefiles\config.mak
set ONE_OPTION_ADDED=1
goto ox_end

:ox_n
echo DEFS = $(DEFS) %2 >> ..\Makefiles\config.mak

:ox_end
shift
goto cont_lp

REM **************************************************************************
REM *
REM * Library name configuration
REM *
REM **************************************************************************
:opt_motf
echo Using '%2' in generated library names

echo STLP_BUILD_LIB_MOTIF = %2 >> ..\Makefiles\config.mak

shift
goto cont_lp

REM **************************************************************************
REM *
REM * Clean
REM *
REM **************************************************************************
:opt_cln
del ..\Makefiles\config.mak
echo STLport configuration file removed.
goto skp_comp

REM **************************************************************************
REM *
REM * End loop
REM *
REM **************************************************************************

:end_loop

echo Done configuring STLport.
echo.

if "%STLPORT_COMPILE_COMMAND%" == "" goto skp_comp
echo Please type "%STLPORT_COMPILE_COMMAND%" to build STLport.
echo Type "%STLPORT_COMPILE_COMMAND% install" to install STLport to the "lib"
echo and "bin" folder when done.
echo.

:skp_comp
set STLPORT_COMPILE_COMMAND=
set SELECTED_COMPILER=
set SELECTED_COMPILER_VERSION=
set ONE_OPTION_ADDED=
