#include <multiboot2.h>

#include <kfs/vga.h>
#include <kfs/kernel.h>

extern void	init_gdt(void);
extern int kernel_stack_top;

void	vga_set_palette_entry(int i, int color);

/**
 * @brief The arch main entry in the higher half
 *
 * @param mb_info multiboot2 info
 * @param magic multiboot magic
 */
void	arch_main(struct multiboot_info *mb_info, unsigned int magic)
{
	(void)mb_info;
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
	{
		// TODO : panic
	}
	init_gdt();
	vga_setup();
	vga_set_text_color(1, 2);
	vga_print_string("hello");
	vga_set_palette_entry(1, 0xff0000);
	vga_print_string(" world");
	vga_reset_text_color();
	vga_print_string(" !");
}
