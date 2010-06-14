[GLOBAL cpuid_call_vendor]
cpuid_call_vendor:
	push ebp
	mov ebx, esp

	mov eax, 0x0 ;Get the vendor ID
	cpuid ;Call the CPUID function

	mov [vendorid], ebx
	mov [vendorid + 4], edx
	mov [vendorid + 8], ecx
	mov eax, vendorid

	pop ebp
	ret

vendorid db "UnknownVend", 0
