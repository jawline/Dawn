/**
 * @page UMTheory User mode Theory
 *
 * The theory behind user mode is that executable code can intentionally or accidentally perform instructions that can interrupt/damage the operation of the operating system, or worse, the hardware. To minimize these risks most modern CPU’s including every CPU inside PC released recently supports some kind of protection mechanism. On the x86 architecture the protection is ring based, code running in ring 0 can execute any instruction, whereas code running in ring 3 can only execute from a reduced instruction set which results in no access to input and output ports and the ability to enable and disable interrupts. In addition to the reduced instruction set the protection mechanisms often also allow the operating system to disable write access to certain regions of memory (Such as the kernel code & data which is in every processes virtual memory in SimpleOS) which stops software accidentally or intentionally corrupting the kernel.
 *
 */
