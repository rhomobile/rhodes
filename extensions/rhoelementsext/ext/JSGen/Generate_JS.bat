@ECHO OFF

rem ATTRIB -R "..\..\..\..\extensions\rhoelementsext\public\elements.js"
rem del ..\..\..\..\extensions\rhoelementsext\public\elements.js
javac JSGen.java
java JSGen ../../../../neon/EMMLTags/XML/ jsgen.xsl ../../../../extensions/rhoelementsext/public/elements.js
if not exist "../../../../extensions/rhoelementsext/public/elements.js" goto BuildFailed

goto EndOfScript

:BuildFailed
echo *******************************************
echo ***                                     ***
echo ***           BUILD FAILED!!!!          ***
echo ***    COULD NOT BUILD JS WRAPPER       ***
echo ***                                     ***
echo *******************************************
pause
exit
GOTO EndOfScript
:EndOfScript
