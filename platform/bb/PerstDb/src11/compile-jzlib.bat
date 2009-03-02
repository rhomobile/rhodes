if not defined WTK_HOME set WTK_HOME=c:\WTK2.5.2
set J2MEAPI_PATH=%WTK_HOME%\lib\cldcapi11.jar;%WTK_HOME%\lib\midpapi20.jar
mkdir classes
javac -target 1.2 -source 1.2 -d classes -bootclasspath %J2MEAPI_PATH% com\jcraft\jzlib\*.java
cd classes
jar cvf ../../lib/jzlib-midp.jar .
cd ..
rd /s/q classes