/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ptrace.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:02:24 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/12 15:11:44 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ASM_PTRACE_H
# define ASM_PTRACE_H

# include <kfs/types.h>

# ifndef __ASSEMBLY__

/**
 * @brief This is how interupts handler push registers to the stack
 *
 */
typedef struct {
	u32	edx;
	u32	ecx;
	u32	ebx;
	u32	eax;
	u32	edi;
	u32	esi;
	u32	ebp;
	u32 _res1;
	u32 orig_eax;
	u32 eip;
}	int_regs_s;

# endif // ifndef __ASSEMBLY__

#endif
