sh scripts/rebuild_components.sh
sh scripts/rebuild_kernel.sh
sh scripts/create_ramdisk.sh
sh scripts/update_image.sh hdd.img

qemu-system-i386 -hda "hdd.img" -cpu coreduo -m 1000