rmdir RhoBundle /S /Q
xcopy "../../shared/framework/lib" RhoBundle /S /I /EXCLUDE:excludeFRMWK.txt
del "RhoBundle\date.rb"
del "RhoBundle\rho\rhofsconnector.rb"
xcopy "../../apps/Rhosugar" "RhoBundle/apps/Rhosugar" /S /I /EXCLUDE:excludeAPPS.txt
ruby compileERB.rb

java -jar xruby-0.3.3.jar -c RhoBundle

rmdir apps /S /Q
xcopy "../../apps/*.html" "./apps" /I /F
xcopy "../../apps/Rhosugar/*.html" "./apps/Rhosugar" /I /F
REM xcopy "../../apps/shared" "apps/shared" /S /I

jar uf RhoBundle.jar apps/*.*

"%BB_JDE%\bin\preverify.exe" -classpath "%BB_JDE%\lib\net_rim_api.jar";"..\RubyVM\RubyVM.jar" -d ..\RhoBundle RhoBundle.jar
rmdir RhoBundle /S /Q
rmdir apps /S /Q