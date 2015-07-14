.include <../config.mk>

LIB=		wc
SHLIB_MAJOR=	1
SHLIB_MINOR=	0

SRCS=		big5.c ces.c char_conv.c charset.c combining.c \
		conv.c detect.c gb18030.c gbk.c hkscs.c hz.c \
		iso2022.c jis.c priv.c putc.c search.c sjis.c \
		status.c ucs.c uhc.c utf8.c viet.c wtf.c \
		../libindep/Str.c

HDRS=		big5.h ccs.h ces.h gb18030.h gbk.h hkscs.h hz.h \
		iso2022.h jis.h priv.h search.h sjis.h ucs.h \
		uhc.h utf8.h viet.h wc.h wc_types.h wtf.h

CFLAGS+=	-I. -I../include -I${PREFIX}/include

LDADD+=		-L${PREFIX}/lib -lcurses -lgc

install:

.include <bsd.lib.mk>
