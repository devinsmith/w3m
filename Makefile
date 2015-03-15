.include <config.mk>

.if (${USE_M17N:L} == "yes")
SUBDIR+=	libwc
.endif

SUBDIR+=	libindep libexec src doc

.include <bsd.subdir.mk>
