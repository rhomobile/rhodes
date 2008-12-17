rmdir RhoBundle /S /Q
xcopy "../../shared/framework/lib" RhoBundle /S /I /EXCLUDE:excludeFRMWK.txt
del "RhoBundle\date.rb"
del "RhoBundle\rho\rhofsconnectorME.rb"
xcopy "../../apps/Rhosugar" "RhoBundle/apps/Rhosugar" /S /I /EXCLUDE:excludeAPPS.txt
xcopy "../../apps/Sources" "RhoBundle/apps/Sources" /S /I /EXCLUDE:excludeAPPS.txt
xcopy "../../apps/*.erb" "RhoBundle/apps" /I /F
ruby compileERB.rb

java -jar lib/xruby-0.3.3.jar -c RhoBundle

rmdir apps /S /Q
xcopy "../../apps/*.html" "./apps" /I /F
xcopy "../../apps/Rhosugar/*.html" "./apps/Rhosugar" /I /F
xcopy "../../apps/Sources/*.html" "./apps/Sources" /I /F
xcopy "../../apps/app_manifest.txt" "./apps" /I /F
@REM xcopy "../../apps/shared" "apps/shared" /S /I

jar uf RhoBundle.jar apps/*.*

rmdir RhoBundle /S /Q
rmdir apps /S /Q