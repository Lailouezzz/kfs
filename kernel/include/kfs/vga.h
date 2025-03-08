/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:53:39 by amassias          #+#    #+#             */
/*   Updated: 2025/03/08 13:00:13 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  KFS_VGA_H
# define KFS_VGA_H

void	vga_setup(void);

u32		vga_get_palette_entry(int i);
void	vga_get_palette(int *buf);

void	vga_set_palette_entry(int i, int color);
void	vga_set_palette(int *buf);

void	vga_move_cursor(int x, int y);
void	vga_enable_cursor(char x, char y);
void	vga_clear_screen(void);

void	vga_set_text_color(int background, int foreground);
void	vga_reset_text_color(void);
int		vga_get_text_color(void);

int		vga_print_char(char c);
int		vga_print_string(const char *str);
int		vga_print_hex(char v);
int		vga_print_bits(char v);

#endif
