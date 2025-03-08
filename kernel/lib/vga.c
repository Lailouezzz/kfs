/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:56:25 by amassias          #+#    #+#             */
/*   Updated: 2025/03/08 13:38:54 by amassias         ###   ########.fr       */
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

#define R__DAC_ADDRESS_WRITE_MODE (0x3C8)
#define R__DAC_ADDRESS_READ_MODE (0x3C7)
#define R__DAC_DATA (0x3C9)
#define R__DAC_STATE (0x3C7)

#define R__CONTROL (0x3D4)
#define R__DATA (0x3D5)

#define COLOR_PALETTE_SIZE (256)

#define TEXT_WIDTH (80)
#define TEXT_HEIGHT (25)

#define HEX_PREFIX "0x"
#define HEX_DIGITS "0123456789ABCDEF"

#define BIN_PREFIX "0b"
#define BIN_DIGITS "0123456789ABCDEF"

#define TEXT_CHAR(c) (((text_color & 0xFF) << 8) | (c))
#define CLEAR_COLOR (0x0f)

// ************************************************************************** //
// *                                                                        * //
// * Globals        .                                                       * //
// *                                                                        * //
// ************************************************************************** //

static unsigned char	cx = 0;
static unsigned char	cy = 0;
static int				text_mode = 0;
static unsigned short	*video;
static unsigned char	text_color = CLEAR_COLOR;

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

u32		vga_get_palette_entry(
			int i
			)
{
	int		r;
	int		g;
	int		b;

	if (i < 0 || i > COLOR_PALETTE_SIZE)
		return (0);
	outb(i, R__DAC_ADDRESS_READ_MODE);
	r = inb(R__DAC_DATA);
	g = inb(R__DAC_DATA);
	b = inb(R__DAC_DATA);
	return ((r << 16) | (g << 8) | (b << 0));
}

void	vga_set_palette_entry(
			int i,
			int color
			)
{
	if (i < 0 || i > COLOR_PALETTE_SIZE)
		return ;
	outb(i, R__DAC_ADDRESS_WRITE_MODE);
	outb((unsigned char)((color >> 16) & 0xFF), R__DAC_DATA);
	outb((unsigned char)((color >>  8) & 0xFF), R__DAC_DATA);
	outb((unsigned char)((color >>  0) & 0xFF), R__DAC_DATA);
}

void	vga_get_palette(
			int *buf
			)
{
	outb(0, R__DAC_ADDRESS_READ_MODE);
	for (int i = 0; i < COLOR_PALETTE_SIZE; ++i)
		buf[i] = inb(R__DAC_DATA);
}

void	vga_set_palette(
			int *buf
			)
{
	struct color {
		unsigned char __padd;
		unsigned char r;
		unsigned char g;
		unsigned char b;
	}	*color;

	outb(0, R__DAC_ADDRESS_WRITE_MODE);
	for (int i = 0; i < COLOR_PALETTE_SIZE; ++i)
	{
		color = (struct color *)&buf[i];
		outb(color->r, R__DAC_DATA);
		outb(color->g, R__DAC_DATA);
		outb(color->b, R__DAC_DATA);
	}
}

void	vga_move_cursor(
			int x,
			int y
			)
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x >= TEXT_WIDTH) x = TEXT_WIDTH - 1;
	if (y >= TEXT_HEIGHT) y = TEXT_HEIGHT - 1;

	cx = x;
	cy = y;

	unsigned short pos = y * TEXT_WIDTH + x;

	outb(0x0F, R__CONTROL);
	outb((unsigned char) (pos & 0xFF), R__DATA);
	outb(0x0E, R__CONTROL);
	outb((unsigned char) ((pos >> 8) & 0xFF), R__DATA);
}

void	vga_clear_screen(void)
{
	for (int k = 0; k < TEXT_WIDTH * TEXT_HEIGHT; ++k)
		video[k] = CLEAR_COLOR << 8;
	vga_move_cursor(0, 0);
}

void	vga_enable_cursor(
			char cursor_start,
			char cursor_end
			)
{
	outb(0x0A, R__CONTROL);
	outb((inb(R__DATA) & 0xC0) | cursor_start, R__DATA);
	outb(0x0B, R__CONTROL);
	outb((inb(R__DATA) & 0xE0) | cursor_end, R__DATA);
}

void	vga_set_text_color(
			int background,
			int foreground
			)
{
	text_color = ((background & 0xf) << 4) | (foreground & 0xf);
}

void	vga_reset_text_color(void)
{
	text_color = CLEAR_COLOR;
}

int		vga_get_text_color(void)
{
	return (text_color);
}

int		vga_print_char(
			char c
			)
{
	if (c != '\n')
		video[cx++ + TEXT_WIDTH * cy] = TEXT_CHAR(c);
	if (cx >= TEXT_WIDTH || c == '\n')
	{
		cx = 0;
		++cy;
	}
	if (cy >= TEXT_HEIGHT)
	{
		// Scroll
		cy = TEXT_HEIGHT - 1;
		memmove(&video[0], &video[TEXT_WIDTH], TEXT_WIDTH * (TEXT_HEIGHT - 1) * sizeof(u16));
		for (int k = TEXT_WIDTH * (TEXT_HEIGHT - 1); k < TEXT_WIDTH * TEXT_HEIGHT; ++k)
			video[k] = CLEAR_COLOR << 8;
	}
	vga_move_cursor(cx, cy);
	return (1);
}

int		vga_print_string(
			const char *str
			)
{
	unsigned int	i;

	for (i = 0; str[i]; ++i)
		vga_print_char(str[i]);
	return (i);
}

int		vga_print_hex(
			char v
			)
{
	vga_print_string(HEX_PREFIX);
	vga_print_char(HEX_DIGITS[(v & 0xf0) >> 4]);
	vga_print_char(HEX_DIGITS[(v & 0x0f) >> 0]);
	return (4);
}

int		vga_print_bits(
			char v
			)
{
	vga_print_string(BIN_PREFIX);
	for (int i = 7; i >= 0; --i)
		vga_print_char(BIN_DIGITS[(v >> i) & 1]);
	return (10);
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
