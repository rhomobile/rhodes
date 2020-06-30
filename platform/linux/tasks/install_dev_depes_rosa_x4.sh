#!/bin/bash
sudo urpmi git gcc gcc-c++ ruby ruby-devel libgmp-devel lib64gl-devel lib64glu-devel rpm-build lib64gmp-devel
sudo urpmi qt5-devel lib64qt5webenginecore-devel lib64qt5webenginewidgets-devel lib64qt5multimedia-devel libglfw-devel
sudo urpmi lib64z-devel libopenssl-devel bison bison++

git clone https://github.com/ruby/ruby 
cd ruby
git checkout ruby_2_3
autoconf
./configure
make
sudo make install

sudo mv /usr/bin/ruby /usr/bin/ruby_backup
sudo ln -s /usr/local/bin/ruby /usr/bin/ruby

sudo mv /usr/bin/gem /usr/bin/gem_backup
sudo ln -s /usr/local/bin/gem /usr/bin/gem

grep -qxF 'export QTDIR=/usr/lib64/qt5' ~/.bashrc || echo 'export QTDIR=/usr/lib64/qt5' >> ~/.bashrc
sudo ln -s /bin/tar /usr/bin/tar