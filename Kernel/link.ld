ENTRY(start)

SECTIONS
{
    . += 0xC0100000;

    kstart = .;
    _kstart = .;
    __kstart = .;

    .text : AT(ADDR(.text) - 0xC0000000)
    {
	code = .; 
	_code = .;
	__code = .;

	*(.text)
        *(.rodata*)

	ecode = .;
	_ecode = .;
	__ecode = .;
    }

    .data ALIGN (4096) : AT(ADDR(.data) - 0xC0000000)
    {
	data = .;
	_data = .;
	__data = .;

	*(.data)

	edata = .;
	_edata = .;
	__edata = .;
    }

    .bss : AT(ADDR(.bss) - 0xC0000000)
    {
	sbss = .;
	_sbss = .;
	__sbss = .;

	*(COMMON)
	*(.bss)

	ebss = .;
	_ebss = .;
	__ebss = .;
    }

    k_end = .;
    _k_end = .;
    __k_end = .;
}
