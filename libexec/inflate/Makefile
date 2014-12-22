PROG=		inflate
MAN=

PREFIX?=	/usr/local

CFLAGS+=	-Wall -I${PREFIX}/include

LDADD+=		-L${PREFIX}/lib -lz
BINDIR=		${PREFIX}/libexec/w3m

.include <bsd.prog.mk>
