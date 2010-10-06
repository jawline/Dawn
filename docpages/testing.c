/**
 * @page Testing
 * This page refers to testing Dawn in a emulated environment. for testing on a live environment see @link Installation installation @endlink
 *
 * @section QEMU Using QEMU
 *
 * @subsection Linux On Linux platforms
 * To test Dawn on linux using QEMU ensure you have qemu installed (sudo apt-get install qemu on Ubuntu (And similar) distributions and similar. sudo yum install qemu on Fedora (And similar) distributions)
 * Simply type sh test.sh from the root of the project directory (For example cd SimpleOSRepo then sh test.sh) And when prompted for a image enter hdd.img
 *
 * Sidenote: on newer versions (From GIT as of Wed Oct 06 06:41 PM) The shell script will not ask you what image to use when you build the OS. If you wish to change the default from hdd.img to another edit test.sh and remove the hdd.img following the call to sh scripts/update_image.sh which will cause the script to prompt for a image file on each run OR replace it with the desired image file
 *
 * @subsection Other
 * Currently not supported (Though images will probably be compatable with any platforms QEMU, I choose not to support them as I have no means to test)
 */
