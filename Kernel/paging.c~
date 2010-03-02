#include "paging.h"
#include "idt.h"
#include "screen.h"

extern uint32 end;
static uint32 paging_placement_address = (uint32)&end;

//A bitset of frames.
static uint32 n_frames;
static uint32 * frames;

//The kernels directory so the page directory can be switched back to the kernels quickly.
page_directory_t *kernel_directory=0;
//The current page directory
page_directory_t *current_directory=0;

uint32 placement_alloc(uint32 sz, int align, uint32 *phys)
{

  if (align == 1 && (paging_placement_address & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    paging_placement_address &= 0xFFFFF000;
    paging_placement_address += 0x1000;
  }

  if (phys)
  {
    *phys = paging_placement_address;
  }

  uint32 tmp = paging_placement_address;
  paging_placement_address += sz;

  return tmp;
} 

// Static function to set a bit in the frames bitset
static inline void set_frame(uint32 frame_addr)
{
   uint32 frame = frame_addr/0x1000;
   uint32 idx = INDEX_FROM_BIT(frame);
   uint32 off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static inline void clear_frame(uint32 frame_addr)
{
   uint32 frame = frame_addr/0x1000;
   uint32 idx = INDEX_FROM_BIT(frame);
   uint32 off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static inline uint32 test_frame(uint32 frame_addr)
{
   uint32 frame = frame_addr/0x1000;
   uint32 idx = INDEX_FROM_BIT(frame);
   uint32 off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static inline uint32 first_frame()
{
   uint32 i, j;
   for (i = 0; i < INDEX_FROM_BIT(n_frames); i++)
   {
       if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
       {
           // at least one bit is free here.
           for (j = 0; j < 32; j++)
           {
               uint32 toTest = 0x1 << j;
               if ( !(frames[i]&toTest) )
               {
                   return i*4*8+j;
               }
           }
       }
   }
} 

// Function to allocate a frame.
static inline void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    if (page->frame != 0)
    {
        return;
    }
    else
    {
        uint32 idx = first_frame();

        if (idx == (uint32)-1)
        {
            PANIC("No free frames");
        }

        set_frame(idx*0x1000);
        page->present = 1;
        page->rw = (is_writeable)?1:0;
        page->user = (is_kernel)?0:1;
        page->frame = idx;
    }
}

// Function to deallocate a frame.
static inline void free_frame(page_t *page)
{
    uint32 frame;
    if (!(frame=page->frame))
    {
        return;
    }
    else
    {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint32 cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

inline page_t *get_page(uint32 address, int make, page_directory_t *dir)
{
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    uint32 table_idx = address / 1024;
    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        uint32 tmp;
        dir->tables[table_idx] = (page_table_t*)placement_alloc(sizeof(page_table_t), 1, &tmp);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        return 0;
    }
}

void page_fault(idt_call_registers_t regs)
{
    PANIC("Page fault");
}

void Initialize_Paging(uint32 mem_end_page) {
	//Use placement_alloc to allocate a bitset that marks where frames (Physical areas of RAM which pages can be mapped too) are currently used.
	n_frames = mem_end_page / 0x1000;
        frames = (uint32 *)placement_alloc(INDEX_FROM_BIT(n_frames), 0, 0);
	memset(frames, 0, INDEX_FROM_BIT(n_frames)); //Null all bytes.

	//Use our good 'ol placement_alloc to allocate the space for the kernel's page directory
	kernel_directory = (page_directory_t*)placement_alloc(sizeof(page_directory_t), 1, 0);
	current_directory = kernel_directory;
	

	int i = 0;
	while (i < paging_placement_address)
	{
		// Kernel code is readable but not writeable from userspace.
		alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}

	register_interrupt_handler(14, page_fault);
	switch_page_directory(kernel_directory);
}
