/* $Id: istream.c,v 1.27 2010/07/18 13:43:23 htrb Exp $ */
#include "fm.h"
#include "istream.h"
#include <unistd.h>
#include <signal.h>
#include <curses.h>
#include <tls.h>

#define	uchar		unsigned char

#define STREAM_BUF_SIZE 8192
#define TLS_BUF_SIZE	1536

#define MUST_BE_UPDATED(bs) ((bs)->stream.cur==(bs)->stream.next)

#define POP_CHAR(bs) ((bs)->iseos?'\0':(bs)->stream.buf[(bs)->stream.cur++])

static void basic_close(int *handle);
static int basic_read(int *handle, char *buf, int len);

static void file_close(struct file_handle * handle);
static int file_read(struct file_handle * handle, char *buf, int len);

static int str_read(Str handle, char *buf, int len);

static int ens_read(struct ens_handle * handle, char *buf, int len);
static void ens_close(struct ens_handle * handle);

static void
do_update(BaseStream base)
{
	int len;
	base->stream.cur = base->stream.next = 0;
	len = base->read(base->handle, base->stream.buf, base->stream.size);
	if (len <= 0)
		base->iseos = TRUE;
	else
		base->stream.next += len;
}

static int
buffer_read(StreamBuffer sb, char *obuf, int count)
{
	int len = sb->next - sb->cur;
	if (len > 0) {
		if (len > count)
			len = count;
		memcpy(obuf, &sb->buf[sb->cur], len);
		sb->cur += len;
	}
	return len;
}

static void
init_buffer(BaseStream base, char *buf, int bufsize)
{
	StreamBuffer sb = &base->stream;
	sb->size = bufsize;
	sb->cur = 0;
	if (buf) {
		sb->buf = (uchar *) buf;
		sb->next = bufsize;
	} else {
		sb->buf = NewAtom_N(uchar, bufsize);
		sb->next = 0;
	}
	base->iseos = FALSE;
}

static void
init_base_stream(BaseStream base, int bufsize)
{
	init_buffer(base, NULL, bufsize);
}

static void
init_str_stream(BaseStream base, Str s)
{
	init_buffer(base, s->ptr, s->length);
}

InputStream
newInputStream(int des)
{
	InputStream stream;
	if (des < 0)
		return NULL;
	stream = New(union input_stream);
	init_base_stream(&stream->base, STREAM_BUF_SIZE);
	stream->base.type = IST_BASIC;
	stream->base.handle = New(int);
	*(int *) stream->base.handle = des;
	stream->base.read = (int (*) ()) basic_read;
	stream->base.close = (void (*) ()) basic_close;
	return stream;
}

InputStream
newFileStream(FILE * f, void (*closep) ())
{
	InputStream stream;
	if (f == NULL)
		return NULL;
	stream = New(union input_stream);
	init_base_stream(&stream->base, STREAM_BUF_SIZE);
	stream->file.type = IST_FILE;
	stream->file.handle = New(struct file_handle);
	stream->file.handle->f = f;
	if (closep)
		stream->file.handle->close = closep;
	else
		stream->file.handle->close = (void (*) ()) fclose;
	stream->file.read = (int (*) ()) file_read;
	stream->file.close = (void (*) ()) file_close;
	return stream;
}

InputStream
newStrStream(Str s)
{
	InputStream stream;
	if (s == NULL)
		return NULL;
	stream = New(union input_stream);
	init_str_stream(&stream->base, s);
	stream->str.type = IST_STR;
	stream->str.handle = s;
	stream->str.read = (int (*) ()) str_read;
	stream->str.close = NULL;
	return stream;
}

static int 
tls_read_is(struct tls_handle *tls, char *buf, int len)
{
	size_t outlen;
	if (tls_read(tls->tls, buf, len, &outlen) == -1 || outlen > INT_MAX) {
		return -1;
	}
	return (int) outlen;
}

