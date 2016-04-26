@ECHO OFF

ATTRIB -R "..\..\R2D1\RhoElementsOneExtension\extensions\rhoelementsext\symbolapi\elements.rb"
del ..\..\R2D1\RhoElementsOneExtension\extensions\rhoelementsext\symbolapi\elements.rb
javac RubyGen.java
java RubyGen ../../EMMLTags/XML/ rubygen.xsl ../../R2D1/RhoElementsOneExtension/extensions/rhoelementsext/symbolapi/elements.rb
if not exist "../../R2D1/RhoElementsOneExtension/extensions/rhoelementsext/symbolapi/elements.rb" goto BuildFailed

goto EndOfScript

:BuildFailed
echo *******************************************
echo ***                                     ***
echo ***           BUILD FAILED!!!!          ***
echo ***  COULD NOT BUILD RUBY WRAPPER       ***
echo ***                                     ***
echo *******************************************
pause
exit
GOTO EndOfScript
:EndOfScript