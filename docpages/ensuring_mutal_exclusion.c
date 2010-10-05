/**
 * @page Mutual Exclusion
 * @section EnsuringMutualExclusion Ensuring mutual exclusion within the kernel
 * There are points in the kernel where the same region of memory may need to be accessed by two different processors at the same time, when this occurs it can lead to data corruption and reduce the integrity of the system.
 * 
 *
 * The first simple idea that could be used to try and ensure mutual exclusion would be to set a boolean variable to one when the variables cannot be accessed, and to zero when they can. This approach however doesn’t work using the tradition value = 1; value = 0; methods because two processors may try to set the value at the same time, instead the programmer has to ensure that one processor cannot access the variable while the other can. Luckily GCC provides a set of functions that if used correctly provide this functionality.
 *
 *
 * Locks will be used several sections of the Kernel. In the future a few different methods of locking may be used to ensure the best performance whilst maintaining data integrity and the kernels stability.
 */
