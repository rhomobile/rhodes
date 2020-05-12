#!/usr/bin/env bash
set -x

cp ./rhobuild.yml.example ./rhobuild.yml

set -e

echo Downloading Qt

wget -q https://s3.amazonaws.com/files.tau-technologies.com/buildenv/Qt5.13.2_mini.tar.gz -O $HOME/Qt5.13.2.tar.gz
tar -xzf $HOME/Qt5.13.2.tar.gz -C $HOME/
echo Qt installed

echo "Installing Rhoconnect client"	
git clone -b master https://github.com/rhomobile/rhoconnect-client.git ../rhoconnect-client

git clone -b OpenSSL_1_1_0-stable https://github.com/tauplatform/openssl.git ../openssl
cd ../openssl
./tau_build_macos_lib.sh
