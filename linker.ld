ENTRY (_start)

SECTIONS
{
        . = 0x00100000;
	/* The kernel will live at 3GB + 1MB in the virtual address space, */
	/* which will be mapped to 1MB in the physical address space. */
	/* Note that we page-align the sections. */

	_kernel_start = .;
        .multiboot.data ALIGN (4K) : {
            *(.multiboot.data)
        }

       .multiboot.text ALIGN (4K) : {
           *(.multiboot.text)
       }

    .init ALIGN (4K) :
	{
		init.o(*)
	}


    . = 0x00400000;
	. += 0xC0000000;
	/* Add a symbol that indicates the start address of the kernel. */

		.bss_i ALIGN (4K) : AT (ADDR (.bss_i) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}

		.text ALIGN (4K) : AT (ADDR (.text) - 0xC0000000)
	{
		*(.text)
	}

	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000)
	{
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - 0xC0000000)
	{
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
	/* Add a symbol that indicates the end address of the kernel. */
	PROVIDE(_kernel_end = .);
}

