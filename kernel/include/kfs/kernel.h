/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:18:09 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/11 16:49:03 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  KFS_KERNEL_H
# define KFS_KERNEL_H

# include <asm/except.h>
# include <stdarg.h>

int		vprintk(const char *fmt, va_list list);
int		printk(const char *fmt, ...);
void	panic(interrupt_stack_frame_t stack_frame, const char *fmt, ...);

#endif
