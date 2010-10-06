#!/bin/bash
#update_image.sh
#SimpleOS ISO Image generator.
#Updated Tue 23rd, Feb, 2010
#Updated again Wed 5th, Oct, 2010
#From wed 5th assumes is run from root OS directory
#Removed the build steps from this file and placed in rebuildcomponents and buildramdisk.

echo "Image Update script"

#Get the hard drive image
echo "Image name: "
read image_name


sudo losetup /dev/loop0 $image_name

#Mount the virtual directory
mkdir tmount
sudo umount ./tmount
sudo mount /dev/loop0 ./tmount

#Copy over files
echo "Copying over files to mounted directory"
sudo cp bin/kernel_elf tmount/boot/Kernel
sudo cp bin/ramdisk_build tmount/boot/ramdisk

#Unmount it and remove the tmount directory
sudo umount ./tmount
rmdir tmount

#Print done
echo "Done!"
