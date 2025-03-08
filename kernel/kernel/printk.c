/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printk.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:22:18 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/07 20:23:37 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kfs/string.h>
#include <kfs/vga.h>
#include <stdarg.h>

#define PRINTK_BUF_SIZE (4096)

int	vprintk(
		const char *fmt,
		va_list args
		)
{
	static char	buf[PRINTK_BUF_SIZE];
	int			n;

	n = vsnprintf(buf, PRINTK_BUF_SIZE, fmt, args);
	vga_print_string(buf);
	return (n);
}

int	printk(
		const char *fmt,
		...
		)
{
	va_list	args;
	int		n;

	va_start(args, fmt);
	n = vprintk(fmt, args);
	va_end(args);
	return (n);
}
