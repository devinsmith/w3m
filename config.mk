CFLAGS+=	-fPIC -Wall

PREFIX?=	/usr/local

USE_IMAGE?=	yes
USE_M17N?=	yes

SYS_CONF_DIR?=	/etc
LIBEXEC_DIR?=	${PREFIX}/libexec/w3s
LOCALE_DIR?=	/usr/share/locale
HELP_DIR?=	${PREFIX}/share/doc/w3s
CFLAGS+=	-DCGIBIN_DIR="\"${LIBEXEC_DIR}/cgi-bin\"" \
		-DAUXBIN_DIR="\"${LIBEXEC_DIR}\"" \
		-DHELP_DIR="\"${HELP_DIR}\"" \
		-DRC_DIR="\"~/.w3s\"" \
		-DETC_DIR="\"${SYS_CONF_DIR}\"" \
		-DCONF_DIR="\"${SYS_CONF_DIR}/w3s\"" \
		-DLOCALEDIR="\"${LOCALE_DIR}\""
