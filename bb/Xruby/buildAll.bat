rmdir bin /S /Q
mkdir bin

"%BB_JDE%\bin\rapc.exe" -javacompiler="%JAVA_HOME%\bin\javac.exe" -quiet import="%BB_JDE%\lib\net_rim_api.jar" codename=bin\RhoApp RhoApp.rapc ..\PerstDb\blackberry_jde\PerstImplJSR75.jar ..\PerstDb\blackberry_jde\PerstLite.jar ..\RhoBundle\RhoBundle.jar ..\RubyVM\RubyVM.jar ..\rhodes\rhodes.jar
call signAll.bat
