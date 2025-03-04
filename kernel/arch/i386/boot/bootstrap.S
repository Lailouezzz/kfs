global _start
extern early_main

bits 32
section .bootstrap
_start:
	cli
	cmp eax, 0x36d76289
	je early_main
	hlt
.1:
	jmp .1
