#!/bin/bash

if [ -d build ]; then
  rm -rf build
fi
mkdir build
cd build
cmake .. . -DQ:STRING=$1
make
cd ../
poetry run python answers/$1/main.py
./bin/cmd
