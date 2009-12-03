XRuby
Xue Yong Zhi (zhixueyong AT hotmail.com)

Overview
=====================
XRuby is an open source Ruby compiler which compiles Ruby source code (.rb) 
to Java bytecode (.class). For more information, please visit our project 
home at http://code.google.com/p/xruby .


Compile
=====================
JDK 5.0 is required to compile XRuby's source code:

On Windows:
>build.bat

On Unix:
>./build.sh


Usage
=====================
* Compile a ruby script then run:
>java -jar xruby-0.3.3.jar -c test.rb
>java -jar test.jar

If xruby.jar and test.jar are not in the same folder, then should be:
>java -cp xruby-0.3.3.jar;test.jar xruby.test.main

* Run script directly (compile behind the scene):
>java -jar xruby-0.3.3.jar test.rb

Some ruby scripts require large amount of memory, if you see OutOfMemoryError,
try to increase the maximum heap size with -Xmx parameter:
>java -Xmx512m -jar xruby-0.3.3.jar test.rb


