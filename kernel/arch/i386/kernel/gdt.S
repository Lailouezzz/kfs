.section .data, "a"
.align 16
gdt_start:
	.quad 0x0000000000000000     // Null Descriptor

// Kernel Code Segment (0x08)
gdt_kernel_code:
	.word 0xFFFF                 // Limit (0-15)
	.word 0x0000                 // Base (0-15)
	.byte 0x00                   // Base (16-23)
	.byte 0x9A                   // Flags: Code, Execute, Read, Present, Ring 0
	.byte 0xCF                   // Flags: Granularity, 32-bit, Limit (16-19)
	.byte 0x00                   // Base (24-31)

// Kernel Data Segment (0x10)
gdt_kernel_data:
	.word 0xFFFF
	.word 0x0000
	.byte 0x00
	.byte 0x92                   // Data Segment, Read/Write, Present, Ring 0
	.byte 0xCF
	.byte 0x00

// User Code Segment (0x18)
gdt_user_code:
	.word 0xFFFF
	.word 0x0000
	.byte 0x00

	.byte 0xFA                   // Code Segment, Execute/Read, Present, Ring 3

	.byte 0xCF


	.byte 0x00

// User Data Segment (0x20)
gdt_user_data:
	.word 0xFFFF
	.word 0x0000
	.byte 0x00
	.byte 0xF2                   // Data Segment, Read/Write, Present, Ring 3
	.byte 0xCF
	.byte 0x00

gdt_end:

gdt_pointer:
	.word gdt_end - gdt_start - 1  // Taille de la GDT (moins 1)
	.long gdt_start                // Adresse de la GDT

.global init_gdt
.section .text, "ax"

init_gdt:
	lgdt gdt_pointer
	ljmp $0x8, $flush
flush:
	mov $0x10, %eax
	mov %eax, %ds
	mov %eax, %ss
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	ret
