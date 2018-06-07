#!/usr/bin/env bash
set -x

cp ./rhobuild.yml.example ./rhobuild.yml

set -e


echo Downloading Qt
set -v
set -e

DOWNLOAD_URL=https://download.qt.io/archive/qt/5.9/5.9.5/qt-opensource-mac-x64-5.9.5.dmg
INSTALLER=`basename $DOWNLOAD_URL`
INSTALLER_NAME=${INSTALLER%.*}
ENVFILE=qt-5.9.5-osx.env
APPFILE=/Volumes/${INSTALLER_NAME}/${INSTALLER_NAME}.app/Contents/MacOS/${INSTALLER_NAME}
echo $INSTALLER_NAME
echo $APPFILE

wget -nv -c $DOWNLOAD_URL
hdiutil attach ${INSTALLER}
ls /Volumes
echo Installing Qt
extract-qt-installer $APPFILE $PWD/Qt
hdiutil detach /Volumes/${INSTALLER_NAME}

echo Create $ENVFILE
cat << EOF > $ENVFILE
export PATH=$PWD/Qt/5.9.5/clang_64/bin:$PATH
EOF
echo Qt installed

if [ $TRAVIS_BRANCH == "CI" ]
then
	BRANCH="master"
else
	BRANCH=$TRAVIS_BRANCH
fi

echo "Installing Rhoconnect client"
git clone -b master https://github.com/rhomobile/rhoconnect-client.git ../rhoconnect-client

git clone -b OpenSSL_1_1_0-stable https://github.com/tauplatform/openssl.git ../openssl
cd ../openssl
./tau_build_macos_lib.sh

echo "Building rhosim"
cd $TRAVIS_BUILD_DIR
rake build:osx:rhosimulator
# > build.log

OUT=$?

if [ $OUT -eq 0 ];then
   echo "RhoSimulator built successfully"
else
   echo "Error building RhoSimulator"
   cat build.log
fi

exit $OUT