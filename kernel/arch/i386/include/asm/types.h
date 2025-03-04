/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:28:11 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/04 19:30:40 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ASM_TYPES_H
# define ASM_TYPES_H

#ifndef __ASSEMBLY__

typedef char				i8;
typedef short				i16;
typedef int					i32;
typedef long int			i64;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long int	u64;

typedef u32					uaddr;

#endif // ifndef __ASSEMBLY__

#endif
