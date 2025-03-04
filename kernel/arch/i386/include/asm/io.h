/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:32:26 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/04 18:40:03 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ASM_IO_H
# define ASM_IO_H

# ifndef __ASSEMBLY__

#  define outb(value,port) \
	__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))


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
