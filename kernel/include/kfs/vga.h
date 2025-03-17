/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:53:39 by amassias          #+#    #+#             */
/*   Updated: 2025/03/16 23:14:46 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  KFS_VGA_H
# define KFS_VGA_H

# include <kfs/types.h>

void	init_vga(void);

void	vga_move_cursor(int x, int y);
void	vga_enable_cursor(char x, char y);
void	vga_clear_screen(void);
void	vga_scroll_down(void);

void	vga_set_vram(usize offset, const u8 *data, usize size);
void	vga_set_char_at(u8 x, u8 y, char c);
void	vga_set_color_at(u8 x, u8 y, u8 color);
void	vga_set_colored_char_at(u8 x, u8 y, char c, u8 color);

#endif
