cd scripts
sh build_and_update.sh
cd ..
qemu -cdrom boot_disk.iso -hda harddisk.img -boot d
