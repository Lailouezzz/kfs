#include <asm/memory.h>
#include <multiboot.h>

.extern arch_main
.global _start
.global bootstrap_pde

.section .bootstrap, "ax"
.type _start, @function
_start:
	cli
	lea KERNEL_PHYS(bootstrap_pde), %ecx
	mov %ecx, %cr3

	mov %cr0, %ecx
	or $0x80000001, %ecx
	mov %ecx, %cr0
	lea _higher_half, %ecx
	jmp *%ecx

.section .text, "ax"
_higher_half:
	push %eax
	push %ebx
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
kernel_stack_top:
