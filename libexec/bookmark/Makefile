.include <../../config.mk>

PROG=		bookmark
MAN=

CFLAGS+=	-I../../include -I${PREFIX}/include

LDADD+=		-L${PREFIX}/lib -lgc \
		../../libindep/libindep.a \
		../dep-dummy/dummy.o

BINDIR?=	${LIBEXEC_DIR}/cgi-bin

install:
	install -d ${BINDIR}
	install -m 755 bookmark ${BINDIR}

.include <bsd.prog.mk>
