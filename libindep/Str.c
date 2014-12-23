/* $Id: Str.c,v 1.8 2002/12/24 17:20:46 ukai Exp $ */
/*
 * String manipulation library for Boehm GC
 * 
 * (C) Copyright 1998-1999 by Akinori Ito
 * 
 * This software may be redistributed freely for this purpose, in full or in
 * part, provided that this entire copyright notice is included on any copies
 * of this software and applications and derivations thereof.
 * 
 * This software is provided on an "as is" basis, without warranty of any kind,
 * either expressed or implied, as to any matter including, but not limited
 * to warranty of fitness of purpose, or merchantability, or results obtained
 * from use of this software.
 */
#include <stdio.h>
#include <stdlib.h>
#include <gc.h>
#include <stdarg.h>
#include <string.h>
#include "Str.h"
#include "myctype.h"

#define INITIAL_STR_SIZE 32

#ifdef STR_DEBUG
/* This is obsolete, because "Str" can handle a '\0' character now. */
#define STR_LENGTH_CHECK(x) if (((x)->ptr==0&&(x)->length!=0)||(strlen((x)->ptr)!=(x)->length))abort();
#else				/* not STR_DEBUG */
#define STR_LENGTH_CHECK(x)
#endif				/* not STR_DEBUG */

Str
Strnew()
{
	Str x = GC_MALLOC(sizeof(struct _Str));
	x->ptr = GC_MALLOC_ATOMIC(INITIAL_STR_SIZE);
	x->ptr[0] = '\0';
	x->area_size = INITIAL_STR_SIZE;
	x->length = 0;
	return x;
}

Str
Strnew_size(int n)
{
	Str x = GC_MALLOC(sizeof(struct _Str));
	x->ptr = GC_MALLOC_ATOMIC(n + 1);
	x->ptr[0] = '\0';
	x->area_size = n + 1;
	x->length = 0;
	return x;
}

Str
Strnew_charp(const char *p)
{
	Str x;
	int n;

	if (p == NULL)
		return Strnew();
	x = GC_MALLOC(sizeof(struct _Str));
	n = strlen(p) + 1;
	x->ptr = GC_MALLOC_ATOMIC(n);
	x->area_size = n;
	x->length = n - 1;
	memcpy(x->ptr, p, n);
	return x;
}

Str
Strnew_m_charp(const char *p, ...)
{
	va_list ap;
	Str r = Strnew();

	va_start(ap, p);
	while (p != NULL) {
		Strcat_charp(r, p);
		p = va_arg(ap, char *);
	}
	va_end(ap);
	return r;
}

Str
Strnew_charp_n(const char *p, int n)
{
	Str x;

	if (p == NULL)
		return Strnew_size(n);
	x = GC_MALLOC(sizeof(struct _Str));
	x->ptr = GC_MALLOC_ATOMIC(n + 1);
	x->area_size = n + 1;
	x->length = n;
	memcpy(x->ptr, p, n);
	x->ptr[n] = '\0';
	return x;
}

Str
Strdup(Str s)
{
	Str n = Strnew_size(s->length);
	STR_LENGTH_CHECK(s);
	Strcopy(n, s);
	return n;
}

void
Strclear(Str s)
{
	s->length = 0;
	s->ptr[0] = '\0';
}

void
Strfree(Str x)
{
	GC_free(x->ptr);
	GC_free(x);
}

void
Strcopy(Str x, Str y)
{
	STR_LENGTH_CHECK(x);
	STR_LENGTH_CHECK(y);
	if (x->area_size < y->length + 1) {
		GC_free(x->ptr);
		x->ptr = GC_MALLOC_ATOMIC(y->length + 1);
		x->area_size = y->length + 1;
	}
	memcpy(x->ptr, y->ptr, y->length + 1);
	x->length = y->length;
}

