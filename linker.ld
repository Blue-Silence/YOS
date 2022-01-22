ENTRY (_start)

SECTIONS
{
    . = 0x00100000;

    .multiboot.data ALIGN (4K) : {
        *(.multiboot.data)
		*(.init_stack)
    }

   .multiboot.text ALIGN (4K) : {
       *(.multiboot.text)
    }

	. = 0x00400000; /* id map only the above 4M part , used as "_kernel_start" */
	. += 0xC0000000;
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
	PROVIDE( kern_end = .; );  /* edited,untested */
}