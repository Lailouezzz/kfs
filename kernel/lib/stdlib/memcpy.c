/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memcpy.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:55:22 by amassias          #+#    #+#             */
/*   Updated: 2025/03/04 18:55:38 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	memcpy(void *dst, void *src, unsigned long size)
{
	while (size--)
		*(unsigned char *)dst++ = *(unsigned char *)src++;
}
