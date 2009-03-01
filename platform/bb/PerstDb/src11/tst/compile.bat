set SAVE_PATH=%PATH%
set SAVE_CLASSPATH=%CLASSPATH%
set PATH=c:\jdk1.1.8\bin;%path%
set CLASSPATH=..\..\lib\perst11.jar
javac -g *.java
set PATH=%SAVE_PATH%
set CLASSPATH=%SAVE_CLASSPATH%