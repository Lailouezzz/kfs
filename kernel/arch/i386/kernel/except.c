/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   except.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:12:28 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/12 17:00:00 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ************************************************************************** //
// *                                                                        * //
// * Includes                                                               * //
// *                                                                        * //
// ************************************************************************** //

#include <asm/except.h>
#include <asm/idt.h>
#include <asm/io.h>
#include <kfs/kernel.h>
#include <kfs/types.h>
#include <kfs/vga.h>

// ************************************************************************** //
// *                                                                        * //
// * Externs                                                                * //
// *                                                                        * //
// ************************************************************************** //

extern void	irq0(void);
extern void	irq1(void);
extern void	irq2(void);
extern void	irq3(void);
extern void	irq4(void);
extern void	irq5(void);
extern void	irq6(void);
extern void	irq7(void);
extern void	irq8(void);
extern void	irq9(void);
extern void	irq10(void);
extern void	irq11(void);
extern void	irq12(void);
extern void	irq13(void);
extern void	irq14(void);
extern void	irq15(void);

// ************************************************************************** //
// *                                                                        * //
// * Helpers                                                                * //
// *                                                                        * //
// ************************************************************************** //

static
void	irq_remap(void);

// ************************************************************************** //
// *                                                                        * //
// * Globals                                                                * //
// *                                                                        * //
// ************************************************************************** //

static
void	*irq_routines[16] = {0};

static
void	*isr_routines[256] = {0};

// ************************************************************************** //
// *                                                                        * //
// * Header definitions                                                     * //
// *                                                                        * //
// ************************************************************************** //

void	irq_install_handler(int irq, exception_handler_t handler)
{
	irq_routines[irq] = handler;
}

void	irq_uninstall_handler(int irq)
{
	irq_routines[irq] = NULL;
}

void	isr_install_handler(int isr, exception_handler_t handler)
{
	isr_routines[isr] = handler;
}

void	isr_uninstall_handler(int isr)
{
	isr_routines[isr] = NULL;
}

void	irq_install(void)
{
	irq_remap();

	idt_set_gate(32 +  0, irq0 , 0x08, IDT_INT_GATE);
	idt_set_gate(32 +  1, irq1 , 0x08, IDT_INT_GATE);
	idt_set_gate(32 +  2, irq2 , 0x08, IDT_INT_GATE);
	idt_set_gate(32 +  3, irq3 , 0x08, IDT_INT_GATE);
	idt_set_gate(32 +  4, irq4 , 0x08, IDT_INT_GATE);
	idt_set_gate(32 +  5, irq5 , 0x08, IDT_INT_GATE);
	idt_set_gate(32 +  6, irq6 , 0x08, IDT_INT_GATE);
	idt_set_gate(32 +  7, irq7 , 0x08, IDT_INT_GATE);
	idt_set_gate(32 +  8, irq8 , 0x08, IDT_INT_GATE);
	idt_set_gate(32 +  9, irq9 , 0x08, IDT_INT_GATE);
	idt_set_gate(32 + 10, irq10, 0x08, IDT_INT_GATE);
	idt_set_gate(32 + 11, irq11, 0x08, IDT_INT_GATE);
	idt_set_gate(32 + 12, irq12, 0x08, IDT_INT_GATE);
	idt_set_gate(32 + 13, irq13, 0x08, IDT_INT_GATE);
	idt_set_gate(32 + 14, irq14, 0x08, IDT_INT_GATE);
	idt_set_gate(32 + 15, irq15, 0x08, IDT_INT_GATE);
}

void	isr_handler(interrupt_stack_frame_t stack_frame)
{
	exception_handler_t	handler;

	handler = (exception_handler_t)irq_routines[stack_frame.intnum];
	if (handler != NULL)
		handler(&stack_frame);
}

void	irq_handler(interrupt_stack_frame_t stack_frame)
{
	exception_handler_t	handler;

	if (32 <= stack_frame.intnum && stack_frame.intnum < 48)
	{
		handler = (exception_handler_t)irq_routines[stack_frame.intnum - 32];
		if (handler != NULL)
			handler(&stack_frame);
	}
	if (stack_frame.intnum >= 40)
		outb(0x20, PIC2_COMMAND);
	outb(0x20, PIC1_COMMAND);
}

// ************************************************************************** //
// *                                                                        * //
// * Helper definitions                                                     * //
// *                                                                        * //
// ************************************************************************** //

static
void	irq_remap(void)
{
	// Starts the initialization sequence (in cascade mode)
	outb_p(ICW1_INIT | ICW1_ICW4, PIC1_COMMAND);
	outb_p(ICW1_INIT | ICW1_ICW4, PIC2_COMMAND);
	outb_p(0x20, PIC1_DATA);
	outb_p(0x28, PIC2_DATA);
	// Tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb_p(4, PIC1_DATA);
	// Tell Slave PIC its cascade identity (0000 0010)
	outb_p(2, PIC2_DATA);

	// Have the PICs use 8086 mode (and not 8080 mode)
	outb(ICW4_8086, PIC1_DATA);
	outb(ICW4_8086, PIC2_DATA);

	// Unmask both PICs.
	outb(0, PIC1_DATA);
	outb(0, PIC2_DATA);
}
