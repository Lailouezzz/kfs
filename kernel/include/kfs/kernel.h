/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:18:09 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/12 14:04:10 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  KFS_KERNEL_H
# define KFS_KERNEL_H

# include <asm/irq.h>
# include <stdarg.h>

int		vprintk(const char *fmt, va_list list);
int		printk(const char *fmt, ...);
void	panic(interrupt_stack_frame_s stack_frame, const char *fmt, ...);

#endif
