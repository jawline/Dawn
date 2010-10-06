echo "WARNING: Incorrect values can screw up your hard drive. Be careful"
echo "I am not responsible for anything that happens using this tool"
echo "Author: Blake Loring"

echo "Enter root address"
read root_addr
echo "Enter partition address"
read root_part
echo "Processing"

mkdir tmount

sudo umount $root_part
sudo mount $root_part tmount

sudo grub-install --root-directory=./tmount/ $root_addr --force

echo "Copying Kernel"

sudo cp bin/kernel_elf tmount/boot/Kernel
sudo cp bin/ramdisk_build tmount/boot/ramdisk

sudo cp etc/grub.cfg tmount/boot/grub/grub.cfg

sudo umount tmount

sudo umount $root_part

rmdir tmount
