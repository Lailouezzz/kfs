/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:27:17 by amassias          #+#    #+#             */
/*   Updated: 2025/03/16 23:31:12 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  KFS_TERMINAL_H
# define KFS_TERMINAL_H

# include <kfs/types.h>

void	init_terminal(void);

void	terminal_put_char(char c);
int		terminal_put_string(const char *str);

void	terminal_get_cursor(u8 *x, u8 *y);

void	terminal_set_cursor(u8 x, u8 y);
void	terminal_set_char_at(u8 x, u8 y, char c);
void	terminal_set_color_at(u8 x, u8 y, u8 color);
void	terminal_set_colored_char_at(u8 x, u8 y, char c, u8 color);

#endif
