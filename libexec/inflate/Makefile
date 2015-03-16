.include<../../config.mk>

PROG=		inflate
MAN=

CFLAGS+=	-Wall -I${PREFIX}/include

LDADD+=		-L${PREFIX}/lib -lz
BINDIR=		${PREFIX}/libexec/w3m

install:
	install -d ${BINDIR}
	install -m 755 inflate ${BINDIR}

.include <bsd.prog.mk>
