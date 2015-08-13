.include <../../config.mk>

PROG=		helperpanel
MAN=

CFLAGS+=	-Wall -I../../include -I${PREFIX}/include

LDADD+=		-L${PREFIX}/lib -lgc \
		../../libindep/libindep.a \
		../dep-dummy/dummy.o

BINDIR?=	${LIBEXEC_DIR}/cgi-bin

install:
	install -d ${BINDIR}
	install -m 755 helperpanel ${BINDIR}

.include <../../config.mk>
.include <bsd.prog.mk>
