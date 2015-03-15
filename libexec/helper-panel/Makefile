PROG=		w3mhelperpanel
MAN=

PREFIX?=	/usr/local

CFLAGS+=	-Wall -I../../include -I${PREFIX}/include

LDADD+=		-L${PREFIX}/lib -lgc \
		../../libindep/libindep.a \
		../dep-dummy/dummy.o

BINDIR=		${PREFIX}/libexec/w3m

.include <../../config.mk>
.include <bsd.prog.mk>
