#!/usr/bin/env bash
set -x

cp ./rhobuild.yml.example ./rhobuild.yml

#sudo add-apt-repository --yes ppa:ubuntu-sdk-team/ppa
sudo apt-get update -qq

sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -qq
if [ "$CXX" = "g++" ]; then sudo apt-get install -qq g++-4.9; fi
if [ "$CXX" = "g++" ]; then export CXX="g++-4.9" CC="gcc-4.9"; fi

sudo apt-get install qt5-default qtbase5-dev qtdeclarative5-dev qttools5-dev-tools
sudo apt-get install qtmultimedia5-dev libqt5multimediawidgets5 
sudo apt-get install libqt5multimedia5-plugins qtwebengine5-dev 

sudo find / -name qmake

set -e

exit 0