/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vsprintf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:06:25 by amassias          #+#    #+#             */
/*   Updated: 2025/03/08 12:08:36 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ************************************************************************** //
// *                                                                        * //
// * Includes                                                               * //
// *                                                                        * //
// ************************************************************************** //

#include <kfs/ctype.h>
#include <kfs/string.h>
#include <stdarg.h>
#include <limits.h>

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SMALL	32		/* use lowercase in hex (must be 32 == 0x20) */
#define SPECIAL	64		/* prefix hex with "0x", octal with "0" */

#define TOLOWER(x) ((x) | 0x20)

enum format_type
{
	FORMAT_TYPE_NONE, /* Just a string part */
	FORMAT_TYPE_WIDTH,
	FORMAT_TYPE_PRECISION,
	FORMAT_TYPE_CHAR,
	FORMAT_TYPE_STR,
	FORMAT_TYPE_PTR,
	FORMAT_TYPE_PERCENT_CHAR,
	FORMAT_TYPE_INVALID,
	FORMAT_TYPE_LONG_LONG,
	FORMAT_TYPE_ULONG,
	FORMAT_TYPE_LONG,
	FORMAT_TYPE_UBYTE,
	FORMAT_TYPE_BYTE,
	FORMAT_TYPE_USHORT,
	FORMAT_TYPE_SHORT,
	FORMAT_TYPE_UINT,
	FORMAT_TYPE_INT,
	FORMAT_TYPE_NRCHARS,
	FORMAT_TYPE_SIZE_T,
	FORMAT_TYPE_PTRDIFF
};

struct printf_spec
{
	u8	type;			/* format_type enum */
	u8	flags;			/* flags to number() */
	u8	base;			/* number base, 8, 10 or 16 only */
	u8	qualifier;		/* number qualifier, one of 'hHlLtzZ' */
	i16	field_width;	/* width of output field */
	i16	precision;		/* # of digits/chars */
};

static
int		skip_atoi(
			const char **str_ptr
			)
{
	int	n;

	n = 0;
	while (isdigit(**str_ptr))
		n = 10 * n + *((*str_ptr)++) - '0';
	return (n);
}

static
int		read_format(
			const char *fmt,
			struct printf_spec *spec
			)
{
	const char	*start;
	int			found;

	start = fmt;

	if (spec->type == FORMAT_TYPE_WIDTH)
	{
		if (spec->field_width < 0)
		{
			spec->field_width = -spec->field_width;
			spec->flags |= LEFT;
		}
		spec->type = FORMAT_TYPE_NONE;
		goto precision;
	}

	if (spec->type == FORMAT_TYPE_PRECISION)
	{
		if (spec->precision < 0)
			spec->precision = 0;
		spec->type = FORMAT_TYPE_NONE;
		goto qualifier;
	}

	spec->type = FORMAT_TYPE_NONE;
	for (; *fmt && *fmt != '%'; ++fmt)
		;
	if (fmt != start || !*fmt)
		return (fmt - start);

	spec->flags = 0;

	while (1)
	{
		found = 1;
		++fmt;
		switch (*fmt)
		{
			case '-': spec->flags |= LEFT; break ;
			case '+': spec->flags |= PLUS; break ;
			case ' ': spec->flags |= SPACE; break ;
			case '#': spec->flags |= SPECIAL; break ;
			case '0': spec->flags |= ZEROPAD; break ;
			default: found = 0;
		}
		if (!found)
			break ;
	}

	spec->field_width = -1;
	if (isdigit(*fmt))
		spec->field_width = skip_atoi(&fmt);
	else if (*fmt == '*')
	{
		spec->type = FORMAT_TYPE_WIDTH;
		return (++fmt - start);
	}

precision:
	spec->precision = -1;
	if (*fmt == '.')
	{
		++fmt;
		if (isdigit(*fmt))
		{
			spec->precision = skip_atoi(&fmt);
			if (spec->precision < 0)
				spec->precision = 0;
		}
		else if (*fmt == '*')
		{
			spec->type = FORMAT_TYPE_PRECISION;
			return (++fmt - start);
		}
	}

qualifier:
	spec->qualifier = -1;
	if (*fmt == 'h' || TOLOWER(*fmt) == 'l' || TOLOWER(*fmt) == 'z' || *fmt == 't')
	{
		spec->qualifier = *fmt++;
		if (spec->qualifier == *fmt)
		{
			if (spec->qualifier == 'l')
			{
				spec->qualifier = 'L';
				++fmt;
			}
			else if (spec->qualifier == 'h')
			{
				spec->qualifier = 'H';
				++fmt;
			}
		}
	}

	spec->base = 10;
	switch (*fmt)
	{
		case 'c': spec->type = FORMAT_TYPE_CHAR; return (++fmt - start);
		case 's': spec->type = FORMAT_TYPE_STR; return (++fmt - start);
		case 'p': spec->type = FORMAT_TYPE_PTR; return (++fmt - start);
		case 'n': spec->type = FORMAT_TYPE_NRCHARS; return (++fmt - start);
		case '%': spec->type = FORMAT_TYPE_PERCENT_CHAR; return (++fmt - start);

		case 'o': spec->base = 8; break ;
		case 'x': spec->flags |= SMALL;
		case 'X': spec->base = 16; break ;
		case 'd':
		case 'i': spec->flags |= SIGN;
		case 'u': break ;

		default: spec->type = FORMAT_TYPE_INVALID; return (++fmt - start);
	}

	if (spec->qualifier == 'L')
		spec->type = FORMAT_TYPE_LONG_LONG;
	else if (spec->qualifier == 'l')
		spec->type = (spec->flags & SIGN)
			? FORMAT_TYPE_LONG
			: FORMAT_TYPE_ULONG;
	else if (TOLOWER(spec->qualifier) == 'z')
		spec->type = FORMAT_TYPE_SIZE_T;
	else if (spec->qualifier == 't')
		spec->type = FORMAT_TYPE_PTRDIFF;
	else if (spec->qualifier == 'H')
		spec->type = (spec->flags & SIGN)
			? FORMAT_TYPE_BYTE
			: FORMAT_TYPE_UBYTE;
	else if (spec->qualifier == 'h')
		spec->type = (spec->flags & SIGN)
			? FORMAT_TYPE_SHORT
			: FORMAT_TYPE_USHORT;
	else
		spec->type = (spec->flags & SIGN)
			? FORMAT_TYPE_INT
			: FORMAT_TYPE_UINT;

	return (++fmt - start);
}

