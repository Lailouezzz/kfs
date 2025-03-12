/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irq.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:53:21 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/12 18:19:48 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kfs/kernel.h>
#include <asm/ptrace.h>
#include <asm/irq.h>
#include <asm/idt.h>
#include <asm/pic.h>
#include <asm/io.h>

void	init_irq(void)
{
	remap_pic(FIRST_EXTERNAL_VECTOR, FIRST_EXTERNAL_VECTOR+8);
	for (usize k = 0; k < IRQS_NR; ++k)
		idt_set_gate(k+FIRST_EXTERNAL_VECTOR, interrupt_stub_table[k], IDT_INT_GATE);
}

/**
 * @brief Called from entry.S
 *
 * @param regs The registers pushed on the stack
 */
void	do_irq(int_regs_s regs)
{
	const u8	vector = regs.orig_eax;
	/* TODO: a irq descriptor table to retrieve callbacks and flags etc etc */

	printk("do_irq (%d)\n", vector);
	/* TODO: move pic_send_ack to an abstract structure for futur use (APIC) */
	pic_send_ack(vector);
}
