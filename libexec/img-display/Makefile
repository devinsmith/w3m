.include <bsd.xconf.mk>

PROG=		w3mimgdisplay
SRCS=		w3mimgdisplay.c w3mimg.c x11_w3mimg.c
MAN=

PREFIX?=	/usr/local

CFLAGS+=	-Wall -I../../include -I${PREFIX}/include \
		-DUSE_W3MIMG_X11 -I${X11BASE}/include -DUSE_IMLIB2 \
		-DW3MIMGDISPLAY_SETUID

LDADD+=		-L${PREFIX}/lib -L${X11BASE}/lib -lImlib2 \
		-lX11 -lXext -lxcb -lfreetype

BINDIR=		${PREFIX}/libexec/w3m

.include <bsd.prog.mk>
