/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:56:25 by amassias          #+#    #+#             */
/*   Updated: 2025/03/17 00:06:53 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ************************************************************************** //
// *                                                                        * //
// * Includes                                                               * //
// *                                                                        * //
// ************************************************************************** //

#include <asm/io.h>
#include <kfs/string.h>
#include <kfs/vga.h>

// ************************************************************************** //
// *                                                                        * //
// * Defines        .                                                       * //
// *                                                                        * //
// ************************************************************************** //

#define WIDTH (80)
#define HEIGHT (25)

#define HEX_PREFIX "0x"
#define HEX_DIGITS "0123456789ABCDEF"

#define BIN_PREFIX "0b"
#define BIN_DIGITS "0123456789ABCDEF"

#define DEFAULT_COLOR (0x0f)

// ************************************************************************** //
// *                                                                        * //
// * Globals        .                                                       * //
// *                                                                        * //
// ************************************************************************** //

static int				text_mode = 0;
static unsigned short	*video;

// ************************************************************************** //
// *                                                                        * //
// * Helper prototypes                                                      * //
// *                                                                        * //
// ************************************************************************** //

static
void	get_video_address(void);

// ************************************************************************** //
// *                                                                        * //
// * Header function.                                                       * //
// *                                                                        * //
// ************************************************************************** //

void	init_vga(void)
{
	get_video_address();
	vga_clear_screen();
	vga_enable_cursor(13, 13);
}

void	vga_move_cursor(
			int x,
			int y
			)
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x >= WIDTH) x = WIDTH - 1;
	if (y >= HEIGHT) y = HEIGHT - 1;

	unsigned short pos = y * WIDTH + x;

	outb(0x0F, 0x3D4);
	outb((unsigned char) (pos & 0xFF), 0x3D5);
	outb(0x0E, 0x3D4);
	outb((unsigned char) ((pos >> 8) & 0xFF), 0x3D5);
}

void	vga_clear_screen(void)
{
	for (int k = 0; k < WIDTH * HEIGHT; ++k)
		video[k] = 0x0F00;
	vga_move_cursor(0, 0);
}

void	vga_enable_cursor(
			char cursor_start,
			char cursor_end
			)
{
	outb(0x0A, 0x3D4);
	outb((inb(0x3D5) & 0xC0) | cursor_start, 0x3D5);
	outb(0x0B, 0x3D4);
	outb((inb(0x3D5) & 0xE0) | cursor_end, 0x3D5);
}

void	vga_scroll_down(void)
{
	memmove(&video[0], &video[WIDTH], WIDTH * (HEIGHT - 1) * sizeof(u16));
	for (int k = WIDTH * (HEIGHT - 1); k < WIDTH * HEIGHT; ++k)
		video[k] = 0x0F00;
}

void	vga_set_vram(usize offset, const u8 *data, usize size)
{
	memcpy(((volatile u8 *)video) + offset, data, size);
}

void	vga_set_char_at(u8 x, u8 y, char c)
{
	u16	cc;

	cc = video[x + WIDTH * y];
	cc = (cc & 0xFF00) | c;
	video[x + WIDTH * y] = cc;
}

void	vga_set_color_at(u8 x, u8 y, u8 color)
{
	u16	cc;

	cc = video[x + WIDTH * y];
	cc = (color << 8) | (cc & 0xFF);
	video[x + WIDTH * y] = cc;
}

void	vga_set_colored_char_at(u8 x, u8 y, char c, u8 color)
{
	video[x + WIDTH * y] = (color << 8) | c;
}

// ************************************************************************** //
// *                                                                        * //
// * Helper definitions                                                     * //
// *                                                                        * //
// ************************************************************************** //

static
void	get_video_address(void)
{
	outb(0x06, 0x3CE);
	const unsigned char mgr = inb_p(0x3CF);
	const unsigned char mode = (mgr >> 2) & 0b11;

	unsigned short *video_addresses[] = {
		(unsigned short *)0xA0000,
		(unsigned short *)0xA0000,
		(unsigned short *)0xB0000,
		(unsigned short *)0xB8000
	};

	text_mode = mode == 0b11;
	video = video_addresses[mode];
}
