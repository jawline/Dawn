#!/bin/bash
#update_image.sh
#SimpleOS ISO Image updater.
#Updated Tue 23rd, Feb, 2010

#MOUNT Image
echo "Mounting Image"
sudo mkdir t_disk_mount
sudo mount -o loop diskimage.iso t_disk_mount

#MOVE Files
echo "Moving Kernel"
sudo cp Kernel/Build/Kernel t_disk_mount/boot/Kernel

#UNMOUNT Image
echo "Unmounting Image"
sudo umount t_disk_mount
sudo rmdir t_disk_mount
