#include "cpu.h"

//Function: checkCpuidInlne
//Arguments: None
//Return: 1 if true 0 if false
//Description: Inline ASM to check whether CPUID is supported

inline unsigned char checkCpuidInline()
{
	unsigned long eax_reg_value = 0;
	unsigned char return_value = 0;

	//Description
	//pushfd pushes the EFLAGS register to the stack
	//pop eax sets the value of eax to EFLAGs
	//mov eax, ecx; stores the value of eax in ecx
	//xor $0x20000, %%eax flips the bit at 0x200000
	//pushl %%eax push's the top of the stack to the eax register
	//popfl sets the EFLAGs register
	//pushfl pushes EFLAGs to the stack again
	//popl eax sets the eax register to the new value of EFLAGs
	//XOR Nulls all values true with the last EFLAGs
	//SHR Shifts the register 21 bits to the right
	//The AND test the value of eax
	//The mov sets the return value to the value of EAX
	asm volatile("pushfl;\n \
		      popl %%eax;\n \
		      movl %%eax, %%ecx;\n \
		      xor $0x200000, %%eax; \n \
		      pushl %%eax; \n \
		      popfl; \n \
		      pushfl; \n \
		      popl %%eax; \n \
		      xorl %%ecx, %%eax; \n \
		      shrl $21, %%eax; \n \
		      andl $1, %%eax; \n \
		      movl %%eax, %0; \n \
		      " : "=r" (eax_reg_value));

	return_value = eax_reg_value;

	return return_value;
}

//Function:
//Arguments: None
//Returns: 1 if true 0 if false
//Description: Check if the CPUID function is supported
unsigned char cpuidSupported()
{
	return checkCpuidInline();
}
