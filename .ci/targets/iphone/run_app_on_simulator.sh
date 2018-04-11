#!/usr/bin/env bash
set -x
ulimit -n 1024
rake run:iphone:spec
