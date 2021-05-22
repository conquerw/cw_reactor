#!/bin/bash

export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

cd cmake

cmake .

make

rm -fr CMakeCache.txt CMakeFiles/ cmake_install.cmake Makefile

cd ../
