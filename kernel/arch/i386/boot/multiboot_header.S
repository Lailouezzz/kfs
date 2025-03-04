extern multiboot_header

; TODO : switch this shit to C source file

bits 32
section .multiboot
align 4
multiboot_header:
header_start:
	dd 0xE85250D6 ; magic
	dd 0 ; architecture (0 == i386)
	dd header_end - header_start ; header_length
	dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start))
	dw 0	; type
	dw 0	; flags
	dd 8	; size
header_end:
