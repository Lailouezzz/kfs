/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irq.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:05:29 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/12 17:02:12 by ale-boud         ###   ########.fr       */
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

#  define DUMP_STACK_FRAME(sf)                                \
    {                                                        \
        interrupt_stack_frame_s    *__asm__ptr__s__ = &(sf);    \
        __asm__ volatile (                                    \
            "pushl %%eax\n"                                    \
            "movl %%edi, %%eax\n"                            \
            "movl %0, %%edi\n"                                \
            "movl %%eax, 24(%%edi)\n"                        \
            "movl %%cr0, %%eax\n"                            \
            "movl %%eax,   (%%edi)\n"                        \
            "movl %%cr3, %%eax\n"                            \
            "movl %%eax,  4(%%edi)\n"                        \
            "popl %%eax\n"                                    \
            "movl %%edx,  8(%%edi)\n"                        \
            "movl %%ecx, 12(%%edi)\n"                        \
            "movl %%ebx, 16(%%edi)\n"                        \
            "movl %%eax, 20(%%edi)\n"                        \
            "movl %%esi, 28(%%edi)\n"                        \
            "movl %%ebp, 32(%%edi)\n"                        \
            "movl %%esp, 36(%%edi)\n"                        \
            :                                                \
            : "r"(__asm__ptr__s__)                            \
            : "eax", "edi"                                    \
        );                                                    \
    }

/* TODO : same TODO has bellow */
#  define HALT() for (;;) __asm__ volatile ("hlt")

extern void	*interrupt_stub_table[IRQS_NR];

void		init_irq(void);

# endif

#endif
