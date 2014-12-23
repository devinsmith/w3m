SYS_CONF_DIR=	/etc
LIBEXEC_DIR=	${PREFIX}/libexec
LOCALE_DIR=	/usr/share/locale
CFLAGS+=	-DCGIBIN_DIR="\"${LIBEXEC_DIR}/w3m/cgi-bin\"" \
		-DAUXBIN_DIR="\"${LIBEXEC_DIR}/w3m\"" \
		-DHELP_DIR="\"${PREFIX}/share/w3m\"" \
		-DRC_DIR="\"~/.w3m\"" \
		-DETC_DIR="\"${SYS_CONF_DIR}\"" \
		-DCONF_DIR="\"${SYS_CONF_DIR}/w3m\"" \
		-DLOCALEDIR="\"${LOCALE_DIR}\""
