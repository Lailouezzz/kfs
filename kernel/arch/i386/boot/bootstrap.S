#include <asm/memory.h>
#include <multiboot.h>

.extern arch_main
.global _start
.global bootstrap_pde

.section .bootstrap, "ax"
.type _start, @function
_start:
	cli
	leal KERNEL_PHYS(bootstrap_pde), %ecx
	movl %ecx, %cr3

	movl %cr0, %ecx
	orl $0x80000001, %ecx
	movl %ecx, %cr0
	movl $_higher_half, %ecx
	jmp *%ecx // Flush CPU pipeline

.section .text, "ax"
_higher_half: // hello there we are in the higher half
	movl $kernel_stack_top, %esp
	pushl %eax
	pushl %ebx
	call arch_main
1:
	hlt
	jmp 1

.section .data
.align PAGE_SIZE
bootstrap_pde:
	.rept 1024
	.long KERNEL_PHYS(bootstrap_pte) + 1 // bootstrap_pte symbol is located in the _higher_half
	.endr
bootstrap_pte:
	.set addr, 0
	.rept 1024
	.long addr | 3
	.set addr, addr + 0x1000 // skip a page
	.endr

.section .bss
.align 16
kernel_stack_bottom:
	.skip KERNEL_STACK_SIZE
.global kernel_stack_top
kernel_stack_top:
