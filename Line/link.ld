ENTRY(start)

SECTIONS
{
    . = 0x1000000;
    
    .setup :
    {
	*(.setup)
    }

    .text : AT(ADDR(.text))
    {
	code = .; _code = .;__code = .;
	*(.text)
    }

    .data ALIGN (4096) : AT(ADDR(.data))
    {
	data = .; _data = .; __data = .;
	*(.data)
	*(.rodata*)
    }

    .bss ALIGN (4096) : AT(ADDR(.bss))
    {
	bss = .; _bss = .; __bss = .;
	*(COMMON*)
	*(.bss*)
    }

    end = .; _end = .; __end = .;
}
