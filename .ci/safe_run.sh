#!/usr/bin/env bash

([ -x $1 ] && $1) || echo "Can't find step script $1. Skipping"
