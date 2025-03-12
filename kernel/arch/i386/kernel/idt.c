/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:24:15 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/12 17:17:40 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm/idt.h>

extern void	*const except_handler_table[32];

/* Define the IDT */

idt_entry_s		idt_entries[IDT_ENTRIES_NR] = { 0 };

idt_pointer_s	idt_pointer = {
	.size = sizeof(idt_entries) - 1,
	.offset = idt_entries
};

void	init_idt(void)
{
	for (usize i = 0; i < 3; ++i)
		idt_set_gate(i, except_handler_table[i], IDT_INT_GATE);
	idt_set_gate(3, except_handler_table[3], IDT_TRAP_GATE);
	idt_set_gate(4, except_handler_table[4], IDT_TRAP_GATE);
	for (usize i = 5; i < sizeof(except_handler_table) / sizeof(*except_handler_table); ++i)
		idt_set_gate(i, except_handler_table[i], IDT_INT_GATE);

	load_idt();
}
