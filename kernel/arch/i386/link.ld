OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

_kernel_higher_half_offset = 0xC0000000;
_kernel_physical_start = 0x00100000;
_kernel_virtual_start = _kernel_physical_start + _kernel_higher_half_offset;

SECTIONS {
	. = _kernel_physical_start; /* 0x00100000 */

	.bootstrap ALIGN(4K) : {
		KEEP(*(.multiboot))
		*(.bootstrap)
	}

	. = ALIGN(4K);
	. += _kernel_higher_half_offset;

	.text ALIGN(4K): AT(ADDR(.text) - _kernel_higher_half_offset) {
		*(.text)
	}

	.rodata ALIGN(4K): AT(ADDR(.rodata) - _kernel_higher_half_offset) {
		*(.rodata)
	}

	.bss ALIGN(4K): AT(ADDR(.bss) - _kernel_higher_half_offset) {
		*(.bss)
		*(COMMON)
	}

	.data ALIGN(4K): AT(ADDR(.data) - _kernel_higher_half_offset) {
		*(.data)
	}

	. = ALIGN(4K);
	phys_kernel_end = . - _kernel_higher_half_offset;
}