static
char	*string(
			char *buf,
			char *end,
			const char *s,
			struct printf_spec spec
			)
{
	int	len;
	int	i;

	if (s == NULL)
		s = "(null)";
	len = strnlen(s, spec.precision);
	if (!(spec.flags & LEFT))
		while (len < spec.field_width--)
			if (buf++ < end)
				buf[-1] = ' ';
	for (i = 0; i < len; ++i, ++buf, ++s)
		if (buf < end)
				*buf = *s;
	while (len < spec.field_width--)
		if (buf++ < end)
				buf[-1] = ' ';
	return (buf);
}

static
char	*put_dec_trunc(
			char *buf,
			unsigned q
			)
{
	unsigned d3;
	unsigned d2;
	unsigned d1;
	unsigned d0;

	d1 = (q>>4) & 0xf;
	d2 = (q>>8) & 0xf;
	d3 = (q>>12);

	d0 = 6*(d3 + d2 + d1) + (q & 0xf);
	q = (d0 * 0xcd) >> 11;
	d0 = d0 - 10*q;
	*buf++ = d0 + '0'; /* least significant digit */
	d1 = q + 9*d3 + 5*d2 + d1;
	if (d1 != 0)
	{
		q = (d1 * 0xcd) >> 11;
		d1 = d1 - 10*q;
		*buf++ = d1 + '0'; /* next digit */

		d2 = q + 2*d2;
		if ((d2 != 0) || (d3 != 0))
		{
			q = (d2 * 0xd) >> 7;
			d2 = d2 - 10*q;
			*buf++ = d2 + '0'; /* next digit */

			d3 = q + 4*d3;
			if (d3 != 0)
			{
				q = (d3 * 0xcd) >> 11;
				d3 = d3 - 10*q;
				*buf++ = d3 + '0';  /* next digit */
				if (q != 0)
					*buf++ = q + '0'; /* most sign. digit */
			}
		}
	}

	return (buf);
}

