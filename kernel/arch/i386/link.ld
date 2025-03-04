OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

SECTIONS {
	. = 1M;

	.multiboot ALIGN(8): AT(ADDR(.multiboot)) {
		KEEP(*(.multiboot))
	}

	.bootstrap : AT(ADDR(.bootstrap)) {
		*(.bootstrap)
	}

	. = ALIGN(4K);
	phys_root = .;
	phys_kernel_start = .;
	. = 0xC0000000;
	virt_root = .;

	.text ALIGN(4K): AT(ADDR(.text) - virt_root + phys_root) {
		*(.text)
	}

	.rodata ALIGN(4K): AT(ADDR(.rodata) - virt_root + phys_root) {
		*(.rodata)
	}

	.bss ALIGN(4K): AT(ADDR(.bss) - virt_root + phys_root) {
		*(.bss)
		*(COMMON)
	}

	.data ALIGN(4K): AT(ADDR(.data) - virt_root + phys_root) {
		*(.data)
	}

	. = ALIGN(4K);
	phys_kernel_end = . - virt_root + phys_root;

}
