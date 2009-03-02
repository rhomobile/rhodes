set SAVE_PATH=%PATH%
set PATH=c:\jdk1.1.8\bin;%path%
mkdir classes
javac -d classes  org\garret\perst\*.java org\garret\perst\impl\*.java cdc\org\garret\perst\impl\*.java 
cd classes
jar cvf ../../lib/perst11.jar .
cd ..
rd /s/q classes
set PATH=%SAVE_PATH%