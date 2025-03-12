/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:53:39 by amassias          #+#    #+#             */
/*   Updated: 2025/03/12 17:16:28 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  KFS_VGA_H
# define KFS_VGA_H

void	vga_setup(void);

void	vga_move_cursor(int x, int y);
void	vga_get_cursor(int *x, int *y);
void	vga_enable_cursor(char x, char y);
void	vga_clear_screen(void);

int		vga_print_char_c(char c, int color);
int		vga_print_string_c(const char *str, int color);
int		vga_print_hex_c(char v, int color);
int		vga_print_bits_c(char v, int color);

int		vga_print_char(char c);
int		vga_print_string(const char *str);
int		vga_print_hex(char v);
int		vga_print_bits(char v);

#endif
