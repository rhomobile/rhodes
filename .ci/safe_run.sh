#!/usr/bin/env bash

echo "Run if exists: $1"

if [ -x $1 ]
then
	$1
else
	echo "Script is not found. Do nothing."
fi
