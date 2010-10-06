/**
 * @page Installation
 * @section InstallOnLinux On Linux platforms
 * Warning: This guide assumes you have all the tools required to install Dawn to a disk. namely GRUB bootloader installer and the compilers required to build the OS
 * (GCC and NASM)
 * To install Dawn onto a drive from a linux platform follow this set of instructions
 * 1. Identify the root device and the partition of that you wish to install SimpleOS onto. (For example /dev/sdb as the root device and /dev/sdb1 as the partition)
 * 2. From the root OS directory execute sudo sh scripts/install_drive.sh
 * 3. When prompted enter the root device address (For example /dev/sdb)
 * 4. When prompted enter the partition address (For example /dev/sdn1)
 * 5. Wait until GRUB has finished installing and the OS has finished copying over to the partition
 * 6. Type sync and then unmount the drive
 * 7. The device is now bootable. if it was a USB for example remove it from the host machine and then boot your testbed from it.
 */
