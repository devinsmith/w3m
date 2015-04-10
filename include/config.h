/* config.h.  Generated from config.h.in by configure.  */
#ifndef CONFIG_H_SEEN
#define CONFIG_H_SEEN
/*
 * Configuration for w3m
 */
#define JA 0
#define EN 1

/* Name of package */
#define PACKAGE "w3m"

#define W3MCONFIG    "w3mconfig"
#define IMGDISPLAY   "w3mimgdisplay"
#define XFACE2XPM    "xface2xpm"

#define BOOKMARK     "bookmark.html"
#define CONFIG_FILE  "config"
#define KEYMAP_FILE  "keymap"
#define MENU_FILE    "menu"
#define MOUSE_FILE   "mouse"
#define COOKIE_FILE  "cookie"
#define HISTORY_FILE "history"

#define PASSWD_FILE	RC_DIR "/passwd"
#define PRE_FORM_FILE	RC_DIR "/pre_form"
#define USER_MAILCAP	RC_DIR "/mailcap"
#define SYS_MAILCAP	CONF_DIR "/mailcap"
#define USER_MIMETYPES	"~/.mime.types"
#define SYS_MIMETYPES	ETC_DIR "/mime.types"
#define USER_URIMETHODMAP	RC_DIR "/urimethodmap"
#define SYS_URIMETHODMAP	CONF_DIR "/urimethodmap"

#define DEF_SAVE_FILE "index.html"

/* User Configuration */
#define DISPLAY_CHARSET WC_CES_UTF_8
#define SYSTEM_CHARSET WC_CES_UTF_8
#define DOCUMENT_CHARSET WC_CES_UTF_8
#define W3M_LANG EN
#define LANG W3M_LANG

/*
 * Define to 1 if translation of program messages to the user's native
 * language is requested.
 */
/* #define ENABLE_NLS 0 */

/* #undef USE_MIGEMO */
#define USE_MARK
/* #define USE_MOUSE */
/* #undef USE_GPM */
/* #undef USE_SYSMOUSE */
#define USE_MENU 1
#define USE_COOKIE 1
#define USE_DIGEST_AUTH 1
#define USE_EXTERNAL_URI_LOADER 1
#define USE_W3MMAILER 1
#define USE_GOPHER 1
#define USE_ALARM 1
#define USE_XFACE 1
#define USE_DICT 1
#define USE_HISTORY 1
/* #undef FORMAT_NICE */
#define ID_EXT
/* #undef CLEAR_BUF */
#define INET6 1
/* #undef HAVE_OLD_SS_FAMILY */
#define ENABLE_REMOVE_TRAILINGSPACES
/* #undef MENU_THIN_FRAME */
/* #undef USE_RAW_SCROLL */
/* #undef TABLE_EXPAND */
/* #undef TABLE_NO_COMPACT */
#define NOWRAP

#define DEF_EDITOR "/usr/bin/vi"
#define DEF_MAILER "/usr/bin/mail"
#define DEF_EXT_BROWSER "/usr/bin/firefox"

/* fallback viewer. mailcap override these configuration */
#define DEF_IMAGE_VIEWER	"feh"
#define DEF_AUDIO_PLAYER	"mplayer"

/* for USE_MIGEMO */
#define DEF_MIGEMO_COMMAND "migemo -t egrep /usr/local/share/migemo/migemo-dict"

/* #undef USE_BINMODE_STREAM */
#define HAVE_TERMIOS_H 1
/* #undef HAVE_TERMIO_H */
/* #undef HAVE_SGTTY_H */
#define SIZEOF_LONG_LONG 8
#if SIZEOF_LONG_LONG > 0
typedef long long clen_t;
#else
typedef long clen_t;
#endif
#define HAVE_STRCASESTR 1
#define HAVE_SYS_ERRLIST 1
/* #undef HAVE_GETPASSPHRASE */
#define HAVE_LANGINFO_CODESET 1

#define RETSIGTYPE void
typedef RETSIGTYPE MySignalHandler;
#define SIGNAL_ARG int _dummy	/* XXX */
#define SIGNAL_ARGLIST 0	/* XXX */
#define SIGNAL_RETURN return

#define SETJMP(env) sigsetjmp(env,1)
#define LONGJMP(env,val) siglongjmp(env,val)
#define JMP_BUF sigjmp_buf

#define DEFAULT_TERM	0	/* XXX */

#define GUNZIP_CMDNAME  "gunzip"
#define INFLATE_CMDNAME	"inflate"
#define W3MBOOKMARK_CMDNAME	"w3mbookmark"
#define W3MHELPERPANEL_CMDNAME	"w3mhelperpanel"
#define DEV_TTY_PATH	"/dev/tty"
#define CGI_EXTENSION	".cgi"

#define PATH_SEPARATOR	':'
#define GUNZIP_NAME  "gunzip"
#define INFLATE_NAME "inflate"

#endif				/* CONFIG_H_SEEN */
