LIB=		wc
SHLIB_MAJOR=	1
SHLIB_MINOR=	0

SRCS=		big5.c ces.c char_conv.c charset.c combining.c \
		conv.c detect.c gb18030.c gbk.c hkscs.c hz.c \
		iso2022.c jis.c johab.c priv.c putc.c search.c \
		sjis.c status.c ucs.c uhc.c utf7.c utf8.c viet.c \
		wtf.c ../libindep/Str.c

HDRS=		big5.h ccs.h ces.h gb18030.h gbk.h hkscs.h hz.h \
		iso2022.h jis.h johab.h priv.h search.h sjis.h \
		ucs.h uhc.h utf7.h utf8.h viet.h wc.h wc_types.h \
		wtf.h


PREFIX?=	/usr/local

CFLAGS+=	-Wall -I. -I../include -I${PREFIX}/include \
		-DUSE_UNICODE

LDADD+=		-L${PREFIX}/lib -lgc

install:

.include <bsd.lib.mk>