static void
tls_close_is(struct tls_handle *tls)
{
	(void)tls_close(tls->tls);
}

InputStream
newTLSStream(struct tls *tls, int sock)
{
	InputStream stream;
	if (sock < 0)
		return NULL;
	stream = New(union input_stream);
	init_base_stream(&stream->base, TLS_BUF_SIZE);
	stream->tls.type = IST_TLS;
	stream->tls.handle = New(struct tls_handle);
	stream->tls.handle->tls = tls;
	stream->tls.handle->sock = sock;
	stream->tls.read = (int (*) ()) tls_read_is;
	stream->tls.close = (void (*) ()) tls_close_is;
	return stream;
}

InputStream
newEncodedStream(InputStream is, char encoding)
{
	InputStream stream;
	if (is == NULL || (encoding != ENC_QUOTE && encoding != ENC_BASE64 &&
			   encoding != ENC_UUENCODE))
		return is;
	stream = New(union input_stream);
	init_base_stream(&stream->base, STREAM_BUF_SIZE);
	stream->ens.type = IST_ENCODED;
	stream->ens.handle = New(struct ens_handle);
	stream->ens.handle->is = is;
	stream->ens.handle->pos = 0;
	stream->ens.handle->encoding = encoding;
	stream->ens.handle->s = NULL;
	stream->ens.read = (int (*) ()) ens_read;
	stream->ens.close = (void (*) ()) ens_close;
	return stream;
}

int
ISclose(InputStream stream)
{
	void(*prevtrap) ();
	if (stream == NULL || stream->base.close == NULL ||
	    stream->base.type & IST_UNCLOSE)
		return -1;
	prevtrap = mySignal(SIGINT, SIG_IGN);
	stream->base.close(stream->base.handle);
	mySignal(SIGINT, prevtrap);
	return 0;
}

int
ISgetc(InputStream stream)
{
	BaseStream base;
	if (stream == NULL)
		return '\0';
	base = &stream->base;
	if (!base->iseos && MUST_BE_UPDATED(base))
		do_update(base);
	return POP_CHAR(base);
}

int
ISundogetc(InputStream stream)
{
	StreamBuffer sb;
	if (stream == NULL)
		return -1;
	sb = &stream->base.stream;
	if (sb->cur > 0) {
		sb->cur--;
		return 0;
	}
	return -1;
}

#define MARGIN_STR_SIZE 10
Str
StrISgets(InputStream stream)
{
	BaseStream base;
	StreamBuffer sb;
	Str s = NULL;
	uchar *p;
	int len;

	if (stream == NULL)
		return '\0';
	base = &stream->base;
	sb = &base->stream;

	while (!base->iseos) {
		if (MUST_BE_UPDATED(base)) {
			do_update(base);
		} else {
			if ((p = memchr(&sb->buf[sb->cur], '\n', sb->next - sb->cur))) {
				len = p - &sb->buf[sb->cur] + 1;
				if (s == NULL)
					s = Strnew_size(len);
				Strcat_charp_n(s, (char *) &sb->buf[sb->cur], len);
				sb->cur += len;
				return s;
			} else {
				if (s == NULL)
					s = Strnew_size(sb->next - sb->cur + MARGIN_STR_SIZE);
				Strcat_charp_n(s, (char *) &sb->buf[sb->cur],
					       sb->next - sb->cur);
				sb->cur = sb->next;
			}
		}
	}

	if (s == NULL)
		return Strnew();
	return s;
}

