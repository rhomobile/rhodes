call compileApp.bat

@IF "%1" == "" (
 @set RubyVMDir=..\RubyVM
 @set BundleDir=..\RhoBundle
) ELSE (
 @set RubyVMDir=%1
 @set BundleDir=%1 
)

"%BB_JDE%\bin\preverify.exe" -classpath "%BB_JDE%\lib\net_rim_api.jar";"%RubyVMDir%\RubyVM.jar" -d %BundleDir% RhoBundle.jar
