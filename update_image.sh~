#!/bin/bash
#update_image.sh
#SimpleOS ISO Image generator.
#Updated Tue 23rd, Feb, 2010

#Requirements: genisoimage installed on the computer.
#	       etc/menu.lst to exist
#	       etc/stage2_eltorito to exist.

#	       The disk will still be bootable if no Kernel is copied. but the bootloader will not function as desired.

#Make temporary directory
echo "Creating temporary directory."
mkdir temp_disk
mkdir -p temp_disk/boot/grub/

echo "Copying bootloader."
cp etc/menu.lst temp_disk/boot/grub/menu.lst
cp etc/stage2_eltorito temp_disk/boot/grub/stage2_eltorito

echo "Copying Kernel"
cp Kernel/Build/Kernel temp_disk/boot/Kernel
cp InitRD/ramdisk.img temp_disk/boot/ramdisk

echo "Generating ISO image"
genisoimage -quiet -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o boot_disk.iso temp_disk

echo "Removing temporary directory"

rm -r temp_disk
echo "Done!"