static
char	*put_dec_full(
			char *buf,
			unsigned q
			)
{
	unsigned d3;
	unsigned d2;
	unsigned d1;
	unsigned d0;

	d1 = (q>>4) & 0xf;
	d2 = (q>>8) & 0xf;
	d3 = (q>>12);

	/*
	 * Possible ways to approx. divide by 10
	 * gcc -O2 replaces multiply with shifts and adds
	 * (x * 0xcd) >> 11: 11001101 - shorter code than * 0x67 (on i386)
	 * (x * 0x67) >> 10:  1100111
	 * (x * 0x34) >> 9:    110100 - same
	 * (x * 0x1a) >> 8:     11010 - same
	 * (x * 0x0d) >> 7:      1101 - same, shortest code (on i386)
	 */
	d0 = 6*(d3 + d2 + d1) + (q & 0xf);
	q = (d0 * 0xcd) >> 11;
	d0 = d0 - 10*q;
	*buf++ = d0 + '0';
	d1 = q + 9*d3 + 5*d2 + d1;
		q = (d1 * 0xcd) >> 11;
		d1 = d1 - 10*q;
		*buf++ = d1 + '0';

		d2 = q + 2*d2;
			q = (d2 * 0xd) >> 7;
			d2 = d2 - 10*q;
			*buf++ = d2 + '0';

			d3 = q + 4*d3;
				q = (d3 * 0xcd) >> 11; /* - shorter code */
				/* q = (d3 * 0x67) >> 10; - would also work */
				d3 = d3 - 10*q;
				*buf++ = d3 + '0';
					*buf++ = q + '0';

	return (buf);
}

static
char	*put_dec(
			char *buf,
			unsigned long long n
			)
{
	unsigned rem;

	while (1)
	{
		if (n < 100000)
			return put_dec_trunc(buf, n);
		rem = n % 100000;
		n /= 100000;
		buf = put_dec_full(buf, rem);
	}
}

static
char	*number(
			char *buf,
			char *end,
			unsigned long long n,
			struct printf_spec spec
			)
{
	static const char digits[] = "0123456789ABCDEF";

	char	tmp[66];
	char	sign;
	char	locase;
	char	c;
	int		need_prefix = ((spec.flags & SPECIAL) && spec.base != 10);
	int		i;
	int		mask;
	int		shift;

	locase = spec.flags & SMALL;
	if (spec.flags & LEFT)
		spec.flags &= ~ZEROPAD;
	sign = 0;
	if (spec.flags & SIGN)
	{
		if ((signed long long)n < 0)
		{
			sign = '-';
			n = -(signed long long)n;
			--spec.field_width;
		} else if (spec.flags & PLUS)
		{
			sign = '+';
			--spec.field_width;
		} else if (spec.flags & SPACE)
		{
			sign = ' ';
			--spec.field_width;
		}
	}

	if (need_prefix)
	{
		--spec.field_width;
		if (spec.base == 16)
			--spec.field_width;
	}
	i = 0;
	if (n == 0)
		tmp[i++] = '0';
	else if (spec.base != 10)
	{
		mask = spec.base - 1;
		shift = 3;
		if (spec.base == 16)
			shift = 4;
		do
		{
			tmp[i++] = digits[((unsigned char)n) & mask] | locase;
			n >>= shift;
		} while (n);
	}
	else
		i = put_dec(tmp, n) - tmp;

	if (i >= spec.precision)
		spec.precision = i;
	spec.field_width -= spec.precision;

	if (!(spec.flags & (ZEROPAD | LEFT)))
		while (--spec.field_width >= 0)
			if (buf++ < end)
				buf[-1] = ' ';

	if (sign)
		if (buf++ < end)
			buf[-1] = sign;

	if (need_prefix)
	{
		if (buf++ < end)
			buf[-1] = '0';
		if (spec.base == 16)
			if (buf++ < end)
				buf[-1] = ('x' | locase);
	}

	if (!(spec.flags & LEFT))
	{
		c = (spec.flags & ZEROPAD) ? '0' : ' ';
		while (--spec.field_width >= 0)
			if (buf++ < end)
				buf[-1] = c;
	}

	while (i < --spec.precision)
		if (buf++ < end)
			buf[-1] = '0';

	while (--i >= 0)
		if (buf++ < end)
			buf[-1] = tmp[i];

	while (--spec.field_width >= 0)
		if (buf++ < end)
			buf[-1] = ' ';

	return (buf);
}

static
char	*pointer(
			const char *fmt,
			char *buf,
			char *end,
			void *ptr,
			struct printf_spec spec
			)
{
	if (!ptr && *fmt != 'K')
	{
		if (spec.field_width == -1)
			spec.field_width = 2 * sizeof(void *);
		return (string(buf, end, "(null)", spec));
	}

	spec.flags |= SMALL;
	if (spec.field_width == -1)
	{
		spec.field_width = 2 * sizeof(void *);
		spec.flags |= ZEROPAD;
	}
	spec.base = 16;
	return (number(buf, end, (unsigned long)ptr, spec));
}

