.include <../config.mk>

PROG=		w3s

SRCS=		anchor.c buffer.c cookie.c display.c \
		entity.c etc.c file.c form.c frame.c ftp.c func.c \
		history.c istream.c linein.c local.c mailcap.c \
		main.c map.c menu.c mimehead.c parsetagx.c rc.c \
		search.c symbol.c table.c terms.c url.c version.c \
		tagtable.c

.if (${USE_IMAGE:L} == "yes")
SRCS+=		image.c
CFLAGS+=	-DUSE_IMAGE
.endif

.if (${USE_M17N:L} == "yes")
CFLAGS+=	-DUSE_M17N -I../libwc
.endif

SRCS+=		keybind.c

MAN=		../doc/w3s.1

CFLAGS+=	-I${PREFIX}/include -I../include

CFLAGS+=	-Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+=	-Wsign-compare

LDADD+=		-L${PREFIX}/lib -lm -ltermcap -lcurses \
		-ltls -lssl -lcrypto \
		-lgc ../libindep/libindep.a

.if (${USE_M17N:L} == "yes")
LDADD+=		../libwc/libwc.a
.endif

BINDIR?=	${PREFIX}/bin
MANDIR?=	${PREFIX}/man/man

.include <bsd.prog.mk>
