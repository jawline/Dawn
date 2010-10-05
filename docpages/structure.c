/**
 * @page Structure The structure of a operating system
 *
 * A operating system can be split into 3 major components. The kernel, drivers and applications.
 * @section Kernel
 *
 * The kernel is the management center of a operating system. It is given the control from boot and manages everything the processor has access to like drivers, applications, RAM etcetera. The kernel has ring 0 level access, this means that it can access any area of the system and call any instruction within its code, this leads to kernel bugs being major problems as they can crash the whole environment. The kernel is typically the hardest and most complicated part of a operating system.
 * 
 *
 * @section Drivers
 *
 * Drivers have similar access levels to the kernel. Their purpose is to handle a specific peripheral connected to the environment – hence the name. They can typically crash the environment though bugs are less likely to cause a complete failure then a kernel bug.
 *
 *
 * @section Applications
 *
 * A application is a piece of code that is loaded run and managed by the operating system. Its run with low level privileges so their is low risk of them crashing the whole environment – thus all non-essential ring 0 processes should aim to be moved to applications in ring-3.
 */

