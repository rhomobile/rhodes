if not defined WTK_HOME set WTK_HOME=c:\WTK2.5.2
set J2MEAPI_PATH=%WTK_HOME%\lib\cldcapi11.jar;%WTK_HOME%\lib\midpapi20.jar;%WTK_HOME%\lib\jsr75.jar
mkdir classes
javac -d classes -target 1.2 -source 1.2 -classpath ../lib/perst-jsr75.jar;. -bootclasspath %J2MEAPI_PATH% -sourcepath fulltext fulltext\org\garret\perst\fulltext\*.java fulltext\org\garret\perst\impl\*.java 
cd classes
jar cvf ../../lib/fulltext-cldc11.jar .
cd ..
rd /s/q classes