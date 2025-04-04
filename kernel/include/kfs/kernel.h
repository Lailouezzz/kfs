/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:18:09 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/16 19:35:02 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  KFS_KERNEL_H
# define KFS_KERNEL_H

# include <stdarg.h>

int		vprintk(const char *fmt, va_list list);
int		printk(const char *fmt, ...);
void	printk_set_writer(int (*writer)(const char *));
void	panic(const char *fmt, ...);

#endif
