.include <../config.mk>

LIB=		indep
SHLIB_MAJOR=	1
SHLIB_MINOR=	0

SRCS=		Str.c indep.c regex.c textlist.c parsetag.c \
		myctype.c hash.c

PREFIX?=	/usr/local

CFLAGS+=	-Wall -I../include -I${PREFIX}/include

LDADD+=		-L${PREFIX}/lib -lgc

.if (${USE_M17N:L} == "yes")
CFLAGS+=	-DUSE_M17N -I../libwc
LDADD+=		../libwc/libwc.a
.endif

install:

.include <bsd.lib.mk>
