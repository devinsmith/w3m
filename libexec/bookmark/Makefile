PROG=		w3mbookmark
MAN=

PREFIX?=	/usr/local

CFLAGS+=	-Wall -I../../include -I${PREFIX}/include

LDADD+=		-L${PREFIX}/lib -L../../libwc -lgc \
		../../libwc/libwc.a ../../libindep/libindep.a \
		../dep-dummy/dummy.o

BINDIR=		${PREFIX}/libexec/w3m

.include <bsd.prog.mk>
