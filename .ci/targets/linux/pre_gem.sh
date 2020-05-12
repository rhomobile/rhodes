#!/usr/bin/env bash
set -x

cp ./rhobuild.yml.example ./rhobuild.yml

#sudo add-apt-repository --yes ppa:ubuntu-sdk-team/ppa
sudo apt-get update -qq

sudo apt-get install qt5-default qtbase5-dev qtdeclarative5-dev qttools5-dev-tools
sudo apt-get install qtmultimedia5-dev libqt5multimediawidgets5 
sudo apt-get install libqt5multimedia5-plugins qtwebengine5-dev 

sudo find / -name qmake

set -e

exit 0