/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctype.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:09:17 by amassias          #+#    #+#             */
/*   Updated: 2025/03/07 18:47:29 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	isdigit(
		int ch
		)
{
	return ((ch >= '0') && (ch <= '9'));
}

int	isxdigit(
		int ch
		)
{
	if (isdigit(ch))
		return (1);
	ch |= 0x20;
	return ((ch >= 'a') && (ch <= 'f'));
}

int	isalpha(
		char c
		)
{
	c |= 0x20;
	return ('a' <= c && c <= 'z');
}

int	isalnum(
		char c
		)
{
	return (isalpha(c) || isdigit(c));
}
