#include <asm/memory.h>
#include <multiboot2.h>

.extern arch_main
.global _start
.global bootstrap_pde

.section .bootstrap, "ax"
.type _start, @function
_start:
	cli

	// LOAD Page Directory Table
	leal KERNEL_PHYS(bootstrap_pde), %ecx
	movl %ecx, %cr3

	movl %cr4, %ecx
	orl $0x00000010, %ecx
	movl %ecx, %cr4

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
_1:
	hlt
	jmp _1

.section .data
.align PAGE_SIZE
bootstrap_pde:
	.long 128 + 3
	.long 128 + 3 + 0x00400000
	.rept 766
	.long 0
	.endr
	.long 128 + 3
	.long 128 + 3 + 0x00400000
	.rept 254
	.long 0
	.endr

.section .bss
.align 16
kernel_stack_bottom:
	.skip KERNEL_STACK_SIZE
.global kernel_stack_top
kernel_stack_top:
