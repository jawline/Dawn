#!/bin/sh

#CD To the OS directory
cd ..

echo "Moving to Kernel directory"
cd Kernel

echo "Cleaning previous build output files"
make clean

echo "Building Kernel"
make

echo "Done!"

