#Magic numbers for multiboot
.set ALIGN,  1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC+FLAGS)

#Sets the multiboot header
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

#Makes space for a small stack
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
	#sets the stack
	mov $stack_top, %esp

	#TOD: Load GDT, enable paging

	call kernel_main

	#infinty loop
	cli
1:	hlt
	jmp 1b

#Caluclates the size of the _start functions
.size _start, . - _start

