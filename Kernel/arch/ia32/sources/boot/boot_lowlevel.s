;
; boot_lowlevel.s - Calls the kernel main function in boot_highlevel - Kernel start location
; File also defines the multiboot header
; The intention of this file is to call the kernels main entry point (main function) after setting up the appropriate information on the stack (Pushing the location of the multiboot header 
; and any other needed information from the boot)
;
; Update - Friday the 28th of May - Now setup so it jumps using the GDT trick. Kernel now has to setup paging and identity map the first few mbs PRIOR to setting up the GDT
;
; Update - Wed the 7th of July - Using a cheap page directory instead of the fake GDT. GDT trick = Ugly hack
; Update - 4MB Pages enabled but not necessarily used
;

[BITS 32]                       ; All instructions should be 32-bit.


MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; NOTE:Not using MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

KERNEL_VIRTUAL_BASE equ 0xC0000000                  ; 3GB
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)  ; Page directory index of kernel's 4MB PTE.

[GLOBAL mboot]                  ; Make 'mboot' accessible from C.
[EXTERN _code]                   ; Start of the '.text' section.
[EXTERN _sbss]                    ; Start of the .bss section.
[EXTERN _k_end]                    ; End of the last loadable section.

[SECTION .text]
mboot:
    dd  MBOOT_HEADER_MAGIC      ; GRUB will search for this value on each
                                ; 4-byte boundary in your kernel file
    dd  MBOOT_HEADER_FLAGS      ; How GRUB should load your file / settings
    dd  MBOOT_CHECKSUM          ; To ensure that the above values are correct
    
    dd  mboot                   ; Location of this descriptor
    dd  _code                    ; Start of kernel '.text' (code) section.
    dd  _sbss                     ; End of kernel '.data' section.
    dd  _k_end                    ; End of kernel.
    dd  start                  ; Kernel entry point (initial EIP).

[GLOBAL start]                  ; Kernel entry point.
[EXTERN entry_point]                   ; This is the entry point of our C code

start:
	mov ecx, boot_pagedir - KERNEL_VIRTUAL_BASE
	mov cr3, ecx                                        ; Load Page Directory Base Register
	
	mov ecx, cr4
	or ecx, 0x00000010                          ; Set PSE bit in CR4 to enable 4MB pages.
	mov cr4, ecx

	mov ecx, cr0
	or ecx, 0x80000000                          ; Set PG bit in CR0 to enable paging.
	mov cr0, ecx
	
	jmp higherhalf + KERNEL_VIRTUAL_BASE

higherhalf:
	;mov dword [boot_pagedir], 0
	; Apparently it does matter invlpg[0]

	mov esp, sys_stack_start
	push esp

	;Pass the multiboot structure
	push sys_stack_start
	push ebx

	call entry_point

	jmp $

[SECTION .data]
align 4096
boot_pagedir:
	dd 0x83
	times (KERNEL_PAGE_NUMBER - 1) dd 0                 ; Pages before kernel space.
	; This page directory entry defines a 4MB page containing the kernel.
	dd 0x83
	times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0  ; Pages after the kernel image.


	 
[section .bss]
 
resb 0x4000
sys_stack_start: ;After the RESB because stacks expand downwards
	; our kernel stack
