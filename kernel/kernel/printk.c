/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printk.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:22:18 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/17 01:26:30 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kfs/string.h>
#include <kfs/vga.h>
#include <stdarg.h>

#define PRINTK_BUF_SIZE (4096)

static int	(*volatile printk_writer)(const char *) = NULL;

int	vprintk(
		const char *fmt,
		va_list args
		)
{
	static char	buf[PRINTK_BUF_SIZE + 1] = {0};
	int			n;

	n = 0;
	vsnprintf(buf, PRINTK_BUF_SIZE, fmt, args);
	if (printk_writer)
		n = printk_writer(buf);
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

void	printk_set_writer(int (*writer)(const char *))
{
	printk_writer = writer;
}
