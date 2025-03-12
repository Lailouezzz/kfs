/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irq.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:05:29 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/12 18:57:27 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ASM_IRQ_H
# define ASM_IRQ_H

# include <kfs/compiler.h>

# include <asm/memory.h>
# include <asm/types.h>

# define FIRST_EXTERNAL_VECTOR	0x20
# define SYSCALL_VECTOR			0x80

# define IRQS_NR				16

# ifndef __ASSEMBLY__

typedef void(*irq_cb_t)(unsigned int);

/**
 * @brief Low level routines to jump in C
 *
 */
extern void	*interrupt_stub_table[IRQS_NR];

void		init_irq(void);

void		request_irq(unsigned int irq, irq_cb_t cb);

# endif

#endif
