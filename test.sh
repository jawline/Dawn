sh scripts/rebuild_kernel.sh
sh scripts/rebuild_components.sh
sh scripts/create_ramdisk.sh
sh scripts/update_image.sh
qemu -hda "hdd.img"
