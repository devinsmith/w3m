/* $Id: indep.h,v 1.16 2003/09/22 21:02:19 ukai Exp $ */
#ifndef INDEP_H
#define INDEP_H
#include "Str.h"
#include "config.h"
#include "alloc.h"

#ifndef TRUE
#define TRUE 1
#endif				/* TRUE */
#ifndef FALSE
#define FALSE 0
#endif				/* FALSE */

#define RAW_MODE	0
#define PAGER_MODE	1
#define HTML_MODE	2
#define HEADER_MODE	3

extern const unsigned char QUOTE_MAP[];
extern const char *HTML_QUOTE_MAP[];
#define HTML_QUOTE_MASK   0x07	/* &, <, >, " */
#define SHELL_UNSAFE_MASK 0x08	/* [^A-Za-z0-9_./:\200-\377] */
#define URL_QUOTE_MASK    0x10	/* [\0- \177-\377] */
#define FILE_QUOTE_MASK   0x30	/* [\0- #%&+:?\177-\377] */
#define URL_UNSAFE_MASK   0x70	/* [^A-Za-z0-9_$\-.] */
#define GET_QUOTE_TYPE(c) QUOTE_MAP[(int)(unsigned char)(c)]
#define is_html_quote(c)   (GET_QUOTE_TYPE(c) & HTML_QUOTE_MASK)
#define is_shell_unsafe(c) (GET_QUOTE_TYPE(c) & SHELL_UNSAFE_MASK)
#define is_url_quote(c)    (GET_QUOTE_TYPE(c) & URL_QUOTE_MASK)
#define is_file_quote(c)   (GET_QUOTE_TYPE(c) & FILE_QUOTE_MASK)
#define is_url_unsafe(c)   (GET_QUOTE_TYPE(c) & URL_UNSAFE_MASK)
#define html_quote_char(c) HTML_QUOTE_MAP[(int)is_html_quote(c)]

extern char *conv_entity(unsigned int ch);
extern int getescapechar(char **s);
extern char *getescapecmd(char **s);
extern char *allocStr(const char *s, int len);
extern int strCmp(const void *s1, const void *s2);
extern char *cleanupName(const char *name);
extern char *expandPath(const char *name);
#ifndef HAVE_STRCASESTR
extern char *strcasestr(const char *s1, const char *s2);
#endif
extern int strcasemstr(char *str, char *srch[], char **ret_ptr);
extern char *remove_space(const char *str);
extern int non_null(const char *s);
extern void cleanup_line(Str s, int mode);
extern char *html_quote(const char *str);
extern char *html_unquote(char *str);
extern char *file_quote(const char *str);
extern char *file_unquote(const char *str);
extern char *url_quote(const char *str);
extern Str Str_url_unquote(Str x, int is_form, int safe);
extern Str Str_form_quote(Str x);
#define Str_form_unquote(x) Str_url_unquote((x), TRUE, FALSE)
extern const char *shell_quote(const char *str);

extern char *w3m_auxbin_dir();
extern char *w3m_lib_dir();
extern char *w3m_etc_dir();
extern char *w3m_conf_dir();
extern char *w3m_help_dir();

#endif				/* INDEP_H */