Str
StrmyISgets(InputStream stream)
{
	BaseStream base;
	StreamBuffer sb;
	Str s = NULL;
	int i, len;

	if (stream == NULL)
		return '\0';
	base = &stream->base;
	sb = &base->stream;

	while (!base->iseos) {
		if (MUST_BE_UPDATED(base)) {
			do_update(base);
		} else {
			if (s && Strlastchar(s) == '\r') {
				if (sb->buf[sb->cur] == '\n')
					Strcat_char(s, (char) sb->buf[sb->cur++]);
				return s;
			}
			for (i = sb->cur;
			     i < sb->next && sb->buf[i] != '\n' && sb->buf[i] != '\r';
			     i++);
			if (i < sb->next) {
				len = i - sb->cur + 1;
				if (s == NULL)
					s = Strnew_size(len + MARGIN_STR_SIZE);
				Strcat_charp_n(s, (char *) &sb->buf[sb->cur], len);
				sb->cur = i + 1;
				if (sb->buf[i] == '\n')
					return s;
			} else {
				if (s == NULL)
					s = Strnew_size(sb->next - sb->cur + MARGIN_STR_SIZE);
				Strcat_charp_n(s, (char *) &sb->buf[sb->cur],
					       sb->next - sb->cur);
				sb->cur = sb->next;
			}
		}
	}

	if (s == NULL)
		return Strnew();
	return s;
}

int
ISread(InputStream stream, Str buf, int count)
{
	int rest, len;
	BaseStream base;

	if (stream == NULL || (base = &stream->base)->iseos)
		return 0;

	len = buffer_read(&base->stream, buf->ptr, count);
	rest = count - len;
	if (MUST_BE_UPDATED(base)) {
		len = base->read(base->handle, &buf->ptr[len], rest);
		if (len <= 0) {
			base->iseos = TRUE;
			len = 0;
		}
		rest -= len;
	}
	Strtruncate(buf, count - rest);
	if (buf->length > 0)
		return 1;
	return 0;
}

int
ISfileno(InputStream stream)
{
	if (stream == NULL)
		return -1;
	switch (IStype(stream) & ~IST_UNCLOSE) {
	case IST_BASIC:
		return *(int *) stream->base.handle;
	case IST_FILE:
		return fileno(stream->file.handle->f);
	case IST_TLS:
		return stream->tls.handle->sock;
	case IST_ENCODED:
		return ISfileno(stream->ens.handle->is);
	default:
		return -1;
	}
}

int
ISeos(InputStream stream)
{
	BaseStream base = &stream->base;
	if (!base->iseos && MUST_BE_UPDATED(base))
		do_update(base);
	return base->iseos;
}


/* Raw level input stream functions */

static void
basic_close(int *handle)
{
	close(*(int *) handle);
}

static int
basic_read(int *handle, char *buf, int len)
{
	return read(*(int *) handle, buf, len);
}

static void
file_close(struct file_handle * handle)
{
	handle->close(handle->f);
}

static int
file_read(struct file_handle * handle, char *buf, int len)
{
	return fread(buf, 1, len, handle->f);
}

static int
str_read(Str handle, char *buf, int len)
{
	return 0;
}

static void
ens_close(struct ens_handle * handle)
{
	ISclose(handle->is);
}

static int
ens_read(struct ens_handle * handle, char *buf, int len)
{
	if (handle->s == NULL || handle->pos == handle->s->length) {
		char *p;
		handle->s = StrmyISgets(handle->is);
		if (handle->s->length == 0)
			return 0;
		cleanup_line(handle->s, PAGER_MODE);
		if (handle->encoding == ENC_BASE64)
			Strchop(handle->s);
		else if (handle->encoding == ENC_UUENCODE) {
			if (!strncmp(handle->s->ptr, "begin", 5))
				handle->s = StrmyISgets(handle->is);
			Strchop(handle->s);
		}
		p = handle->s->ptr;
		if (handle->encoding == ENC_QUOTE)
			handle->s = decodeQP(&p);
		else if (handle->encoding == ENC_BASE64)
			handle->s = decodeB(&p);
		else if (handle->encoding == ENC_UUENCODE)
			handle->s = decodeU(&p);
		handle->pos = 0;
	}
	if (len > handle->s->length - handle->pos)
		len = handle->s->length - handle->pos;

	memcpy(buf, &handle->s->ptr[handle->pos], len);
	handle->pos += len;
	return len;
}
