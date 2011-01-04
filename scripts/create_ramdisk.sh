#Script to create the ramdisk
#Assumes run from root directory
#Assumes build steps completed

cp ./Config/* ./bin/RamdiskFolder/
./bin/Tools/RamdiskCompiler ./bin/RamdiskFolder/ ./bin/ramdisk_build
