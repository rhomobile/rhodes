if not defined WTK_HOME set WTK_HOME=c:\WTK2.5.2
set J2MEAPI_PATH=%WTK_HOME%\lib\cldcapi11.jar;%WTK_HOME%\lib\midpapi20.jar;%WTK_HOME%\lib\jsr75.jar
mkdir classes
javac -d classes -target 1.2 -source 1.2 -bootclasspath %J2MEAPI_PATH% -sourcepath .;jsr75;rms org\garret\perst\*.java org\garret\perst\impl\*.java jsr75\org\garret\perst\impl\*.java rms\org\garret\perst\*.java rms\org\garret\perst\impl\*.java
javac -d classes -target 1.2 -source 1.2 -sourcepath weak -classpath classes weak\org\garret\perst\impl\*.java
javac -d classes -target 1.2 -source 1.2 -sourcepath cdc -classpath classes cdc\org\garret\perst\impl\*.java
cd classes
jar cvf ../../lib/perst-jsr75.jar .
cd ..
rd /s/q classes