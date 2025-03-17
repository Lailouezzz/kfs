/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Antoine Massias <massias.antoine.pro@gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:19:39 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/17 13:23:38 by Antoine Mas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ASM_IDT_H
# define ASM_IDT_H

# include <kfs/compiler.h>

# include <asm/types.h>
# include <asm/memory.h>

# define IDT_INT_GATE 0x8E
# define IDT_TRAP_GATE 0x8F
# define IDT_ENTRIES_NR 256

# define IDT_ENTRY(off, attr, sel)						\
	(idt_entry_s)										\
	{													\
		.offset_1 = ((uaddr)(off)) & 0xFFFF,			\
		.offset_2 = (((uaddr)(off)) >> 16) & 0xFFFF,	\
		.type_attributes = (attr),						\
		.selector = (sel),								\
		.zero = 0										\
	}


typedef struct
{
	u16	offset_1;        // offset bits 0..15
	u16	selector;        // a code segment selector in GDT or LDT
	u8	zero;            // unused, set to 0
	u8	type_attributes; // gate type, dpl, and p fields
	u16	offset_2;        // offset bits 16..31
}	PACKED idt_entry_s;

typedef struct
{
	u16			size;
	idt_entry_s	*offset;
}	PACKED idt_pointer_s;

extern idt_entry_s		idt_entries[IDT_ENTRIES_NR];
extern idt_pointer_s	idt_pointer;

void	init_idt(void);

static inline void ALWAYS_INLINE	idt_set_gate(
										u8 number,
										void *addr,
										u16 attr
									)
{
	idt_entries[number] = IDT_ENTRY(addr, attr, 0x08);
}

static inline void ALWAYS_INLINE	load_idt(void)
{
	__asm__ volatile(
		"lidt (%0)"
		:
		: "r" (&idt_pointer)
		:
	);
}

#endif
