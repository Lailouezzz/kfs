/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:42:26 by amassias          #+#    #+#             */
/*   Updated: 2025/03/12 14:35:38 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm/generic.h>
#include <kfs/kernel.h>

#include <stdarg.h>

void	panic(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);

	HALT();
}
