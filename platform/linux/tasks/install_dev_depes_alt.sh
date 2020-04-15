#!/bin/bash

sudo apt-get install git gcc gcc-c++ ruby libruby-devel irb gmp-devel libGL-devel libGLU-devel rpm-build zlib-devel

sudo apt-get install qt5-base-devel qt5-webengine-devel qt5-multimedia-devel libglfw libglfw-devel

grep -qxF 'export QTDIR=/usr/share/qt5' ~/.bashrc || echo 'export QTDIR=/usr/share/qt5' >> ~/.bashrc
