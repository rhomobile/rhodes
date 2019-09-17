#!/usr/bin/env bash
set -x

export QTDIR=/usr/lib/x86_64-linux-gnu/qt5/
export QT_QPA_PLATFORM=offscreen
rake run:linux:spec
