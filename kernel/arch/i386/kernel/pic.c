/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pic.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:34:21 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/12 19:12:49 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm/pic.h>
#include <asm/io.h>

void	remap_pic(u8 m_off, u8 s_off)
{
	// Starts the initialization sequence (in cascade mode)
	outb_p(ICW1_INIT | ICW1_ICW4, PIC1_COMMAND);
	outb_p(ICW1_INIT | ICW1_ICW4, PIC2_COMMAND);
	outb_p(m_off, PIC1_DATA);
	outb_p(s_off, PIC2_DATA);
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
