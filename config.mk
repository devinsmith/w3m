CFLAGS+=	-fPIC -Wall

PREFIX?=	/usr/local

USE_IMAGE?=	yes
USE_M17N?=	yes

SYS_CONF_DIR?=	/etc
LIBEXEC_DIR?=	${PREFIX}/libexec
LOCALE_DIR?=	/usr/share/locale
HELP_DIR?=	${PREFIX}/share/doc/w3m
CFLAGS+=	-DCGIBIN_DIR="\"${LIBEXEC_DIR}/w3m/cgi-bin\"" \
		-DAUXBIN_DIR="\"${LIBEXEC_DIR}/w3m\"" \
		-DHELP_DIR="\"${HELP_DIR}\"" \
		-DRC_DIR="\"~/.w3m\"" \
		-DETC_DIR="\"${SYS_CONF_DIR}\"" \
		-DCONF_DIR="\"${SYS_CONF_DIR}/w3m\"" \
		-DLOCALEDIR="\"${LOCALE_DIR}\""
