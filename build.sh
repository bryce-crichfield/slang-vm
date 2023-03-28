#!/usr/bin/bash

clear

SOURCES=$(find . -name "*.c")
LIBS="-lm"
CFLAGS="-Wall -Werror -O3"

set -xe 

clang $SOURCES -o exe $LIBS $CFLAGS