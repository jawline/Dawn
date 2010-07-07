;
; boot_lowlevel.s - Calls the kernel main function in boot_highlevel - Kernel start location
; File also defines the multiboot header
; The intention of this file is to call the kernels main entry point (main function) after setting up the appropriate information on the stack (Pushing the location of the multiboot header 
; and any other needed information from the boot)
;
; Update - Friday the 28th of May - Now setup so it jumps using the GDT trick. Kernel now has to setup paging and identity map the first few mbs PRIOR to setting up the GDT
;
; Update - Wed the 7th of July - Using a cheap page directory instead of the fake GDT. GDT trick = Ugly hack
;

[BITS 32]                       ; All instructions should be 32-bit.
[SECTION .setup]

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; NOTE:Not using MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[GLOBAL mboot]                  ; Make 'mboot' accessible from C.
[EXTERN code]                   ; Start of the '.text' section.
[EXTERN bss]                    ; Start of the .bss section.
[EXTERN end]                    ; End of the last loadable section.

mboot:
    dd  MBOOT_HEADER_MAGIC      ; GRUB will search for this value on each
                                ; 4-byte boundary in your kernel file
    dd  MBOOT_HEADER_FLAGS      ; How GRUB should load your file / settings
    dd  MBOOT_CHECKSUM          ; To ensure that the above values are correct
    
    dd  mboot                   ; Location of this descriptor
    dd  code                    ; Start of kernel '.text' (code) section.
    dd  bss                     ; End of kernel '.data' section.
    dd  end                     ; End of kernel.
    dd  start                   ; Kernel entry point (initial EIP).

[GLOBAL start]                  ; Kernel entry point.
[EXTERN main]                   ; This is the entry point of our C code
start:
    lgdt [trickgdt]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:higherhalf

[SECTION .text]
higherhalf:
	mov esp, sys_stack
	push esp
	push ebx
	call main
	jmp $

[section .setup] ; tells the assembler to include this data in the '.setup' section
 
trickgdt:
	dw gdt_end - gdt - 1 ; size of the GDT
	dd gdt ; linear address of GDT

gdt:
	dd 0, 0							; null gate
	db 0xFF, 0xFF, 0, 0, 0, 10011010b, 11001111b, 0x40	; code selector 0x08: base 0x40000000, limit 0xFFFFFFFF, type 0x9A, granularity 0xCF
	db 0xFF, 0xFF, 0, 0, 0, 10010010b, 11001111b, 0x40	; data selector 0x10: base 0x40000000, limit 0xFFFFFFFF, type 0x92, granularity 0xCF
 
gdt_end:
 
[section .bss]
 
resb 0x4000
sys_stack:
	; our kernel stack