void
Strcopy_charp(Str x, const char *y)
{
	int len;

	STR_LENGTH_CHECK(x);
	if (y == NULL) {
		x->length = 0;
		return;
	}
	len = strlen(y);
	if (x->area_size < len + 1) {
		GC_free(x->ptr);
		x->ptr = GC_MALLOC_ATOMIC(len + 1);
		x->area_size = len + 1;
	}
	memcpy(x->ptr, y, len + 1);
	x->length = len;
}

void
Strcopy_charp_n(Str x, const char *y, int n)
{
	int len = n;

	STR_LENGTH_CHECK(x);
	if (y == NULL) {
		x->length = 0;
		return;
	}
	if (x->area_size < len + 1) {
		GC_free(x->ptr);
		x->ptr = GC_MALLOC_ATOMIC(len + 1);
		x->area_size = len + 1;
	}
	memmove(x->ptr, y, n);
	x->ptr[n] = '\0';
	x->length = n;
}

void
Strcat_charp_n(Str x, const char *y, int n)
{
	int newlen;

	STR_LENGTH_CHECK(x);
	if (y == NULL)
		return;
	newlen = x->length + n + 1;
	if (x->area_size < newlen) {
		char *old = x->ptr;
		newlen = newlen * 3 / 2;
		x->ptr = GC_MALLOC_ATOMIC(newlen);
		x->area_size = newlen;
		memcpy(x->ptr, old, x->length);
		GC_free(old);
	}
	memmove(&x->ptr[x->length], y, n);
	x->length += n;
	x->ptr[x->length] = '\0';
}

void
Strcat(Str x, Str y)
{
	STR_LENGTH_CHECK(y);
	Strcat_charp_n(x, y->ptr, y->length);
}

void
Strcat_charp(Str x, const char *y)
{
	if (y == NULL)
		return;
	Strcat_charp_n(x, y, strlen(y));
}

void
Strcat_m_charp(Str x,...)
{
	va_list ap;
	char *p;

	va_start(ap, x);
	while ((p = va_arg(ap, char *)) != NULL)
		Strcat_charp_n(x, p, strlen(p));
	va_end(ap);
}

void
Strgrow(Str x)
{
	char *old = x->ptr;
	int newlen;
	newlen = x->length * 6 / 5;
	if (newlen == x->length)
		newlen += 2;
	x->ptr = GC_MALLOC_ATOMIC(newlen);
	x->area_size = newlen;
	memcpy(x->ptr, old, x->length);
	GC_free(old);
}

Str
Strsubstr(Str s, int beg, int len)
{
	Str new_s;
	int i;

	STR_LENGTH_CHECK(s);
	new_s = Strnew();
	if (beg >= s->length)
		return new_s;
	for (i = 0; i < len && beg + i < s->length; i++)
		Strcat_char(new_s, s->ptr[beg + i]);
	return new_s;
}

void
Strlower(Str s)
{
	int i;
	STR_LENGTH_CHECK(s);
	for (i = 0; i < s->length; i++)
		s->ptr[i] = TOLOWER(s->ptr[i]);
}

void
Strupper(Str s)
{
	int i;
	STR_LENGTH_CHECK(s);
	for (i = 0; i < s->length; i++)
		s->ptr[i] = TOUPPER(s->ptr[i]);
}

void
Strchop(Str s)
{
	STR_LENGTH_CHECK(s);
	while ((s->ptr[s->length - 1] == '\n' || s->ptr[s->length - 1] == '\r') &&
	       s->length > 0) {
		s->length--;
	}
	s->ptr[s->length] = '\0';
}

void
Strinsert_char(Str s, int pos, char c)
{
	int i;
	STR_LENGTH_CHECK(s);
	if (pos < 0 || s->length < pos)
		return;
	if (s->length + 2 > s->area_size)
		Strgrow(s);
	for (i = s->length; i > pos; i--)
		s->ptr[i] = s->ptr[i - 1];
	s->ptr[++s->length] = '\0';
	s->ptr[pos] = c;
}

