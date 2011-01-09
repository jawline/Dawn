#!/bin/sh
#Assumes to be run from the root OS directory

echo "Moving to Kernel directory"
cd Kernel

echo "Cleaning previous build output files"
make -f mk_ia32 clean

echo "Building Kernel"
make -f mk_ia32

echo "Done!"

