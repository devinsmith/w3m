LIB=		indep
SHLIB_MAJOR=	1
SHLIB_MINOR=	0

SRCS=		Str.c indep.c regex.c textlist.c parsetag.c \
		myctype.c hash.c

PREFIX?=	/usr/local

CFLAGS+=	-Wall -I../include -I../libwc -I${PREFIX}/include

LDADD+=		-L${PREFIX}/lib -lgc

install:

.include <../config.mk>
.include <bsd.lib.mk>
