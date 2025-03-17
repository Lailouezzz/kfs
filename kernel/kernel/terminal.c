/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:48:52 by amassias          #+#    #+#             */
/*   Updated: 2025/03/17 01:07:13 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ************************************************************************** //
// *                                                                        * //
// * Includes                                                               * //
// *                                                                        * //
// ************************************************************************** //

#include <asm/keyboard.h>
#include <asm/mouse.h>
#include <kfs/kernel.h>
#include <kfs/string.h>
#include <kfs/terminal.h>
#include <kfs/types.h>
#include <kfs/vga.h>

// ************************************************************************** //
// *                                                                        * //
// * Defines                                                                * //
// *                                                                        * //
// ************************************************************************** //

#define WIDTH (80)
#define HEIGHT (25)
#define DEFAULT_COLOR (0x0F)
#define SCREEN_SIZE (WIDTH * HEIGHT)

#define MAKE_CHAR(color, c) (((color) << 8) | (c))

// ************************************************************************** //
// *                                                                        * //
// * Globals                                                                * //
// *                                                                        * //
// ************************************************************************** //

static u16	cx = 0;
static u16	cy = 0;

// ************************************************************************** //
// *                                                                        * //
// * Header definitions                                                     * //
// *                                                                        * //
// ************************************************************************** //

void	init_terminal(void)
{
	u16			screen_buffer[WIDTH * HEIGHT] = {0};
	const u16	default_colored_char = MAKE_CHAR(DEFAULT_COLOR, ' ');

	for (usize i = 0; i < sizeof(screen_buffer) / sizeof(u16); ++i)
		screen_buffer[i] = default_colored_char;
	vga_set_vram(0, (const u8*)screen_buffer, SCREEN_SIZE * sizeof(u16));
	printk_set_writer(terminal_put_string);
}

void	terminal_put_char(char c)
{
	if (cy >= HEIGHT)
		return ;
	if (c != '\n')
	{
		vga_set_colored_char_at(cx, cy, c, DEFAULT_COLOR);
		++cx;
	}
	if (cx >= WIDTH || c == '\n')
	{
		++cy;
		cx = 0;
	}
	if (cy >= HEIGHT)
	{
		cy = HEIGHT - 1;
		vga_scroll_down();
	}
	vga_move_cursor(cx, cy);
}

// TODO: improve
int	terminal_put_string(const char *str)
{
	int	n;

	n = 0;
	while (str[n])
		terminal_put_char(str[n++]);
	return (n);
}

void	terminal_get_cursor(u8 *x, u8 *y)
{
	*x = cx;
	*y = cy;
}

void	terminal_set_cursor(u8 x, u8 y)
{
	cx = x;
	cy = y;
	vga_move_cursor(x, y);
}

void	terminal_set_char_at(u8 x, u8 y, char c)
{
	vga_set_char_at(x, y, c);
}

void	terminal_set_color_at(u8 x, u8 y, u8 color)
{
	vga_set_color_at(x, y, color);
}

void	terminal_set_colored_char_at(u8 x, u8 y, char c, u8 color)
{
	vga_set_colored_char_at(x, y, c, color);
}
