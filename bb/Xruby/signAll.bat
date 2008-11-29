rmdir bin /S /Q
mkdir bin
xcopy "../RubyVM/RubyVM.cod" "./bin" /I /F
xcopy "../RubyVM/RubyVM.alx" "./bin" /I /F
xcopy "../RubyVM/RubyVM.cso" "./bin" /I /F
xcopy "../RubyVM/RubyVM.csl" "./bin" /I /F

xcopy "../rhodes/rhodes.cod" "./bin" /I /F
xcopy "../rhodes/rhodes.alx" "./bin" /I /F
xcopy "../rhodes/rhodes.cso" "./bin" /I /F
xcopy "../rhodes/rhodes.csl" "./bin" /I /F

xcopy "../RhoBundle/RhoBundle.cod" "./bin" /I /F
xcopy "../RhoBundle/RhoBundle.alx" "./bin" /I /F
xcopy "../RhoBundle/RhoBundle.cso" "./bin" /I /F
xcopy "../RhoBundle/RhoBundle.csl" "./bin" /I /F

xcopy "../PerstDb/blackberry_jde/PerstImplJSR75.cod" "./bin" /I /F
xcopy "../PerstDb/blackberry_jde/PerstImplJSR75.alx" "./bin" /I /F
xcopy "../PerstDb/blackberry_jde/PerstImplJSR75.cso" "./bin" /I /F
xcopy "../PerstDb/blackberry_jde/PerstImplJSR75.csl" "./bin" /I /F

xcopy "../PerstDb/blackberry_jde/PerstLite.cod" "./bin" /I /F
xcopy "../PerstDb/blackberry_jde/PerstLite.alx" "./bin" /I /F
xcopy "../PerstDb/blackberry_jde/PerstLite.cso" "./bin" /I /F
xcopy "../PerstDb/blackberry_jde/PerstLite.csl" "./bin" /I /F

cd bin
java -jar "%BB_JDE%\bin\SignatureTool.jar" RubyVM.cod PerstLite.cod PerstImplJSR75.cod rhodes.cod RhoBundle.cod
cd ..
