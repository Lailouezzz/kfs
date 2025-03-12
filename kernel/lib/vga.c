/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:56:25 by amassias          #+#    #+#             */
/*   Updated: 2025/03/12 17:16:26 by amassias         ###   ########.fr       */
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

static unsigned char	cx = 0;
static unsigned char	cy = 0;
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

void	vga_setup(void)
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

	cx = x;
	cy = y;

	unsigned short pos = y * WIDTH + x;

	outb(0x0F, 0x3D4);
	outb((unsigned char) (pos & 0xFF), 0x3D5);
	outb(0x0E, 0x3D4);
	outb((unsigned char) ((pos >> 8) & 0xFF), 0x3D5);
}

void	vga_get_cursor(
			int *x,
			int *y
		)
{
	*x = cx;
	*y = cy;
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


int		vga_print_char_c(
			char c,
			int color
			)
{
	if (c != '\n')
		video[cx++ + WIDTH * cy] = ((color & 0xff) << 8) | c;
	if (cx >= WIDTH || c == '\n')
	{
		cx = 0;
		++cy;
	}
	if (cy >= HEIGHT)
	{
		// Scroll
		cy = HEIGHT - 1;
		memmove(&video[0], &video[WIDTH], WIDTH * (HEIGHT - 1) * sizeof(u16));
		for (int k = WIDTH * (HEIGHT - 1); k < WIDTH * HEIGHT; ++k)
			video[k] = 0x0F00;
	}
	vga_move_cursor(cx, cy);
	return (1);
}

int		vga_print_string_c(
			const char *str,
			int color
			)
{
	unsigned int	i;

	for (i = 0; str[i]; ++i)
		vga_print_char_c(str[i], color);
	return (i);
}

int		vga_print_hex_c(
			char v,
			int color
			)
{
	vga_print_string_c(HEX_PREFIX, color);
	vga_print_char_c(HEX_DIGITS[(v & 0xf0) >> 4], color);
	vga_print_char_c(HEX_DIGITS[(v & 0x0f) >> 0], color);
	return (4);
}

int		vga_print_bits_c(
			char v,
			int color
			)
{
	vga_print_string_c(BIN_PREFIX, color);
	for (int i = 7; i >= 0; --i)
		vga_print_char_c(BIN_DIGITS[(v >> i) & 1], color);
	return (10);
}

int		vga_print_char(
			char c
			)
{
	return (vga_print_char_c(c, DEFAULT_COLOR));
}

int		vga_print_string(
			const char *str
			)
{
	return (vga_print_string_c(str, DEFAULT_COLOR));
}

int		vga_print_hex(
			char v
			)
{
	return (vga_print_hex_c(v, DEFAULT_COLOR));
}

int		vga_print_bits(
			char v
			)
{
	return (vga_print_bits_c(v, DEFAULT_COLOR));
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
