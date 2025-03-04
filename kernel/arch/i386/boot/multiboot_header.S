#include <multiboot.h>

extern multiboot_header


bits 32
section .multiboot
align 4
multiboot_header:
header_start:
	dd MULTIBOOT2_HEADER_MAGIC
	dd MULTIBOOT_ARCHITECTURE_I386
	dd header_end - header_start ; header_length
	dd 0x100000000 - (MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (header_end - header_start))
	dw 0	; type
	dw 0	; flags
	dd 8	; size
header_end:
