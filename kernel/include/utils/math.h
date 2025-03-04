/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:37:04 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/04 17:38:46 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  H_UTILS_MATH_H
# define H_UTILS_MATH_H

#define __align_mask(_value, _power) ((__typeof__(_value))((_power)-1))

/** @brief Align @c _value to the next multiple of @c _power
 *  @warning This macro assumes _power is a power of 2. When using an arbitrary
 *           value, you must use @ref round_up instead.
 */
#define align_up(_value, _power) \
	((((_value)-1) | __align_mask(_value, _power)) + 1)

#endif
