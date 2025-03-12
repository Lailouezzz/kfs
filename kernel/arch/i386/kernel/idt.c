/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:24:15 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/12 13:13:02 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm/idt.h>
#include <asm/except.h>

extern
void	*const except_isr_table[32];

/* Define the IDT */

idt_entry_t		idt_entries[IDT_ENTRIES_NUM] = { 0 };

idt_pointer_t	idt_pointer = {
	.size = sizeof(idt_entries) - 1,
	.offset = idt_entries
};

void	init_idt(void)
{
	for (usize i = 0; i < 32; ++i)
		idt_entries[i] = IDT_ENTRY(except_isr_table[i], IDT_INT_GATE, 0x08);

	irq_install();

	/* Let's load IDT */
	__asm__ volatile(
		"lidt (%0)"
		:
		: "r" (&idt_pointer)
		:
	);

	return;
}

void	idt_set_gate(u8 number, void *addr, u8 sel, u16 attr)
{
	idt_entries[number] = IDT_ENTRY(addr, attr, sel);
}
