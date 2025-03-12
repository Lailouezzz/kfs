/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:34:17 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/12 14:35:29 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Architecture specific instrucitons
 *
 */

#ifndef  ASM_GENERIC_H
# define ASM_GENERIC_H

# ifndef __ASSEMBLY__

#  define HALT() __asm__ volatile ("hlt")

# endif // ifndef __ASSEMBLY__

#endif
