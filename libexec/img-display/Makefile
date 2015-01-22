.include <bsd.xconf.mk>

PROG=		w3mimgdisplay
SRCS=		w3mimgdisplay.c x11_w3mimg.c
MAN=

PREFIX?=	/usr/local

CFLAGS+=	-Wall -I../../include -I${PREFIX}/include \
		-I${X11BASE}/include

LDADD+=		-L${PREFIX}/lib -L${X11BASE}/lib -lImlib2 \
		-lX11 -lXext -lxcb -lfreetype

BINDIR=		${PREFIX}/libexec/w3m

.include <bsd.prog.mk>
