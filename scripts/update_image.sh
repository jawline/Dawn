#!/bin/bash
#update_image.sh
#SimpleOS ISO Image generator.
#Updated Tue 23rd, Feb, 2010

#cd to the OS directory
cd ..

#Get the hard drive image
echo "Image name: "
read image_name

#Build Line
echo "Rebuilding Line"
cd Line
make clean
make
cd ..

#Copy Line to the ramdisk dir
cp Line/Build/Line InitRD/Line.x

#Make temporary directory
echo "Recompiling initial RAM disks";
cd InitRD
sh buildrd.sh
cd ../

sudo losetup /dev/loop0 $image_name

sudo umount ./tmount
sudo mount /dev/loop0 ./tmount

echo "Copying Kernel"
sudo cp Kernel/Build/Kernel tmount/boot/Kernel
sudo cp InitRD/ramdisk.img tmount/boot/ramdisk

sudo umount ./tmount

echo "Done!"
