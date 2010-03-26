#!/bin/sh

#CD To the OS directory
cd ..

echo "Moving to Kernel Directory"
cd Kernel

echo "Building Kernel"
make
echo "Done!"
