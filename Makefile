PROG=		w3m

SRCS=		Str.c anchor.c backend.c buffer.c cookie.c display.c \
		entity.c etc.c file.c form.c frame.c ftp.c func.c \
		hash.c history.c image.c indep.c istream.c linein.c \
		local.c mailcap.c main.c map.c menu.c mimehead.c \
		news.c myctype.c parsetag.c parsetagx.c rc.c regex.c \
		search.c symbol.c table.c terms.c textlist.c url.c \
		version.c tagtable.c

SRCS+=		keybind.c

MAN=		doc/w3m.1

PREFIX?=	/usr/local
SYS_CONF_DIR=	/etc
LIBEXEC_DIR=	${PREFIX}/libexec
LOCALE_DIR=	/usr/share/locale

CFLAGS+=	-Wall -I${PREFIX}/include -I. -I./libwc \
		-DCGIBIN_DIR="\"${LIBEXEC_DIR}/${PROG}/cgi-bin\"" \
		-DAUXBIN_DIR="\"${LIBEXEC_DIR}/${PROG}\"" \
		-DHELP_DIR="\"${PREFIX}/share/${PROG}\"" \
		-DRC_DIR="\"~/.${PROG}\"" \
		-DETC_DIR="\"${SYS_CONF_DIR}\"" \
		-DCONF_DIR="\"${SYS_CONF_DIR}/${PROG}\"" \
		-DLOCALEDIR="\"${LOCALE_DIR}\""

LDADD+=		-L${PREFIX}/lib -lssl -lcrypto -lgc -lm -lintl \
		-ltermcap ./libwc/libwc.a

.include <bsd.prog.mk>
