mkdir classes
javac -d classes util\*.java 
cd classes
jar cvf ../../lib/sergen.jar .
cd ..
rd /s/q classes
