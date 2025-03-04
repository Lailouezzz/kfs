/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:53:39 by amassias          #+#    #+#             */
/*   Updated: 2025/03/04 19:32:36 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  KFS_VGA_H
# define KFS_VGA_H

void	vga_setup(void);

void	vga_move_cursor(int x, int y);

void	vga_print_char_c(char c, int color);
void	vga_print_string_c(const char *str, int color);
void	vga_print_hex_c(char v, int color);
void	vga_print_bits_c(char v, int color);

void	vga_print_char(char c);
void	vga_print_string(const char *str);
void	vga_print_hex(char v);
void	vga_print_bits(char v);

#endif
