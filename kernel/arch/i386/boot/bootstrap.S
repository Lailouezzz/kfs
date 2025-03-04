global _start
extern phys_kernel_start
extern phys_kernel_end

bits 32
section .bootstrap
_start:
	cmp eax, 0x36d76289
	je multiboot_valid
	hlt
multiboot_valid:
	jmp multiboot_valid
