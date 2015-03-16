.include <../../config.mk>

PROG=		w3mhelperpanel
MAN=

CFLAGS+=	-Wall -I../../include -I${PREFIX}/include

LDADD+=		-L${PREFIX}/lib -lgc \
		../../libindep/libindep.a \
		../dep-dummy/dummy.o

BINDIR=		${PREFIX}/libexec/w3m

install:
	install -d ${BINDIR}
	install -m 755 w3mhelperpanel ${BINDIR}

.include <../../config.mk>
.include <bsd.prog.mk>
