/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:32:26 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/17 00:11:24 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ASM_IO_H
# define ASM_IO_H

# ifndef __ASSEMBLY__

#  define outb(value,port) \
	__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))

#  define outw(value,port) \
	__asm__ ("outw %%ax,%%dx"::"a" (value),"d" (port))

#  define inb(port) ({ \
	unsigned char _v; \
	__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port)); \
	_v; \
	})

/* *_p are the IO who need delay */

#  define outb_p(value,port) \
	__asm__ ("outb %%al,%%dx\n" \
			"\tjmp 1f\n" \
			"1:\tjmp 1f\n" \
			"1:"::"a" (value),"d" (port))

#  define inb_p(port) ({ \
	unsigned char _v; \
	__asm__ volatile ("inb %%dx,%%al\n" \
		"\tjmp 1f\n" \
		"1:\tjmp 1f\n" \
		"1:":"=a" (_v):"d" (port)); \
	_v; \
	})

# endif // ifndef __ASSEMBLY__

#endif
