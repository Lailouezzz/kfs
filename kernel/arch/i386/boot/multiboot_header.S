#include <multiboot.h>

.extern multiboot_header


.section .multiboot, "a"
.align 4
multiboot_header:
header_start:
	.long MULTIBOOT2_HEADER_MAGIC
	.long MULTIBOOT_ARCHITECTURE_I386
	.long header_end - header_start
	.long 0x100000000 - (MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (header_end - header_start))
	.word 0
	.word 0
	.long 8
header_end:
