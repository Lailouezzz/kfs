/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:18:09 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/07 20:13:37 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  KERNEL_H
# define KERNEL_H

# include <stdarg.h>

int	vprintk(const char *fmt, va_list list);
int	printk(const char *fmt, ...);

#endif
