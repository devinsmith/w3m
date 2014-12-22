SYS_CONF_DIR=	/etc
LIBEXEC_DIR=	${PREFIX}/libexec
LOCALE_DIR=	/usr/share/locale
CFLAGS+=	-DCGIBIN_DIR="\"${LIBEXEC_DIR}/${PROG}/cgi-bin\"" \
		-DAUXBIN_DIR="\"${LIBEXEC_DIR}/${PROG}\"" \
		-DHELP_DIR="\"${PREFIX}/share/${PROG}\"" \
		-DRC_DIR="\"~/.${PROG}\"" \
		-DETC_DIR="\"${SYS_CONF_DIR}\"" \
		-DCONF_DIR="\"${SYS_CONF_DIR}/${PROG}\"" \
		-DLOCALEDIR="\"${LOCALE_DIR}\""
