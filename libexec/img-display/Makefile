.include <bsd.xconf.mk>
.include <../../config.mk>

PROG=		w3mimgdisplay
SRCS=		img-display.c x11-imlib2.c
MAN=

CFLAGS+=	-Wall -I../../include -I${PREFIX}/include \
		-I${X11BASE}/include

LDADD+=		-L${PREFIX}/lib -L${X11BASE}/lib -lImlib2 \
		-lX11 -lXext -lxcb -lfreetype

BINDIR=		${PREFIX}/libexec/w3m

install:
	install -d ${BINDIR}
	install -m 755 w3mimgdisplay ${BINDIR}

.include <bsd.prog.mk>
