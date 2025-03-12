/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   except.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:12:28 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/12 19:35:53 by amassias         ###   ########.fr       */
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
