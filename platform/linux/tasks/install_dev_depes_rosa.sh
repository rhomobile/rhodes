#!/bin/bash

sudo urpmi git gcc gcc-c++ ruby ruby-devel libgmp-devel lib64gl-devel lib64glu-devel rpm-build

sudo urpmi qt5-devel lib64qt5webenginecore-devel lib64qt5webenginewidgets-devel lib64qt5multimedia-devel libglfw-devel

grep -qxF 'export QTDIR=/usr/lib64/qt5' ~/.bashrc || echo 'export QTDIR=/usr/lib64/qt5' >> ~/.bashrc
