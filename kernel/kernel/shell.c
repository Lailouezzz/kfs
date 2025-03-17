/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Antoine Massias <massias.antoine.pro@gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:43:24 by amassias          #+#    #+#             */
/*   Updated: 2025/03/17 17:36:33 by Antoine Mas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ************************************************************************** //
// *                                                                        * //
// * Includes                                                               * //
// *                                                                        * //
// ************************************************************************** //

#include <multiboot2.h>

#include <asm/io.h>
#include <asm/keyboard.h>
#include <kfs/kernel.h>
#include <kfs/string.h>
#include <kfs/terminal.h>

// ************************************************************************** //
// *                                                                        * //
// * Defines                                                                * //
// *                                                                        * //
// ************************************************************************** //

#define INPUT_BUFFER_SIZE (80 - 3)

// ************************************************************************** //
// *                                                                        * //
// * Globals                                                                * //
// *                                                                        * //
// ************************************************************************** //

volatile char	input_buffer[INPUT_BUFFER_SIZE + 1];
volatile char	*input_buffer_end;
volatile int	process;

extern struct multiboot_info *g_mb_info;

// ************************************************************************** //
// *                                                                        * //
// * Helper prototypes                                                      * //
// *                                                                        * //
// ************************************************************************** //

static
void	keyboard_callback(keyboard_event_t keyboard);

static
const char	*get_memory_type(u32 type);

static
void	print_memory(void);

static
void	process_command(const char *command);

// ************************************************************************** //
// *                                                                        * //
// * Header definition                                                      * //
// *                                                                        * //
// ************************************************************************** //

void	start_shell(void)
{
	u8	cx;
	u8	cy;

	keyboard_set_callback(keyboard_callback);
	while (1)
	{
		input_buffer[0] = '\0';
		input_buffer_end = input_buffer;
		process = 0;
		terminal_get_cursor(&cx, &cy);
		if (cx != 0)
		{
			terminal_put_char('\n');
			terminal_get_cursor(&cx, &cy);
		}
		printk("> ");
		while (!process)
			__asm__("pause");
		process_command((const char *)input_buffer);
	}
}

// ************************************************************************** //
// *                                                                        * //
// * Helper definition                                                      * //
// *                                                                        * //
// ************************************************************************** //

static
void	keyboard_callback(keyboard_event_t keyboard)
{
	u8	cx;
	u8	cy;

	if (!keyboard.pressed || keyboard.keycode > KBD_ENTER)
		return ;
	if (keyboard.keycode == KBD_ENTER)
	{
		terminal_put_char('\n');
		process = 1;
		return ;
	}
	else if (keyboard.keycode == KBD_BACKSPACE)
	{
		if (input_buffer_end != input_buffer)
		{
			--input_buffer_end;
			*input_buffer_end = '\0';
			terminal_get_cursor(&cx, &cy);
			terminal_set_char_at(cx - 1, cy, ' ');
			terminal_set_cursor(cx - 1, cy);
		}
		return ;
	}
	else if (keyboard.keycode == KBD_TAB)
		keyboard.repr = ' ';
	if (input_buffer_end >= input_buffer + INPUT_BUFFER_SIZE)
		return ;
	*input_buffer_end++ = keyboard.repr;
	*input_buffer_end = '\0';
	terminal_put_char(keyboard.repr);
}

static
const char	*get_memory_type(u32 type)
{
	switch (type)
	{
		case 1:  return ("AVAILABLE");
		case 2:  return ("RESERVED");
		default: return ("UNKNOWN");
	}
}

static
void	print_memory(void)
{
	struct multiboot_tag_mmap	*mmap;
	multiboot_memory_map_t		*entry;
	usize						i;

	mmap = NULL;
	FOREACH_MULTIBOOT_TAG(tag, g_mb_info)
	{
		if (tag->type != MULTIBOOT_TAG_TYPE_MMAP)
			continue ;
		mmap = (struct multiboot_tag_mmap *)tag;
		break ;
	}
	if (mmap == NULL)
	{
		printk("No memory map available...\n");
		return ;
	}
	entry = mmap->entries;
	printk(" %21s | %21s | %9s\n", "addr", "len", "type");
	printk("-----------------------+-----------------------+-----------\n");
	for (i = 0; i < mmap->size - sizeof(*mmap); i += mmap->entry_size, ++entry)
		printk(" %21llx | %21llu | %9s\n",
			entry->addr,
			entry->len,
			get_memory_type(entry->type)
		);
}

static
void	process_command(const char *command)
{
	if (strcmp(command, "exit") == 0)
		outw(0x2000, 0x604); // TODO: Evil hack to shutdown QEMU ! Use ACPI...
	else if (strcmp(command, "ping") == 0)
		printk("pong\n");
	else if (strcmp(command, "memorymap") == 0)
		print_memory();
	else
		printk("Unknown command \"%s\"\n", command);
}
