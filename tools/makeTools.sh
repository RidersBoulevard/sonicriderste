#!/usr/bin/env sh

mkdir build -p && cd build || exit
cmake ../ || exit
ninja -j$(nproc --all)