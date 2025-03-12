#include <multiboot2.h>

#include <kfs/vga.h>
#include <kfs/kernel.h>
#include <asm/idt.h>

extern
void	init_gdt(void);

static
void	check_multiboot(struct multiboot_info *mb_info, unsigned int magic);

static
void	timer_handler(interrupt_stack_frame_t *sf);

/**
 * @brief The arch main entry in the higher half
 *
 * @param mb_info multiboot2 info
 * @param magic multiboot magic
 */
void	arch_main(struct multiboot_info *mb_info, unsigned int magic)
{
	vga_setup();

	init_gdt();
	init_idt();

	check_multiboot(mb_info, magic);

	vga_print_string("42\n");
	vga_print_string_c("Colored kernel baby hehe", 0x04);
	vga_move_cursor(7, 7);
	vga_print_string_c("Movable cursor too !!\n", 0x5);

	__asm__("sti");

	irq_install_handler(0, timer_handler);
}

static
void	check_multiboot(struct multiboot_info *mb_info, unsigned int magic)
{
	interrupt_stack_frame_t	s = {0};

	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
	{
		DUMP_STACK_FRAME(s);
		panic(s, "No multiboot 2 header present (%x)", magic);
	}

	FOREACH_MULTIBOOT_TAG(tag, mb_info)
	{
		switch (tag->type)
		{
		case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
			const struct multiboot_tag_basic_meminfo	*meminfo = (void *)tag;

			printk("upper memory size : %d\nlower memory size : %d\n",
				meminfo->mem_lower, meminfo->mem_upper);
			break;

		default:
			break;
		}
	}
}

static
void	timer_handler(interrupt_stack_frame_t *sf)
{
	static int x = 0;

	(void)sf;
	if (++x >= 10)
	{
		x = 0;
		printk("Timer fired 10 ticks\n");
	}
}
