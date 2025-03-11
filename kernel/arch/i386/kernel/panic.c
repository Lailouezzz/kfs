/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:42:26 by amassias          #+#    #+#             */
/*   Updated: 2025/03/11 18:53:27 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm/except.h>
#include <kfs/kernel.h>

#include <stdarg.h>

void	panic(interrupt_stack_frame_t stack_frame, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);

	printk("\n--- INTERRUPT STACK FRAME :\n");
	printk("EAX=0x%08x  EBX=0x%08x  ECX=0x%08x  EDX=0x%08x\n",
			stack_frame.eax, stack_frame.ebx, stack_frame.ecx, stack_frame.edx);
	printk("EDI=0x%08x  ESI=0x%08x  EBP=0x%08x  ESP=0x%08x\n",
			stack_frame.edi, stack_frame.esi, stack_frame.ebp, stack_frame.esp);
	printk("CR0=0x%08x  CR3=0x%08x\n",
			stack_frame.cr0, stack_frame.cr3);
	printk("---\n");

	HALT();
}
