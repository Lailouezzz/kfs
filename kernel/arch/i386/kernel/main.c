#include <multiboot2.h>

#include <kfs/vga.h>
#include <kfs/kernel.h>
#include <asm/idt.h>
#include <asm/irq.h>

extern void	init_gdt(void);

static void	check_multiboot(struct multiboot_info *mb_info, unsigned int magic);

/**
 * @brief The arch main entry in the higher half
 *
 * @param mb_info multiboot2 info
 * @param magic multiboot magic
 */
void	arch_main(struct multiboot_info *mb_info, unsigned int magic)
{
	init_gdt();
	init_idt();
	init_irq();

	vga_setup();

	check_multiboot(mb_info, magic);

	vga_print_string("42\n");
	vga_print_string_c("Colored kernel baby hehe", 0x04);
	vga_move_cursor(7, 7);
	vga_print_string_c("Movable cursor too !!\n", 0x5);

	__asm__("sti");
}

static void	check_multiboot(struct multiboot_info *mb_info, unsigned int magic)
{
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
		panic("No multiboot 2 header present (%x)", magic);

	FOREACH_MULTIBOOT_TAG(tag, mb_info)
	{
		switch (tag->type)
		{
		case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
			const struct multiboot_tag_basic_meminfo	*meminfo = (void *)tag;

			printk("upper memory size : %d\nlower memory size : %d\n",
				meminfo->mem_lower, meminfo->mem_upper);
			break ;

		default:
			break ;
		}
	}
}
