[GLOBAL get_cpuid_features]
get_cpuid_features:
	push ebp
	mov ebx, esp

	mov eax, 0x1
	cpuid
	mov eax, edx

	pop ebp
	ret
