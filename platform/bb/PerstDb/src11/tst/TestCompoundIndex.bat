set SAVE_PATH=%PATH%
set PATH=c:\jdk1.1.8\bin;%path%
set SAVE_CLASSPATH=%CLASSPATH%
set CLASSPATH=..\..\lib\perst11.jar;.
java -mx512M TestCompoundIndex %1 %2 %3
set PATH=%SAVE_PATH%
set CLASSPATH=%SAVE_CLASSPATH%