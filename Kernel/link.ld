ENTRY(start)

SECTIONS
{
    . = 0x100000;
    
    .setup :
    {
	*(.setup)
    }

    . += 0xC0000000;

    .text : AT(ADDR(.text) - 0xC0000000)
    {
	code = .; _code = .;__code = .;
	*(.text)
    }

    .data ALIGN (4096) : AT(ADDR(.data) - 0xC0000000)
    {
	data = .; _data = .; __data = .;
	*(.data)
	*(.rodata*)
    }

    .bss ALIGN (4096) : AT(ADDR(.bss) - 0xC0000000)
    {
	bss = .; _bss = .; __bss = .;
	*(COMMON*)
	*(.bss*)
    }

    end = .; _end = .; __end = .;
}
