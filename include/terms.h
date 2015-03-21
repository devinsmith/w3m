/* $Id: terms.h,v 1.10 2004/07/15 16:32:39 ukai Exp $ */
#ifndef TERMS_H
#define TERMS_H

extern int LINES, COLS;

#ifdef USE_MOUSE
/* Addition:mouse event */
#define MOUSE_BTN1_DOWN 0
#define MOUSE_BTN2_DOWN 1
#define MOUSE_BTN3_DOWN 2
#define MOUSE_BTN4_DOWN_RXVT 3
#define MOUSE_BTN5_DOWN_RXVT 4
#define MOUSE_BTN4_DOWN_XTERM 64
#define MOUSE_BTN5_DOWN_XTERM 65
#define MOUSE_BTN_UP 3
#define MOUSE_BTN_RESET -1
#endif

extern int initscr(void);
extern int set_tty(void);
extern void set_cc(int spec, int val);
extern void close_tty(void);
extern char *ttyname_tty(void);
extern void reset_tty(void);
extern MySignalHandler reset_exit(SIGNAL_ARG);
extern MySignalHandler error_dump(SIGNAL_ARG);
extern void set_int(void);
extern void getTCstr(void);
extern void setlinescols(void);
extern void setupscreen(void);
extern int initscr(void);
extern void move(int line, int column);
extern void addch(char c);
#ifdef USE_M17N
	extern void addmch(const char *p, size_t len);
#endif
extern void addch(char c);
extern void wrap(void);
extern void touch_line(void);

extern void standout(void);
extern void standend(void);
extern void bold(void);
extern void boldend(void);
extern void underline(void);
extern void underlineend(void);
extern void graphstart(void);
extern void graphend(void);
extern int graph_ok(void);

#ifdef USE_COLOR
	extern void setfcolor(int color);
#ifdef USE_BG_COLOR
	extern void setbcolor(int color);
#endif /* USE_BG_COLOR */
#endif /* USE_COLOR */

extern void refresh(void);
extern void clear(void);

#ifdef USE_RAW_SCROLL
	extern void scroll(int);
	extern void rscroll(int);
#endif

extern void clrtoeol(void);
extern void clrtoeolx(void);
extern void clrtobot(void);
extern void clrtobotx(void);

extern void addstr(const char *s);
extern void addnstr(const char *s, int n);
extern void addnstr_sup(const char *s, int n);

extern void crmode(void);
extern void nocrmode(void);

extern void term_echo(void);
extern void term_noecho(void);
extern void term_raw(void);
extern void term_cooked(void);
extern void term_cbreak(void);
extern void term_title(char *s);

extern void flush_tty(void);
extern void toggle_stand(void);
extern char getch(void);
extern void bell(void);

extern int sleep_till_anykey(int sec, int purge);
#ifdef USE_IMAGE
	extern void touch_cursor();
#endif

#ifdef USE_MOUSE
	extern void mouse_init(void);
	extern void mouse_end(void);
	extern void mouse_active(void);
	extern void mouse_inactive(void);
#endif 

#endif /* not TERMS_H */
