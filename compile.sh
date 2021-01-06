#!/bin/bash

export TEK_TARGET_PLATFORM=Linux
export TEK_DEBUG=True
export TEK_64=True

mkdir -p bin
mkdir -p build
cd build
cmake ..
make
cd ..
