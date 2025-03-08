/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 17:42:15 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/07 18:31:33 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ************************************************************************** //
// *                                                                        * //
// * Includes                                                               * //
// *                                                                        * //
// ************************************************************************** //


#include <kfs/string.h>

// ************************************************************************** //
// *                                                                        * //
// * Helper prototypes                                                      * //
// *                                                                        * //
// ************************************************************************** //

static
void	swap(
			unsigned char *a,
			unsigned char *b
			);

// ************************************************************************** //
// *                                                                        * //
// * Header function.                                                       * //
// *                                                                        * //
// ************************************************************************** //

void	*memchr(
			const void *s,
			int c,
			uaddr n
			)
{
	while (n--)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			return ((void *) s);
		++s;
	}
	return (NULL);
}

int		memcmp(
			const void *s1,
			const void *s2,
			uaddr n
			)
{
	while (n--)
	{
		if (*(unsigned char *)s1 != *(unsigned char *)s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		s1++;
		s2++;
	}
	return (0);
}

void	*memcpy(
			void *restrict dst,
			const void *restrict src,
			uaddr n
			)
{
	if (!dst && !src)
		return (NULL);
	while (n--)
		((char *)dst)[n] = ((char *)src)[n];
	return (dst);
}

void	*memmove(
			void *dst,
			const void *src,
			uaddr n
			)
{
	void	*dst_cpy;

	if (!dst && !src)
		return (NULL);
	dst_cpy = dst;
	if (dst > src && (uaddr)(dst - src) < n)
	{
		dst += n - 1;
		src += n - 1;
		while (n--)
			*(char *)dst-- = *(char *)src--;
	}
	else
		while (n--)
			*(char *)dst++ = *(char *)src++;
	return (dst_cpy);
}

void	*memset(
			void *ptr,
			int c,
			uaddr n
			)
{
	while (n)
		((char *)ptr)[--n] = c;
	return (ptr);
}

void	memswap(
			void *_a,
			void *_b,
			uaddr n
			)
{
	unsigned char	*a;
	unsigned char	*b;

	a = _a;
	b = _b;
	while (n--)
		swap(a++, b++);
}

char	*strchr(
			const char *s,
			int c
			)
{
	while (*s)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			return ((char *) s);
		++s;
	}
	if (*(unsigned char *)s == (unsigned char)c)
		return ((char *) s);
	return (NULL);
}

int		strcmp(
			const char *s1,
			const char *s2
			)
{
	while (*s1)
	{
		if (*(unsigned char *)s1 != *(unsigned char *)s2)
			break ;
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

char	*strcpy(
			char *dst,
			const char *src
			)
{
	char	*dst_cpy;

	dst_cpy = dst;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return (dst_cpy);
}

void	striteri(
			char *str,
			void (*f)(unsigned int, char*)
			)
{
	uaddr	i;

	i = 0;
	while (*str)
		f(i++, str++);
}

uaddr	strlcat(
			char *dest,
			const char *src,
			uaddr size
			)
{
	char		*dst_cpy;
	const char	*src_cpy;
	uaddr		remaining;
	uaddr		dest_len;

	dst_cpy = dest;
	src_cpy = src;
	remaining = size;
	while (remaining-- && *dst_cpy)
		dst_cpy++;
	dest_len = dst_cpy - dest;
	remaining = size - dest_len;
	if (!remaining)
		return (dest_len + strlen(src_cpy));
	while (*src_cpy)
	{
		if (remaining != 1)
		{
			*dst_cpy++ = *src_cpy;
			--remaining;
		}
		src_cpy++;
	}
	*dst_cpy = '\0';
	return (dest_len + (src_cpy - src));
}

uaddr	strlcpy(
			char *dst,
			const char *src,
			uaddr size
			)
{
	uaddr	i;

	i = 0;
	if (size > 0)
	{
		while (src[i] && i < (size - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = 0;
	}
	while (src[i])
		i++;
	return (i);
}

uaddr	strlen(
			const char *str
			)
{
	const char	*p;

	p = str;
	while (*p)
		++p;
	return (p - str);
}

int		strncmp(
			const char *s1,
			const char *s2,
			uaddr n
			)
{
	while (n--)
	{
		if (*s1 == '\0' || *(unsigned char *)s1 != *(unsigned char *)s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		s1++;
		s2++;
	}
	return (0);
}

usize	strnlen(
			const char * s,
			usize n
			)
{
	usize	i;

	i = 0;
	while (s[i] && i < n)
		++i;
	return (i);
}

char	*strnstr(
			const char *big,
			const char *little,
			uaddr len
			)
{
	uaddr	i;
	int		j;

	i = 0;
	if (little[i] == '\0')
		return ((char *)big);
	while (big[i] != '\0' && i < len)
	{
		if (big[i] == *little)
		{
			j = 0;
			while (big[i + j] == little[j] && i + j < len)
			{
				if (little[j + 1] == '\0')
					return ((char *)big + i);
				j++;
			}
		}
		i++;
	}
	return (NULL);
}

char	*strrchr(
			const char *s,
			int c
			)
{
	const char	*r;

	r = NULL;
	while (*s)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			r = s;
		++s;
	}
	if (*(unsigned char *)s == (unsigned char)c)
		r = s;
	return ((char *) r);
}

// ************************************************************************** //
// *                                                                        * //
// * Helper definitions                                                     * //
// *                                                                        * //
// ************************************************************************** //

static
void	swap(
			unsigned char *a,
			unsigned char *b
			)
{
	char	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
