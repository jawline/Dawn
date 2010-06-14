[GLOBAL check_cpuid]
check_cpuid:
	pushfd ; get
	pop eax ;Flags to eax
	mov ecx, eax ;Save current flags
	xor eax, 0x200000 ; flip the id bit
	push eax ; to eax
	popfd

	pushfd ; Test if the bit is 1
	pop eax
	xor eax, ecx ; mask changed bits
	shr eax, 21 ; Bitwise shift 21 bits right so bit 0 = the bit wanted to test
	and eax, 1 ; AND test eax and 1, leaves 0 if false 1 if true

	ret
