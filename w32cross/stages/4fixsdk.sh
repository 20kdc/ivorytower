#!/bin/sh -e

# build and run fixerupper
mkdir -p target/bin
clang++ fixerupper.cpp -o target/bin/w32cross-treecasefix
target/bin/w32cross-treecasefix target
