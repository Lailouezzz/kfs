/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-boud <ale-boud@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:18:09 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/08 11:49:45 by ale-boud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  KFS_KERNEL_H
# define KFS_KERNEL_H

# include <stdarg.h>

int	vprintk(const char *fmt, va_list list);
int	printk(const char *fmt, ...);

#endif
