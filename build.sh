#!/usr/bin/env bash

# Build the CMake files.
cmake --DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" .

# Create the compiled executable.
make

# Remove the unnecessary files.
rm ./CMakeCache.txt
rm ./cmake_install.cmake
rm annotator.cbp
rm Makefile
rm -rf CMakeFiles
