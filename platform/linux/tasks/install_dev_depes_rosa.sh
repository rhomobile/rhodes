#!/bin/bash

sudo urpmi git gcc gcc-c++ ruby ruby-devel libgmp-devel lib64gl-devel lib64glu-devel rpm-build lib64gmp-devel

sudo urpmi qt5-devel lib64qt5webenginecore-devel lib64qt5webenginewidgets-devel lib64qt5multimedia-devel libglfw-devel

grep -qxF 'export QTDIR=/usr/lib64/qt5' ~/.bashrc || echo 'export QTDIR=/usr/lib64/qt5' >> ~/.bashrc

#After that you should install manualy rvm (Ruby Version Manager) and set version >= 2.2 (for example 2.5.8).
#\curl -sSL https://get.rvm.io | bash
#source /home/user/.rvm/scripts/rvm 
#rvm install 2.5.8
#rvm --default use 2.5.8