/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:35:33 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/07 20:19:18 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kfs/types.h>
#include <stdarg.h>

void	*memchr(
			const void *s,
			int c,
			uaddr n
			);

int		memcmp(
			const void *s1,
			const void *s2,
			uaddr n
			);

void	*memcpy(
			void *restrict dst,
			const void *restrict src,
			uaddr n
			);

void	*memmove(
			void *dst,
			const void *src,
			uaddr n
			);

void	*memset(
			void *ptr,
			int c,
			uaddr n
			);

void	memswap(
			void *_a,
			void *_b,
			uaddr n
			);

char	*strchr(
			const char *s,
			int c
			);

int		strcmp(
			const char *s1,
			const char *s2
			);

char	*strcpy(
			char *dst,
			const char *src
			);

void	striteri(
			char *str,
			void (*f)(unsigned int, char*)
			);

uaddr	strlcat(
			char *dest,
			const char *src,
			uaddr size
			);

uaddr	strlcpy(
			char *dst,
			const char *src,
			uaddr size
			);

uaddr	strlen(
			const char *str
			);

int		strncmp(
			const char *s1,
			const char *s2,
			uaddr n
			);

usize	strnlen(
			const char * s,
			usize n
			);

char	*strnstr(
			const char *big,
			const char *little,
			uaddr len
			);

char	*strrchr(
			const char *s,
			int c
			);

int		vsnprintf(
			char *buf,
			usize size,
			const char *fmt,
			va_list args
			);

int		vscnprintf(
			char *buf,
			u32 size,
			const char *fmt,
			va_list args
			);

int		snprintf(
			char *buf,
			usize size,
			const char *fmt,
			...
			);

int		vsprintf(
			char *buf,
			const char *fmt,
			va_list args
			);

int		vsprintf(
			char *buf,
			const char *fmt,
			va_list args
			);

int		sprintf(
			char *buf,
			const char *fmt,
			...
			);
