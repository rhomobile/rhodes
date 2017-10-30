#!/usr/bin/env bash

rake build:rhosimulator
OUT=$?

git clean -fdx

if [ $OUT -eq 0 ];then
   echo "RhoSimulator built successfully"
else
   echo "Error building RhoSimulator"
fi

exit $OUT