/* $Id: terms.c,v 1.63 2010/08/20 09:34:47 htrb Exp $ */
/*
 * An original curses library for EUC-kanji by Akinori ITO,     December 1989
 * revised by Akinori ITO, January 1995
 * gutted by Scarlett, April 2015
 */
#include <termios.h>
#include <curses.h>
#include <limits.h>
#include <errno.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "fm.h"
#include "terms.h"
#ifdef USE_M17N
#include "wc.h"
#include "wtf.h"
#endif

/* XXX: including term.h conflicts with hash.h via fm.h */
extern int tputs(const char *, int, int (*)(int));
char *tgetstr(char *, char **);

static int line_drawing_has_init = 0;
static char *T_as = "";
static char *T_ae = "";
static char *T_ac = "";

static void
line_drawing_init(void)
{
	char str[32];
	char *p = str;
	char *temp;

	if (line_drawing_has_init) return;

	/* enable alternative charset */
	temp = tgetstr("eA", &p);
	if (temp != NULL)
		tputs(temp, 1, putchar);

	/* alternative (graphic) charset start */
	temp = tgetstr("as", &p);
	if (temp != NULL)
		T_as = strdup(temp);

	/* alternative (graphic) charset end */
	temp = tgetstr("ae", &p);
	if (temp != NULL)
		T_ae = strdup(temp);

	/* graphics charset pairs */
	temp = tgetstr("ac", &p);
	if (temp != NULL)
		T_ac = strdup(temp);

	line_drawing_has_init = 1;
}

void
touch_cursor()
{
}

void
graphstart(void)
{
	line_drawing_init();
	tputs(T_as, 1, putchar);
}

void
graphend(void)
{
	tputs(T_ae, 1, putchar);
}

int
graph_ok(void)
{
	if (UseGraphicChar != GRAPHIC_CHAR_DEC)
		return 0;
	return T_as[0] != 0 && T_ae[0] != 0 && T_ac[0] != 0;
}

char *
ttyname_tty(void)
{
	return ttyname(STDIN_FILENO);
}

void
close_tty(void)
{
}

void
flush_tty(void)
{
	fflush(stdout);
}

void
reset_tty(void)
{
}

void
toggle_stand(int nchars)
{
	attr_t attrs = 0;
	short pair = 0;

	attr_get(&attrs, &pair, NULL);
	if ((attrs & A_STANDOUT) != 0) {
		attrs &= ~A_STANDOUT;
	} else {
		attrs |= A_STANDOUT;
	}
	chgat(nchars, attrs, 0, NULL);
}

void
term_title(const char *s)
{
	/*printf("w3m: %s", s);*/
}

#ifdef USE_M17N
void
addmch(const char *pc, size_t len)
{
	char str[5];

	if (len == SIZE_MAX) {
		wc_putc(pc);
	} else if (len == 1) {
		addch(*pc);
	} else {
		if (len < sizeof(str)) {
			(void)memcpy(str, pc, len);
			str[len] = '\0';
			wc_putc(str);
		}
	}
}
#endif

void
addmstr(const char *s)
{
#ifdef USE_M17N
	int len;

	while (*s != '\0') {
		len = wtf_len((const wc_uchar *) s);
		addmch(s, len);
		s += len;
	}
#else
	addstr(s);
#endif
}

void
addnstr_sup(const char *s, int n)
{
	int i;
#ifdef USE_M17N
	int len, width;

	for (i = 0; *s != '\0';) {
		width = wtf_width((const wc_uchar *) s);
		if (i + width > n)
			break;
		len = wtf_len((const wc_uchar *) s);
		addmch(s, len);
		s += len;
		i += width;
	}
#else
	for (i = 0; i < n && *s != '\0'; i++)
		addch(*(s++));
#endif
	for (; i < n; i++)
		addch(' ');
}

int
sleep_till_anykey(int sec, int purge)
{
	struct pollfd pfd[1];
	int er, c, ret;
	struct termios ioval;

	tcgetattr(STDIN_FILENO, &ioval);
	raw();

	pfd[0].fd = STDIN_FILENO;
	pfd[0].events = POLLIN;

	ret = poll(pfd, 1, sec * 1000);
	if (ret > 0 && purge) {
		c = getch();
		if (c == ESC_CODE)
			skip_escseq();
	}
	er = tcsetattr(STDIN_FILENO, TCSANOW, &ioval);
	if (er == -1) {
		printf("Error occured: errno=%d\n", errno);
		exit(1);
	}
	return ret;
}

void
skip_escseq(void)
{
	int c;

	c = getch();
	if (c == '[' || c == 'O') {
		c = getch();
		while (isdigit((unsigned char)c))
			c = getch();
	}
}


