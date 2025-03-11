/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Antoine Massias <massias.antoine.pro@gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:18:44 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/10 18:23:09 by Antoine Mas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ASM_MEMORY_H
# define ASM_MEMORY_H

# define KERNEL_HIGHER_HALF_OFFSET 0xC0000000
# define KERNEL_PHYS_START 0x00100000
# define KERNEL_VIRT_START (KERNEL_HIGHER_HALF_OFFSET + KERNEL_PHYS_START)
# define KERNEL_PHYS(__virt) (__virt - KERNEL_HIGHER_HALF_OFFSET)
# define KERNEL_VIRT(__phys) (__phys + KERNEL_HIGHER_HALF_OFFSET)

# define KERNEL_STACK_SIZE 0x4000U

# define PAGE_SIZE 0x1000

# define __PACKED__ __attribute__((packed))

# ifndef __ASSEMBLY__



# endif // ifndef __ASSEMBLY__

#endif
