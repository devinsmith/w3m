PROG=		w3m

SRCS=		anchor.c backend.c buffer.c cookie.c display.c \
		entity.c etc.c file.c form.c frame.c ftp.c func.c \
		history.c image.c istream.c linein.c local.c \
		mailcap.c main.c map.c menu.c mimehead.c news.c \
		parsetagx.c rc.c search.c symbol.c table.c terms.c \
		url.c version.c tagtable.c

SRCS+=		keybind.c

MAN=		../doc/w3m.1

PREFIX?=	/usr/local

CFLAGS+=	-I${PREFIX}/include -I../libwc -I../include

CFLAGS+=	-Wall
CFLAGS+=	-Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+=	-Wsign-compare

LDADD+=		-L${PREFIX}/lib -ltls -lssl -lcrypto -lgc -lm -lintl \
		-ltermcap ../libwc/libwc.a ../libindep/libindep.a

BINDIR=		${PREFIX}/bin
MANDIR=		${PREFIX}/man/man

.include <../config.mk>
.include <bsd.prog.mk>
