#include <multiboot2.h>

#include <kfs/vga.h>
#include <kfs/kernel.h>
#include <asm/idt.h>
#include <asm/io.h>

extern
void	init_gdt(void);

static
void	check_multiboot(struct multiboot_info *mb_info, unsigned int magic);

void	init_ps2(void);

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

	check_multiboot(mb_info, magic);

	vga_setup();
	init_ps2();

	__asm__("sti");

	printk("Welcome to KFS !\n");
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