void
Strinsert_charp(Str s, int pos, const char *p)
{
	STR_LENGTH_CHECK(s);
	while (*p)
		Strinsert_char(s, pos++, *(p++));
}

void
Strdelete(Str s, int pos, int n)
{
	int i;
	STR_LENGTH_CHECK(s);
	if (s->length <= pos + n) {
		s->ptr[pos] = '\0';
		s->length = pos;
		return;
	}
	for (i = pos; i < s->length - n; i++)
		s->ptr[i] = s->ptr[i + n];
	s->ptr[i] = '\0';
	s->length = i;
}

void
Strtruncate(Str s, int pos)
{
	STR_LENGTH_CHECK(s);
	s->ptr[pos] = '\0';
	s->length = pos;
}

void
Strshrink(Str s, int n)
{
	STR_LENGTH_CHECK(s);
	if (n >= s->length) {
		s->length = 0;
		s->ptr[0] = '\0';
	} else {
		s->length -= n;
		s->ptr[s->length] = '\0';
	}
}

void
Strremovefirstspaces(Str s)
{
	int i;

	STR_LENGTH_CHECK(s);
	for (i = 0; i < s->length && IS_SPACE(s->ptr[i]); i++);
	if (i == 0)
		return;
	Strdelete(s, 0, i);
}

void
Strremovetrailingspaces(Str s)
{
	int i;

	STR_LENGTH_CHECK(s);
	for (i = s->length - 1; i >= 0 && IS_SPACE(s->ptr[i]); i--);
	s->length = i + 1;
	s->ptr[i + 1] = '\0';
}

Str
Stralign_left(Str s, int width)
{
	Str n;
	int i;

	STR_LENGTH_CHECK(s);
	if (s->length >= width)
		return Strdup(s);
	n = Strnew_size(width);
	Strcopy(n, s);
	for (i = s->length; i < width; i++)
		Strcat_char(n, ' ');
	return n;
}

Str
Stralign_right(Str s, int width)
{
	Str n;
	int i;

	STR_LENGTH_CHECK(s);
	if (s->length >= width)
		return Strdup(s);
	n = Strnew_size(width);
	for (i = s->length; i < width; i++)
		Strcat_char(n, ' ');
	Strcat(n, s);
	return n;
}

Str
Stralign_center(Str s, int width)
{
	Str n;
	int i, w;

	STR_LENGTH_CHECK(s);
	if (s->length >= width)
		return Strdup(s);
	n = Strnew_size(width);
	w = (width - s->length) / 2;
	for (i = 0; i < w; i++)
		Strcat_char(n, ' ');
	Strcat(n, s);
	for (i = w + s->length; i < width; i++)
		Strcat_char(n, ' ');
	return n;
}

Str
Sprintf(const char *fmt, ...)
{
	Str s;
	char *cb;
	int ret;
	size_t n;
	va_list ap;

	va_start(ap, fmt);
	ret = vasprintf(&cb, fmt, ap);
	if (ret == -1) {
		fprintf(stderr,
		    "Sprintf: vasprintf returned indicating fail\n");
		exit(1);
	}
	va_end(ap);

	n = (size_t) ret + 1;
	s = Strnew_size(n);
	s->length = ret;
	memcpy(s->ptr, cb, n);
	free(cb);
	return s;
}

Str
Strfgets(FILE * f)
{
	Str s = Strnew();
	char c;
	while (1) {
		c = fgetc(f);
		if (feof(f) || ferror(f))
			break;
		Strcat_char(s, c);
		if (c == '\n')
			break;
	}
	return s;
}

Str
Strfgetall(FILE * f)
{
	Str s = Strnew();
	char c;
	while (1) {
		c = fgetc(f);
		if (feof(f) || ferror(f))
			break;
		Strcat_char(s, c);
	}
	return s;
}