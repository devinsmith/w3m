.include <../config.mk>

SUBDIR=		dep-dummy bookmark helper-panel inflate

.if (${USE_IMAGE:L} == "yes")
SUBDIR+=	img-display
.endif

.include <bsd.subdir.mk>
