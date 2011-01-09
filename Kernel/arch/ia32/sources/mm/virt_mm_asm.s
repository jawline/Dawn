[GLOBAL asm_disable_paging]
asm_disable_paging:
	mov edx, cr0
   	and edx, 0x7fffffff
   	mov cr0, edx
	ret

[GLOBAL asm_enable_paging]
asm_enable_paging:
	mov edx, cr0
   	or edx, 0x7fffffff
   	mov cr0, edx
	ret
