@echo off
set GEN_DIR=..\..\..\res\build-tools
set ASSEMBLY_DIR=..\bin\RhoRubyExtGen\Debug

"%GEN_DIR%\ClassInitGenerator" "%ASSEMBLY_DIR%\RhoRubyExtGen.dll" /libraries:rho.rubyext /out:../RhoRubyLib/Initializers.Generated.cs


