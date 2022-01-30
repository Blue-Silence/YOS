# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a multiboot header that marks the program as a kernel.
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Allocate the initial stack.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384,0 # 16 KiB
stack_top:

# Preallocate pages used for paging. Don't hard-code addresses and assume they
# are available, as the bootloader might have loaded its multiboot structures or
# modules there. This lets the bootloader know it must avoid the addresses.
.section .bss, "aw", @nobits
	.align 4096
.global boot_page_directory
.global boot_page_table1
.global mmap_nodes_usable
.global mmap_nodes_other
boot_page_directory:
	.skip 4096,0
boot_page_table1:
	.skip (4096*16)
# Further page tables may be required if the kernel grows beyond 3 MiB.
mmap_nodes_usable:
	.skip (16*50),0 # Max 50 mmap_nodes
mmap_nodes_other:
	.skip (24*50),0 # Max 50 mmap_nodes


.section .init, "aw", @nobits
	.align 4096
.global temp
temp:
	.skip 4096,0


# The kernel entry point.
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
	mov $(stack_top - 0xC0000000), %esp
	mov %ebx, (temp)
	call mmap


# Start gdt init	
    mov $(stack_top - 0xC0000000), %esp
    call init_gdt
# Finish gdt init

# Start paging init	
	mov $(stack_top - 0xC0000000), %esp
    call init_paging

	# Set cr3 to the address of the boot_page_directory.
	movl $(boot_page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3

	# Enable paging and the write-protect bit.
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0
# Finish paging init

	# Jump to higher half with an absolute jump.
	lea 4f, %ecx
	jmp *%ecx



.global gdt_flush
gdt_flush:
mov 4(%esp), %eax
lgdt (%eax)

mov $0x10, %ax
mov %ax, %ds
mov %ax, %es
mov %ax, %fs
mov %ax, %gs
mov %ax, %ss
jmp $0x08, $.loadCs

.loadCs:
ret






.section .text

4:
	# At this point, paging is fully set up and enabled.
	# Set up the stack.
	mov $stack_top , %esp
	# Enter the high-level kernel.
	call kernel_main

	# Infinite loop if the system has nothing more to do.
	cli
1:	hlt
	jmp 1b
