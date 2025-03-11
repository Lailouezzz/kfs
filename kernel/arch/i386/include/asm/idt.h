/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:19:39 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/11 16:34:57 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ASM_IDT_H
# define ASM_IDT_H

# include <asm/except.h>
# include <asm/types.h>
# include <asm/memory.h>

# define IDT_INT_GATE 0x8E
# define IDT_TRAP_GATE 0xEF
# define IDT_ENTRIES_NUM 256

#define IDT_ENTRY(off, attr, sel)						\
	(idt_entry_t)										\
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
}	__PACKED__ idt_entry_t;

typedef struct
{
	u16			size;
	idt_entry_t	*offset;
}	__PACKED__ idt_pointer_t;

extern
idt_entry_t		idt_entries[IDT_ENTRIES_NUM];

extern
idt_pointer_t	idt_pointer;

void	init_idt(void);

#endif
