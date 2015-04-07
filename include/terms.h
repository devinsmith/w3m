/* $Id: terms.h,v 1.10 2004/07/15 16:32:39 ukai Exp $ */
#ifndef TERMS_H
#define TERMS_H

char *ttyname_tty(void);
void close_tty(void);
void flush_tty(void);
void reset_tty(void);

void clrtoeolx(void);
void clrtobotx(void);

void bold(void);
void boldend(void);
void underline(void);
void underlineend(void);
void toggle_stand(int);

void bell(void);

void term_raw(void);
void term_cbreak(void);
void term_noecho(void);

void term_title(const char *);

#ifdef USE_M17N
void addmch(const char *, size_t);
#endif
void addnstr_sup(const char *, int);

int sleep_till_anykey(int, int);

void skip_escseq(void);

void graphstart(void);
void graphend(void);
int graph_ok(void);

void touch_cursor(void);

void addmstr(const char *);

#endif /* not TERMS_H */
