#!/bin/sh
cd ../../shared/qt/rubylib
qmake -o Makefile -spec macx-g++ rubylib.pro
make all
cd ../rholib
qmake -o Makefile -spec macx-g++ rholib.pro
make all
# cd ../curl
# qmake -o Makefile -spec macx-g++ curl.pro
# make all
cd ../sqlite3
qmake -o Makefile -spec macx-g++ sqlite3.pro
make all
cd ../syncengine
qmake -o Makefile -spec macx-g++ syncengine.pro
make all
cd ../rhodes
qmake -o Makefile -spec macx-g++ RhoSimulator.pro
make all
cd ../../../osx/
