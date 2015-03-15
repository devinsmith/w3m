.include <../config.mk>

PROG=		w3m

SRCS=		anchor.c backend.c buffer.c cookie.c display.c \
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
CFLAGS+=	-DUSE_M17N -DUSE_UNICODE -I../libwc
LDADD+=		../libwc/libwc.a
.endif

SRCS+=		keybind.c

MAN=		../doc/w3m.1

CFLAGS+=	-I${PREFIX}/include -I../include

CFLAGS+=	-Wall
CFLAGS+=	-Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+=	-Wsign-compare

LDADD+=		-L${PREFIX}/lib -ltls -lssl -lcrypto -lgc -lm \
		-ltermcap ../libindep/libindep.a

BINDIR=		${PREFIX}/bin
MANDIR=		${PREFIX}/man/man

.include <bsd.prog.mk>
