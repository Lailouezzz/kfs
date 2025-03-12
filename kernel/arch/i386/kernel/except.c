/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   except.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:12:28 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/12 16:59:20 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kfs/kernel.h>
#include <kfs/compiler.h>
#include <asm/ptrace.h>

void	default_except_handler(int_regs_s regs)
{
	printk("Default exception handler called. (%p)\n", regs.eip);
	UNUSED(regs);
}
