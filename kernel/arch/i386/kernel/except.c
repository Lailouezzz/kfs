/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   except.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:12:28 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/11 16:50:55 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm/except.h>
#include <kfs/kernel.h>
#include <kfs/vga.h>

void	except_handler(interrupt_stack_frame_t stackframe)
{
	panic(stackframe, "%s :(", "Aled");
}