int		vsnprintf(
			char *buf,
			usize size,
			const char *fmt,
			va_list args
			)
{
	struct printf_spec	spec = {0};
	unsigned long long	n;
	const char			*old_fmt;
	char				*str;
	char				*end;
	char				c;
	int					read;
	int					copy;
	u8					qualifier;

	if ((int)size < 0)
		return (0);
	str = buf;
	end = buf + size;
	if (end < buf)
	{
		end = ((void *)-1);
		size = end - buf;
	}
	while (*fmt)
	{
		old_fmt = fmt;
		read = read_format(fmt, &spec);
		fmt += read;
		switch (spec.type)
		{
			case FORMAT_TYPE_NONE:
				copy = read;
				if (str < end)
				{
					if (copy > end - str)
						copy = end - str;
					memcpy(str, old_fmt, copy);
				}
				str += read;
				break ;

			case FORMAT_TYPE_WIDTH:
				spec.field_width = va_arg(args, int);
				break ;
			case FORMAT_TYPE_PRECISION:
				spec.precision = va_arg(args, int);
				break ;

			case FORMAT_TYPE_CHAR:
				if (!(spec.flags & LEFT))
				{
					while (--spec.field_width > 0)
						if (str++ < end)
							str[-1] = ' ';
				}
				c = (unsigned char)va_arg(args, int);
				if (str < end)
					*str = c;
				++str;
				while (--spec.field_width > 0)
						if (str++ < end)
							str[-1] = ' ';
				break ;

			case FORMAT_TYPE_STR:
				str = string(str, end, va_arg(args, char *), spec);
				break ;

			case FORMAT_TYPE_PTR:
				str = pointer(fmt + 1, str, end, va_arg(args, void *), spec);
				for (; isalnum(*fmt); ++fmt)
					;
				break ;

			case FORMAT_TYPE_INVALID:
			case FORMAT_TYPE_PERCENT_CHAR:
				if (str++ < end)
					str[-1] = '%';
				break ;

			case FORMAT_TYPE_NRCHARS:
				qualifier = spec.qualifier;
				if (qualifier == 'l')
					*(long *)va_arg(args, long *) = (str - buf);
				else if (TOLOWER(qualifier) == 'z')
					*(usize *)va_arg(args, usize *) = (str - buf);
				else
					*(int *)va_arg(args, int *) = (str - buf);
				break ;

			default:
				switch (spec.type)
				{
					case FORMAT_TYPE_LONG_LONG:
						n = va_arg(args, long long);
						break ;
					case FORMAT_TYPE_ULONG:
						n = va_arg(args, unsigned long);
						break ;
					case FORMAT_TYPE_LONG:
						n = va_arg(args, signed long);
						break ;
					case FORMAT_TYPE_SIZE_T:
						n = va_arg(args, usize);
						break ;
					case FORMAT_TYPE_PTRDIFF:
						n = va_arg(args, uaddr);
						break ;
					case FORMAT_TYPE_UBYTE:
						n = (unsigned char)va_arg(args, int);
						break ;
					case FORMAT_TYPE_BYTE:
						n = (signed char)va_arg(args, int);
						break ;
					case FORMAT_TYPE_USHORT:
						n = (unsigned short)va_arg(args, int);
						break ;
					case FORMAT_TYPE_SHORT:
						n = (signed short)va_arg(args, int);
						break ;
					case FORMAT_TYPE_INT:
						n = (int)va_arg(args, int);
						break ;
					default:
						n = va_arg(args, unsigned int);
						break ;
				}
				str = number(str, end, n, spec);
				break ;
		}
	}

	if (size > 0)
	{
		if (str < end)
			*str = '\0';
		else
			end[-1] = '\0';
	}
	return (str - buf);
}

int		vscnprintf(
			char *buf,
			u32 size,
			const char *fmt,
			va_list args
			)
{
	int	i;

	i = vsnprintf(buf, size, fmt, args);
	if ((usize)i < size)
		return (i);
	if (size != 0)
		return (size - 1);
	return (0);
}

int		snprintf(
			char *buf,
			usize size,
			const char *fmt,
			...
			)
{
	va_list	args;
	int		i;

	va_start(args, fmt);
	i = vsnprintf(buf, size, fmt, args);
	va_end(args);
	return (i);
}

int		vsprintf(
			char *buf,
			const char *fmt,
			va_list args
			)
{
	return (vsnprintf(buf, INT_MAX, fmt, args));
}

int		sprintf(
			char *buf,
			const char *fmt,
			...
			)
{
	va_list	args;
	int		i;

	va_start(args, fmt);
	i = vsnprintf(buf, INT_MAX, fmt, args);
	va_end(args);
	return (i);
}
