/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irq.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:53:21 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/12 18:59:47 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kfs/kernel.h>
#include <asm/ptrace.h>
#include <asm/irq.h>
#include <asm/idt.h>
#include <asm/pic.h>
#include <asm/io.h>

irq_cb_t	irq_cbs[IRQS_NR] = {
	[0 ... IRQS_NR - 1] = NULL
};

void	init_irq(void)
{
	remap_pic(FIRST_EXTERNAL_VECTOR, FIRST_EXTERNAL_VECTOR+8);
	for (usize k = 0; k < IRQS_NR; ++k)
		idt_set_gate(k+FIRST_EXTERNAL_VECTOR, interrupt_stub_table[k], IDT_INT_GATE);
}

void	request_irq(unsigned int irq, irq_cb_t cb)
{
	if (irq >= IRQS_NR)
		panic("The IRQ%d requested is out of range\n");
	irq_cbs[irq] = cb;
}

void	free_irq(unsigned int irq)
{
	if (irq >= IRQS_NR)
		panic("The IRQ%d requested is out of range\n");
	irq_cbs[irq] = NULL;
}

/**
 * @brief Called from entry.S
 *
 * @param regs The registers pushed on the stack
 */
void	do_irq(int_regs_s regs)
{
	const u8	vector = regs.orig_eax;
	irq_cb_t	cb = irq_cbs[vector];

	printk("do_irq (%d)\n", vector);
	if (cb != NULL)
		cb(vector);
	/* TODO: move pic_send_ack to an abstract structure for futur use (APIC) */
	pic_send_ack(vector);
}
