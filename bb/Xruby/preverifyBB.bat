ruby compileERB.rb
java -jar xruby-0.3.3.jar -c RhoSrc
move RhoSrc.jar RhoBundle.jar
"%BB_JDE%\bin\preverify.exe" -classpath "%BB_JDE%\lib\net_rim_api.jar";"..\RubyVM\RubyVM.jar" -d ..\RhoBundle RhoBundle.jar
