
/**
 * @page Installation
 * @section InstallOnLinux On Linux platforms
 * To install Dawn onto a drive from a linux platform follow this set of instructions
 */

/**
 * @page Roadmap
 * @section ReleaseOne Release 1 codename “Boot”
 * The plans for the first iteration of Dawn are simply to have a executable, created by whatever means, that can be loaded by a bootloader and print a basic “hello world” message onto the screen. To do this several aims have to be achieved:
 *	- A set of tools have to be chosen which would be versatile enough to allow me to achieve all my aims.
 *	- I need to select a format, and the boot loader which I am going to use for the project (at least to begin with, the OS wants to be dynamic so it must be able to run with a variety of boot loaders).
 *	- I need to read the Intel manual, namely the sections referring to the initial state of a monitor and how it is mapped to the address space.
 * 	- I need to design a “Makefile” which will allow me to easily add and remove files to be compiled from my project and link created files into a base executable.
 *	- I need to setup a set of scripts that can start an emulated 32-Bit environment.
 *	- I need to write a set of shell scripts which will allow me to quickly build and test any created executables.
 *
 *
 * @section ReleaseOneSummary Release 1 summary – February 23, 2010
 * Release 1 Implementation notes
 *	- the GNU toolset was chosen, with NASM as a assembly compiler
 *	- I selected the ELF file format as the base system format, and GRUB as my systems boot loader
 *	- I have implemented a basic text-mode screen driver in C using the assumption that the PC starts with a screen in text mode that I can access using the default mapped location of memory
 *	- I have designed a fairly versatile makefile which will allow me to expand and contract the project and source tree with relative ease
 * 	- I have designed a set of shell scripts which not only setup and build the OS into a CD image, but also execute a emulator with a given set of parameters, leaving small times between a build and testing.
 *
 *
 * @section Release2 Release 2 “Memory”
 *
 *This release is primarily focused on the management of RAM and manipulation of memory maps. It has two major aims
 *	- A physical memory manager which could have a list of all the free RAM chunks in memory and allocate them easily
 *	- A virtual memory manager that could handle the mappings between physical chunks and areas of memory
 *
 *

 * @section Release2Summary Release 2 summary – February 24, 2010
 *	- A physical memory manager which manages a stack of free areas of RAM has been added to the kernel.
 *	- A abstracted virtual memory manager was created to manage the mappings between physical and virtual memory
 *	- A paged memory model in which fixed size blocks of physical memory can be mapped to address space has been added
 *	- A simple segmented memory model was setup (GDT) this was more a necessity as some functions of the IA32 architecture are not available without one#
 *
 *
 * @section Release3 Release 3 “Interrupts”
 * The aim of this release is to have a abstracted interrupt mechanism within the kernel, to which callbacks could bind.
 * It has several aims
 *	- Design a abstracted set of methods which could be used to provide a versatile method of handling interrupts
 *	- Implement the low-level hardware code that allowed the processor to correctly handle any interrupts and pass them through the abstraction layer
 *
 *
 * @section Release3Summary Release 3 summary – March 09, 2010
 *	- A heap subsystem was required to make certain parts of the planned release work properly, and so has been added to this release as a feature
 *	- A set of functions which allow the registering of certain interrupt numbers with callbacks. Nice simple abstraction
 *	- A low level interrupt handler is implemented which stores the current state of the processor and then executes the interrupt by calling the execute method in the abstraction layer
 *	- The virtual memory and physical memory managers where modified and corrected to suit them more to their purposes
 *
 *
 * @section Release4 Release 4 “Files”
 * The aim of this release is to have a virtual file system setup within the kernel and a initial ram disk with which to utilize this new functionality
 *	- The virtual file system, a set of structures and methods within the kernel that allow for the concept of files and directories to be used.
 *	- The ram disk, a initial ram disk that gets loaded by the boot loader and passed to the kernel, this will act as a base file system within which all core executables and files can be stored where they will be accessible on boot
 *
 *
 * @section Release4Summary Release 4 summary – March 20, 2010
 * This release required lots of changes to the existing setup, and several new features where introduced which where not listed in the road map for release 4.
 *	- The kernel now has a ram disk which is loaded into memory on startup, it as of yet, provides no real use to the kernel however it is fully functional as a source of read-only files on boot.
 *	- The kernel now has a abstracted input layer, and a keyboard driver. These features where necessary to improve my ability to test the functionality of the kernel
 *	- The kernel now has a virtual file system to which other file systems can be bound, such as the initial ram disk
 *	- Changes where made to the virtual and physical memory managers and the heap to facilitate this new functionality. Also there where bug fixs to make it work on real hardware
 *
 *
 * @section Release5 Release 5 “Multitasking”
 * This release is going to be the final step before the project actually starts to look like a operating system. The aim is to implement a set of structures and a scheduler into the kernel which will allow fake multitasking to take place (By giving each process a certain amount of time to process before the next process the illusion of true multi tasking can be achieved)
* The key things needed for this release are
*	- Structures and functions to create and manage and switch between tasks within the operating system. These tasks are known as processes
*	- A scheduler which can keep a list of tasks and manage which process should be active.
*
*
* @section Release5Summary Release 5 summary – June 13, 2010
* In my implementation of multi-tasking I utilized the interrupt structure I designed for release 3 to set up a interrupt that gets called repeatedly by a hardware timer (Initially I am using the programmable interrupt timer). The interrupt code will automatically store the state of the processor when it is called, so all the scheduler has to do is check if the current process has had its fill of CPU attention and if it has then switch to the next by calling a switch process function and passing the current and next process as arguments.
* 
* Key points:
*	- A scheduler was designed which allowed for simple round robin scheduling
*	- The virtual memory managed was modified to allow for multiple address spaces for different processes but with identical kernel spaces
*	- A low level process manager was designed which contains structures to store all necessary data about a process and methods to initialize, manipulate and swap between processes
*
*
* @section Release6 Release 6 “Applications”
* This release is going to focus on getting a executable loaded into physical and virtual memory from the virtual file system setup in release 4 and executing it. The key aims being:
*	- Selecting a executable format which the kernel should initially be able to parse, load and execute
*	- Writing a abstracted loader for that format
*	- Writing a set of high level functions which will call the loader for the format given a set of parameters (Something like loadAndExecute(fs_node_t* node))
*	- Designing a API that allow the kernel and application to talk to each other and pass data.
*
*
* @section Release6Summary Release 6 summary – June 30, 2010
* In this release I designed a set of functions that could load a simplistic executable from a popular executable file format and execute them in a new process. The key notes are that
*	- I selected the ELF binary executable format for my applications, as it is a very versatile format which is well documented and supported by most major compilers, including GCC.
*	- I wrote a set of functions that allow for abstracted loading of executables
*	- I wrote a loader specific to the ELF Executable though it needs improvement (To handle other archs gracefully, to handle DLL’s and relocatable code as well)
*	- I wrote a API which the loaded executable can use to handle both input and output
*
*
* @section Release7 Release 7 “Cleanup”
* At this stage in the project most of the core OS functions have been added, however they are not all polished, and they do not all work nicely together. With the addition of programmable applications this starts to become a real issue as applications will being to use multiple features of the OS concurrently. To avoid these issues the seventh release will be focused not on adding any truly new features, but adding layers of abstraction and fixing up segments of code which will allow for different areas of the kernel to interact properly with each other. This is also the release in which all the features will be rigorously tested working together, instead of the previous setup in which they where tested individually
*
*
* @section Release7Summary Release 7 Summary – August 25, 2010
* The kernel was swapped to a new function naming system designed to make code easier to understand and bugs easier to find. A number of bugs where fixed, which would have had a great affect kernels stability greatly such as a bug leading to double freeing of some pages (Which would have lead to bad memory corruptions later on)
*
*
* @section Release8 Release 8 “Applications land”
* This release aims to execute functions in ring-3 or userspace, a reduced execution environment in which certain areas of memory are restricted, thus preventing the kernel and hardware from being attacked by malicious code and bugs.
*
*
* @section Release8Summary Release 8 Summary – September 26, 2010
* The kernel memory manager was reprogrammed to allow user-mode programs access to read and write to the correct areas of memory, so they couldn’t damage the kernel. Getting to user-mode was fairly easy using the Intel documentation in the manual, however getting things like interrupts working properly after jumping to user mode was more of a challenge, require workarounds in the jump code to enable them without putting the kernel at risk of facing a interrupt while executing a critical piece of code. Other minor bugs such as problems with the user-mode flags within the IDT took a long time to find and fix.
*
*
* @section Release9 Release 9 "Name change and documentation"
* For this release I have decided to rebrand SimpleOS with the title of Dawn. The indavidual sections will still have there specific codenames (Kernel codename and OS version codenames) but the OS as a collective will be known from this point forward as Dawn. this will require some significant changes to the files within the OS, as I have to modify or remove any reference to "SimpleOS"
*
*
*@section Release9Summary Release 9 Summary - 08 October 2010
* I renamed the repository and removed any references to SimpleOS, replacing them with Dawn.
* I started using Doxygen to generate documentation which could integrate documentation of the source code with my operating system theory and Dawn implementation specs.
*
*
* @section Release10 Release 10 "Drivers"
* This release aims to have a simple driver interface which allows the operating system to load and execute highly privileged pieces of code which can be used to provide support for functionality and hardware that the initial release did not contain.
*
*
*/
