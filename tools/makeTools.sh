#!/usr/bin/env sh

mkdir build -p && cd build || exit
cmake ../ || exit
make -j2