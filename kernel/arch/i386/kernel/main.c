#include <multiboot.h>

#include <kfs/vga.h>
#include <kfs/kernel.h>

extern void	init_gdt(void);
extern int kernel_stack_top;

/**
 * @brief The arch main entry in the higher half
 *
 * @param mb_info multiboot2 info
 * @param magic multiboot magic
 */
void	arch_main(struct multiboot_info *mb_info, unsigned int magic)
{
	int *esp;
	(void)mb_info;
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
	{
		// TODO : panic
	}
	init_gdt();
	vga_setup();
	vga_print_string("42\n");
	vga_print_string_c("Colored kernel baby hehe", 0x04);
	vga_move_cursor(7, 7);
	vga_print_string_c("Movable cursor too !!", 0x5);
	__asm__ ("movl %%esp, %0":"=r"(esp));
	for (; esp < &kernel_stack_top; ++esp)
		printk("%x ", *esp);
}
