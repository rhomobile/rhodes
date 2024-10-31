#!/bin/bash

sudo yum install git gcc gcc-c++ ruby ruby-devel ruby-irb gmp-devel mesa-libGL-devel mesa-libGLU-devel rpm-build qt5-qtbase qt5-devel qt5-qtwebengine-devel qt5-qtmultimedia-devel glfw glfw-devel annobin-plugin-gcc

sudo gem install io-console
sudo gem install bigdecimal

grep -qxF 'export QTDIR=/usr/lib64/qt5' ~/.bashrc || echo 'export QTDIR=/usr/lib64/qt5' >> ~/.bashrc

gem update rake