#include <multiboot2.h>

#include <asm/idt.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/keyboard.h>
#include <asm/mouse.h>
#include <asm/ptrace.h>
#include <asm/ps2.h>
#include <kfs/vga.h>
#include <kfs/kernel.h>

extern void	init_gdt(void);

static void	check_multiboot(struct multiboot_info *mb_info, unsigned int magic);

static void	keyboard_callback(keyboard_event_t event);

static void	mouse_callback(mouse_t mouse);

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

	printk("%11c %11c | ", ' ', ' ');
	vga_print_string_c("   \n", 0x80);

	check_multiboot(mb_info, magic);

	init_ps2();

	keyboard_set_callback(keyboard_callback);
	mouse_set_callback(mouse_callback);

	__asm__("sti");

	printk("Welcome to KFS !\n");
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

static
void	keyboard_callback(keyboard_event_t event)
{
	int	cx;
	int	cy;

	if (event.keycode > KBD_ENTER)
		return ;
	if (!event.pressed)
		return ;
	if (event.keycode == KBD_BACKSPACE)
	{
		vga_get_cursor(&cx, &cy);
		--cx;
		if (cx < 0)
		{
			cx = 80;
			--cy;
			if (cy < 0)
			{
				cx = 0;
				cy = 0;
			}
		}
		vga_move_cursor(cx, cy);
		vga_print_char(' ');
		vga_move_cursor(cx, cy);
	}
	else
		vga_print_char(event.repr);
}

static
void	mouse_callback(mouse_t mouse)
{
	static mouse_t	old_state = {0};
	int				cx;
	int				cy;

	vga_get_cursor(&cx, &cy);
	if (old_state.x != mouse.x)
	{
		vga_move_cursor(0, 0);
		printk("%+10d", mouse.x);
	}
	if (old_state.y != mouse.y)
	{
		vga_move_cursor(12, 0);
		printk("%+10d", mouse.y);
	}
	if (old_state.left_pressed != mouse.left_pressed)
	{
		vga_move_cursor(26, 0);
		vga_print_char_c(' ', mouse.left_pressed ? 0x40 : 0x80);
	}
	if (old_state.middle_pressed != mouse.middle_pressed)
	{
		vga_move_cursor(27, 0);
		vga_print_char_c(' ', mouse.middle_pressed ? 0x40 : 0x80);
	}
	if (old_state.right_pressed != mouse.right_pressed)
	{
		vga_move_cursor(28, 0);
		vga_print_char_c(' ', mouse.right_pressed ? 0x40 : 0x80);
	}
	vga_move_cursor(cx, cy);
	old_state = mouse;
}
