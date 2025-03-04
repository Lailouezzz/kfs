#include <multiboot.h>

global _start
extern arch_main

bits 32
section .bootstrap
_start:
	cli
	push eax
	push ebx
	call arch_main
.1:
	hlt
	jmp .1
