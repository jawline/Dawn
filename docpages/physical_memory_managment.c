/**
 * @page PhysicalMM Physical Memory Managment
 * @section PMMOverview Physical memory management
 *
 * A physical memory manager is the term given to software which keeps track of all the physical memory on a system (RAM). It is used to allocate free blocks of data to applications, drivers and modules of the kernel which can then be used.
 *
 *
 * @section DawnMM The Dawn physical memory manager
 * In Dawn the physical memory manager (henceforth referred to as PMM) keeps a list of free chunks of memory. when a piece of memory is requested the PMM takes the first available memory chunks location from the top of the stack, returns it and marks that location as used by removing it from the list. when a location is no longer needed the PMM is sent a free call. it then re-adds the memory chunk to the list.
 *
 */
