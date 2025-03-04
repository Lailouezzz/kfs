#include <multiboot.h>

#include <vga.h>

/**
 * @brief The arch main entry in the higher half
 *
 * @param mb_info multiboot2 info
 * @param magic multiboot magic
 */
void	arch_main(struct multiboot_info *mb_info, unsigned int magic)
{
	(void)mb_info;
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {

	}
	vga_setup();
	vga_print_string("Hello world !");
	vga_print_char('\n');
	vga_print_string_c("Colored kernel baby hehe", 0x04);
	vga_move_cursor(7, 7);
	vga_print_string_c("Movable cursor too !!", 0x5);
}
