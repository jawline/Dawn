ENTRY(_start)

SECTIONS
{
    . = 0xA000000;

    .text : AT(ADDR(.text))
    {
	code = .; _code = .;__code = .;
	*(.text)
    }

    .data : AT(ADDR(.data))
    {
	data = .; _data = .; __data = .;
	*(.data)
	*(.rodata*)
    }

    .bss : AT(ADDR(.bss))
    {
	bss = .; _bss = .; __bss = .;
	*(COMMON*)
	*(.bss*)
    }

    end = .; _end = .; __end = .;
}
