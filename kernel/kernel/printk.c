/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printk.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:22:18 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/05 18:31:17 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kfs/kernel.h>
#include <kfs/vga.h>
#include <stdarg.h>

static
int	_print_int(
		int n
		);

static
int	_print_hex(
		unsigned int n
		);

int	printk(
		const char *fmt,
		...
		)
{
	int	n = 0;

	// d x s c

	va_list args;
	va_start(args, fmt);

	while (*fmt)
	{
		if (*fmt++ != '%')
		{
			vga_print_char(fmt[-1]);
			++n;
			continue ;
		}
		if (*fmt == '\0')
			break ;
		switch (*fmt++)
		{
			case '%':
				n += vga_print_char('%');
				break ;
			case 'c':
				n += vga_print_char((char)va_arg(args, int));
				break ;
			case 'd':
				n += _print_int(va_arg(args, int));
				break ;
			case 's':
				n += vga_print_string(va_arg(args, char *));
				break ;
			case 'x':
				n += _print_hex(va_arg(args, unsigned int));
				break ;
			default:
				break ;
		}
	}
	va_end(args);
	return (n);
}

static
int	__print_int(unsigned int n)
{
	if (n > 9)
		return (_print_int(n / 10) + vga_print_char('0' + (n % 10)));
	return (vga_print_char('0' + (n % 10)));
}

static
int	_print_int(
		int n
		)
{
	int r = 0;
	unsigned int _n = *(unsigned int *)&n;

	if (n < 0)
	{
		r += vga_print_char('-');
		_n = 1 << 31;
	}
	return (__print_int(_n));
}

static
int	_print_hex(
		unsigned int n
		)
{
	if (n >= 16)
		return (_print_hex(n / 16) + vga_print_char("0123456789QBCDEF"[n % 16]));
	return (vga_print_char("0123456789QBCDEF"[n % 16]));
}
