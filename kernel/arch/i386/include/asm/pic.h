/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pic.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:30:12 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/12 13:38:15 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ASM_PIC_H
# define ASM_PIC_H

# include <kfs/types.h>

# define PIC1						(0x20)	/* IO base address for master PIC */
# define PIC2						(0xA0)	/* IO base address for slave PIC  */

# define PIC1_COMMAND				PIC1
# define PIC1_DATA					(PIC1+1)
# define PIC2_COMMAND				PIC2
# define PIC2_DATA					(PIC2+1)

# define PIC_MASTER_VECTOR_OFFSET	(0x20)
# define PIC_SLAVE_VECTOR_OFFSET	(0xA0)
# define PIC_EOI					(0x20)

# define ICW1_ICW4					(0x01)	/* ICW4 will be present           */
# define ICW1_SINGLE				(0x02)	/* Single (cascade) mode          */
# define ICW1_INTERVAL4				(0x04)	/* Call address interval 4 (8)    */
# define ICW1_LEVEL					(0x08)	/* Level triggered (edge) mode    */
# define ICW1_INIT					(0x10)	/* Initialization - required!     */

# define ICW4_8086					(0x01)	/* 8086/88 (MCS-80/85) mode       */
# define ICW4_AUTO					(0x02)	/* Auto (normal) EOI              */
# define ICW4_BUF_SLAVE				(0x08)	/* Buffered mode/slave            */
# define ICW4_BUF_MASTER			(0x0C)	/* Buffered mode/master           */
# define ICW4_SFNM					(0x10)	/* Special fully nested (not)     */

# ifndef __ASSEMBLY__

/**
 * @brief remap the piq IRQs
 *
 * @param m_off master remap offset
 * @param s_off slave remap offset
 */
void	remap_pic(u8 m_off, u8 s_off);

# endif // ifndef __ASSEMBLY__

#endif
