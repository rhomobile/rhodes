#!/usr/bin/env bash
set -x

export QTDIR=/usr/lib/x86_64-linux-gnu/qt5/
rake run:linux:spec
