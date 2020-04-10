#usr/bash

sudo yum install git gcc gcc-c++ ruby ruby-devel ruby-irb gmp-devel mesa-libGL-devel mesa-libGLU-devel

sudo yum install qt5-qtbase qt5-devel qt5-qtwebengine-devel qt5-qtmultimedia-devel glfw glfw-devel

sudo gem install io-console
sudo gem install bigdecimal

#ruby-dev libxslt-dev libxml2-dev zlib1g-dev libz-dev libiconv-hook1 libiconv-hook-dev libqt5webenginewidgets5 
#libqt5webengine5 libqt5webenginecore5 libqt5multimedia5 libqt5multimediawidgets5 libqt5gui5 libqt5concurrent5 
#libqt5network5 qt5-default qt5-qmake qttools5-dev qtwebengine5-dev qtmultimedia5-dev 

#grep -qxF 'export QTDIR=/usr/lib64/qt5' ~/.bashrc || echo 'export QTDIR=/usr/lib64/qt5' >> ~/.bashrc
