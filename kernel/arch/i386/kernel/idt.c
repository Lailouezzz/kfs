/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:24:15 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/11 16:51:57 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm/idt.h>

/* Define the IDT */

idt_entry_t		idt_entries[IDT_ENTRIES_NUM] = { 0 };

idt_pointer_t	idt_pointer = {
	.size = sizeof(idt_entries) - 1,
	.offset = idt_entries
};

void	init_idt(void)
{
	/* Set exceptions entries */
	for (usize i = 0; i < 3; ++i)
		idt_entries[i] = IDT_ENTRY(except_isr_table[i], IDT_INT_GATE, 0x08);

	idt_entries[3] = IDT_ENTRY(except_isr_table[3], IDT_TRAP_GATE, 0x08);
	idt_entries[4] = IDT_ENTRY(except_isr_table[4], IDT_TRAP_GATE, 0x08);

	for (usize i = 5; i < sizeof(except_isr_table) / sizeof(*except_isr_table); ++i)
		idt_entries[i] = IDT_ENTRY(except_isr_table[i], IDT_INT_GATE, 0x08);

	/* TODO : insert syscall interrupt */

	/* Let's load IDT */
	__asm__ volatile(
		"lidt (%0)"
		:
		: "r" (&idt_pointer)
		:
	);

	return;
}
