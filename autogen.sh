#!/bin/sh
set -e

test -d m4 || mkdir m4

autoreconf -fi
