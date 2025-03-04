OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS {
	. = 1M; /* 0x00100000 */



	.multiboot ALIGN(4): AT(ADDR(.multiboot)) {
		KEEP(*(.multiboot))
	}

	.bootstrap : AT(ADDR(.bootstrap)) {
		*(.bootstrap)
	}

	.text ALIGN(4K): AT(ADDR(.text)) {
		*(.text)
	}

	.rodata ALIGN(4K): AT(ADDR(.rodata)) {
		*(.rodata)
	}

	.bss ALIGN(4K): AT(ADDR(.bss)) {
		*(.bss)
		*(COMMON)
	}

	.data ALIGN(4K): AT(ADDR(.data)) {
		*(.data)
	}

	. = ALIGN(4K);
	phys_root = .;
	phys_kernel_start = .;
	. = 0xC0000000;
	virt_root = .;


	. = ALIGN(4K);
	phys_kernel_end = . - virt_root + phys_root;

}
