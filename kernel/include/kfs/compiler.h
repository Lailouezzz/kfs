/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compiler.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:06:52 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/12 15:16:43 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  KFS_COMPILER_H
# define KFS_COMPILER_H

# ifndef __ASSEMBLY__

#  define PACKED __attribute__((packed))
#  define USED __attribute__((used))
#  define UNUSED(__x) (void)(__x)
#  define SECTION(__s) __attribute__((section(__s)))
#  define ALWAYS_INLINE __attribute__((always_inline))

# endif // ifndef __ASSEMBLY__

#endif
