#include <lib/io.h>

void outb(uint16_t port, uint8_t value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port)
{
   uint8_t ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

void outw(uint16_t port, uint16_t value)
{
	asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

uint16_t inw(uint16_t port)
{
   uint16_t ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

void outl(uint16_t port, uint32_t value)
{
	asm volatile ("outl %1, %0" : : "dN" (port), "a" (value));
}

uint32_t inl(uint16_t port, uint32_t value)
{
	uint32_t ret;
	asm volatile("inl %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}
