#!/usr/bin/env bash
set -x

cp ./rhobuild.yml.example ./rhobuild.yml

sudo add-apt-repository --yes ppa:ubuntu-sdk-team/ppa
sudo apt-get update -qq
sudo apt-get install qtbase5-dev qtdeclarative5-dev libqt5webkit5-dev libsqlite3-dev
sudo apt-get install qt5-default qttools5-dev-tools
sudo apt-get install qtmultimedia5-dev libqt5multimediawidgets5 
sudo apt-get install libqt5multimedia5-plugins libqt5multimedia5 qtwebengine5-dev 
sudo apt-get install libx11-xcb-dev libglu1-mesa-dev libxrender-dev libfontconfig1-dev

export QTDIR=/usr/lib/x86_64-linux-gnu/qt5/bin/

set -e

exit 0