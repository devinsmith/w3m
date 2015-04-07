
#ifndef _WC_CCS_H
#define _WC_CCS_H

#include "iso2022.h"
#include "priv.h"

#define WC_F_ISO_BASE	0x40
#define WC_F_PCS_BASE	0x01
#define WC_F_WCS_BASE	0x00

#define WC_F_CS94_END	WC_F_GB_1988
#define WC_F_CS96_END	WC_F_ISO_8859_16
#define WC_F_CS94W_END	WC_F_JIS_X_0213_2
#define WC_F_CS96W_END	0
#define WC_F_CS942_END	0
#define WC_F_PCS_END	WC_F_RAW
#define WC_F_PCSW_END	WC_F_HKSCS_2
#define WC_F_WCS16_END	WC_F_UCS2
#define WC_F_WCS16W_END	WC_F_UCS2
#define WC_F_WCS32_END	WC_F_GB18030
#define WC_F_WCS32W_END	WC_F_GB18030

#define WC_CCS_A_SET		0x0FFFF
#define WC_CCS_A_TYPE		0x0FF00
#define WC_CCS_A_INDEX		0x000FF
#define WC_CCS_A_MBYTE		0x08000
#define WC_CCS_A_WIDE		0x10000
#define WC_CCS_A_COMB		0x20000
#define WC_CCS_A_ISO_2022	0x00700

#define WC_CCS_A_CS94		0x00100
#define WC_CCS_A_CS96		0x00200
#define WC_CCS_A_CS942		0x00400
#define WC_CCS_A_PCS		0x00800
#define WC_CCS_A_WCS16		0x01000
#define WC_CCS_A_WCS32		0x02000
#define WC_CCS_A_UNKNOWN	0x04000
#define WC_CCS_A_CS94W		(WC_CCS_A_CS94|WC_CCS_A_MBYTE)
#define WC_CCS_A_CS96W		(WC_CCS_A_CS96|WC_CCS_A_MBYTE)
#define WC_CCS_A_PCSW		(WC_CCS_A_PCS|WC_CCS_A_MBYTE)
#define WC_CCS_A_WCS16W		(WC_CCS_A_WCS16|WC_CCS_A_WIDE)
#define WC_CCS_A_WCS32W		(WC_CCS_A_WCS32|WC_CCS_A_WIDE)
#define WC_CCS_A_UNKNOWN_W	(WC_CCS_A_UNKNOWN|WC_CCS_A_MBYTE)
#define WC_CCS_A_CS94_C		(WC_CCS_A_CS94|WC_CCS_A_COMB)
#define WC_CCS_A_CS96_C		(WC_CCS_A_CS96|WC_CCS_A_COMB)
#define WC_CCS_A_CS942_C	(WC_CCS_A_CS942|WC_CCS_A_COMB)
#define WC_CCS_A_PCS_C		(WC_CCS_A_PCS|WC_CCS_A_COMB)
#define WC_CCS_A_WCS16_C	(WC_CCS_A_WCS16|WC_CCS_A_COMB)
#define WC_CCS_A_WCS32_C	(WC_CCS_A_WCS32|WC_CCS_A_COMB)
#define WC_CCS_A_CS94W_C	(WC_CCS_A_CS94W|WC_CCS_A_COMB)
#define WC_CCS_A_CS96W_C	(WC_CCS_A_CS96W|WC_CCS_A_COMB)
#define WC_CCS_A_PCSW_C		(WC_CCS_A_PCSW|WC_CCS_A_COMB)
#define WC_CCS_A_WCS16W_C	(WC_CCS_A_WCS16W|WC_CCS_A_COMB)
#define WC_CCS_A_WCS32W_C	(WC_CCS_A_WCS32W|WC_CCS_A_COMB)

#define WC_CCS_SET_CS94(c)	((c)|WC_CCS_A_CS94)
#define WC_CCS_SET_CS94W(c)	((c)|WC_CCS_A_CS94W)
#define WC_CCS_SET_CS96(c)	((c)|WC_CCS_A_CS96)
#define WC_CCS_SET_CS96W(c)	((c)|WC_CCS_A_CS96W)
#define WC_CCS_SET_CS942(c)	((c)|WC_CCS_A_CS942)
#define WC_CCS_SET_PCS(c)	((c)|WC_CCS_A_PCS)
#define WC_CCS_SET_PCSW(c)	((c)|WC_CCS_A_PCSW)
#define WC_CCS_SET_WCS16(c)	((c)|WC_CCS_A_WCS16)
#define WC_CCS_SET_WCS16W(c)	((c)|WC_CCS_A_WCS16W)
#define WC_CCS_SET_WCS32(c)	((c)|WC_CCS_A_WCS32)
#define WC_CCS_SET_WCS32W(c)	((c)|WC_CCS_A_WCS32W)

#define WC_CCS_IS_WIDE(c)	((c)&(WC_CCS_A_MBYTE|WC_CCS_A_WIDE))
#define WC_CCS_IS_COMB(c)	((c)&WC_CCS_A_COMB)
#define WC_CCS_IS_ISO_2022(c)	((c)&WC_CCS_A_ISO_2022)
#define WC_CCS_IS_UNKNOWN(c)	((c)&WC_CCS_A_UNKNOWN)

#define WC_CCS_SET(c)		((c)&WC_CCS_A_SET)
#define WC_CCS_TYPE(c)		((c)&WC_CCS_A_TYPE)
#define WC_CCS_INDEX(c)		((c)&WC_CCS_A_INDEX)
#define WC_CCS_GET_F(c)		WC_CCS_INDEX(c)

#define WC_CCS_US_ASCII		WC_CCS_SET_CS94(WC_F_US_ASCII)
#define WC_CCS_JIS_X_0201	WC_CCS_SET_CS94(WC_F_JIS_X_0201)
#define WC_CCS_JIS_X_0201K	WC_CCS_SET_CS94(WC_F_JIS_X_0201K)
#define WC_CCS_GB_1988		WC_CCS_SET_CS94(WC_F_GB_1988)

#define WC_CCS_ISO_8859_1	WC_CCS_SET_CS96(WC_F_ISO_8859_1)
#define WC_CCS_ISO_8859_2	WC_CCS_SET_CS96(WC_F_ISO_8859_2)
#define WC_CCS_ISO_8859_3	WC_CCS_SET_CS96(WC_F_ISO_8859_3)
#define WC_CCS_ISO_8859_4	WC_CCS_SET_CS96(WC_F_ISO_8859_4)
#define WC_CCS_ISO_8859_5	WC_CCS_SET_CS96(WC_F_ISO_8859_5)
#define WC_CCS_ISO_8859_6	WC_CCS_SET_CS96(WC_F_ISO_8859_6)
#define WC_CCS_ISO_8859_7	WC_CCS_SET_CS96(WC_F_ISO_8859_7)
#define WC_CCS_ISO_8859_8	WC_CCS_SET_CS96(WC_F_ISO_8859_8)
#define WC_CCS_ISO_8859_9	WC_CCS_SET_CS96(WC_F_ISO_8859_9)
#define WC_CCS_ISO_8859_10	WC_CCS_SET_CS96(WC_F_ISO_8859_10)
#define WC_CCS_ISO_8859_11	WC_CCS_SET_CS96(WC_F_ISO_8859_11)
#define WC_CCS_TIS_620		WC_CCS_ISO_8859_11
#define WC_CCS_ISO_8859_13	WC_CCS_SET_CS96(WC_F_ISO_8859_13)
#define WC_CCS_ISO_8859_14	WC_CCS_SET_CS96(WC_F_ISO_8859_14)
#define WC_CCS_ISO_8859_15	WC_CCS_SET_CS96(WC_F_ISO_8859_15)
#define WC_CCS_ISO_8859_16	WC_CCS_SET_CS96(WC_F_ISO_8859_16)

#define WC_CCS_SPECIAL		WC_CCS_SET_PCS(WC_F_SPECIAL)
#define WC_CCS_CP437		WC_CCS_SET_PCS(WC_F_CP437)
#define WC_CCS_CP737		WC_CCS_SET_PCS(WC_F_CP737)
#define WC_CCS_CP775		WC_CCS_SET_PCS(WC_F_CP775)
#define WC_CCS_CP850		WC_CCS_SET_PCS(WC_F_CP850)
#define WC_CCS_CP852		WC_CCS_SET_PCS(WC_F_CP852)
#define WC_CCS_CP855		WC_CCS_SET_PCS(WC_F_CP855)
#define WC_CCS_CP856		WC_CCS_SET_PCS(WC_F_CP856)
#define WC_CCS_CP857		WC_CCS_SET_PCS(WC_F_CP857)
#define WC_CCS_CP860		WC_CCS_SET_PCS(WC_F_CP860)
#define WC_CCS_CP861		WC_CCS_SET_PCS(WC_F_CP861)
#define WC_CCS_CP862		WC_CCS_SET_PCS(WC_F_CP862)
#define WC_CCS_CP863		WC_CCS_SET_PCS(WC_F_CP863)
#define WC_CCS_CP864		WC_CCS_SET_PCS(WC_F_CP864)
#define WC_CCS_CP865		WC_CCS_SET_PCS(WC_F_CP865)
#define WC_CCS_CP866		WC_CCS_SET_PCS(WC_F_CP866)
#define WC_CCS_CP869		WC_CCS_SET_PCS(WC_F_CP869)
#define WC_CCS_CP874		WC_CCS_SET_PCS(WC_F_CP874)
#define WC_CCS_CP1006		WC_CCS_SET_PCS(WC_F_CP1006)
#define WC_CCS_CP1250		WC_CCS_SET_PCS(WC_F_CP1250)
#define WC_CCS_CP1251		WC_CCS_SET_PCS(WC_F_CP1251)
#define WC_CCS_CP1252		WC_CCS_SET_PCS(WC_F_CP1252)
#define WC_CCS_CP1253		WC_CCS_SET_PCS(WC_F_CP1253)
#define WC_CCS_CP1254		WC_CCS_SET_PCS(WC_F_CP1254)
#define WC_CCS_CP1255		WC_CCS_SET_PCS(WC_F_CP1255)
#define WC_CCS_CP1256		WC_CCS_SET_PCS(WC_F_CP1256)
#define WC_CCS_CP1257		WC_CCS_SET_PCS(WC_F_CP1257)
#define WC_CCS_CP1258_1		WC_CCS_SET_PCS(WC_F_CP1258_1)
#define WC_CCS_CP1258_2		WC_CCS_SET_PCS(WC_F_CP1258_2)
#define WC_CCS_TCVN_5712_1	WC_CCS_SET_PCS(WC_F_TCVN_5712_1)
#define WC_CCS_TCVN_5712_2	WC_CCS_SET_PCS(WC_F_TCVN_5712_2)
#define WC_CCS_TCVN_5712_3	WC_CCS_SET_PCS(WC_F_TCVN_5712_3)
#define WC_CCS_VISCII_11_1	WC_CCS_SET_PCS(WC_F_VISCII_11_1)
#define WC_CCS_VISCII_11_2	WC_CCS_SET_PCS(WC_F_VISCII_11_2)
#define WC_CCS_VPS_1		WC_CCS_SET_PCS(WC_F_VPS_1)
#define WC_CCS_VPS_2		WC_CCS_SET_PCS(WC_F_VPS_2)
#define WC_CCS_KOI8_R		WC_CCS_SET_PCS(WC_F_KOI8_R)
#define WC_CCS_KOI8_U		WC_CCS_SET_PCS(WC_F_KOI8_U)
#define WC_CCS_NEXTSTEP		WC_CCS_SET_PCS(WC_F_NEXTSTEP)
#define WC_CCS_GBK_80		WC_CCS_SET_PCS(WC_F_GBK_80)
#define WC_CCS_RAW		WC_CCS_SET_PCS(WC_F_RAW)

#define WC_CCS_JIS_C_6226	WC_CCS_SET_CS94W(WC_F_JIS_C_6226)
#define WC_CCS_JIS_X_0208	WC_CCS_SET_CS94W(WC_F_JIS_X_0208)
#define WC_CCS_JIS_X_0212	WC_CCS_SET_CS94W(WC_F_JIS_X_0212)
#define WC_CCS_GB_2312		WC_CCS_SET_CS94W(WC_F_GB_2312)
#define WC_CCS_ISO_IR_165	WC_CCS_SET_CS94W(WC_F_ISO_IR_165)
#define WC_CCS_CNS_11643_1	WC_CCS_SET_CS94W(WC_F_CNS_11643_1)
#define WC_CCS_CNS_11643_2	WC_CCS_SET_CS94W(WC_F_CNS_11643_2)
#define WC_CCS_CNS_11643_3	WC_CCS_SET_CS94W(WC_F_CNS_11643_3)
#define WC_CCS_CNS_11643_4	WC_CCS_SET_CS94W(WC_F_CNS_11643_4)
#define WC_CCS_CNS_11643_5	WC_CCS_SET_CS94W(WC_F_CNS_11643_5)
#define WC_CCS_CNS_11643_6	WC_CCS_SET_CS94W(WC_F_CNS_11643_6)
#define WC_CCS_CNS_11643_7	WC_CCS_SET_CS94W(WC_F_CNS_11643_7)
#define WC_CCS_KS_X_1001	WC_CCS_SET_CS94W(WC_F_KS_X_1001)
#define WC_CCS_KPS_9566		WC_CCS_SET_CS94W(WC_F_KPS_9566)
#define WC_CCS_JIS_X_0213_1	WC_CCS_SET_CS94W(WC_F_JIS_X_0213_1)
#define WC_CCS_JIS_X_0213_2	WC_CCS_SET_CS94W(WC_F_JIS_X_0213_2)

#define WC_CCS_SPECIAL_W	WC_CCS_SET_PCSW(WC_F_SPECIAL_W)
#define WC_CCS_BIG5		WC_CCS_SET_PCSW(WC_F_BIG5)
#define WC_CCS_BIG5_1		WC_CCS_SET_PCSW(WC_F_BIG5_1)
#define WC_CCS_BIG5_2		WC_CCS_SET_PCSW(WC_F_BIG5_2)
#define WC_CCS_CNS_11643_8	WC_CCS_SET_PCSW(WC_F_CNS_11643_8)
#define WC_CCS_CNS_11643_9	WC_CCS_SET_PCSW(WC_F_CNS_11643_9)
#define WC_CCS_CNS_11643_10	WC_CCS_SET_PCSW(WC_F_CNS_11643_10)
#define WC_CCS_CNS_11643_11	WC_CCS_SET_PCSW(WC_F_CNS_11643_11)
#define WC_CCS_CNS_11643_12	WC_CCS_SET_PCSW(WC_F_CNS_11643_12)
#define WC_CCS_CNS_11643_13	WC_CCS_SET_PCSW(WC_F_CNS_11643_13)
#define WC_CCS_CNS_11643_14	WC_CCS_SET_PCSW(WC_F_CNS_11643_14)
#define WC_CCS_CNS_11643_15	WC_CCS_SET_PCSW(WC_F_CNS_11643_15)
#define WC_CCS_CNS_11643_16	WC_CCS_SET_PCSW(WC_F_CNS_11643_16)
#define WC_CCS_CNS_11643_X	WC_CCS_SET_PCSW(WC_F_CNS_11643_X)
#define WC_CCS_GB_12345		WC_CCS_SET_PCSW(WC_F_GB_12345)
#define WC_CCS_SJIS_EXT		WC_CCS_SET_PCSW(WC_F_SJIS_EXT)
#define WC_CCS_SJIS_EXT_1	WC_CCS_SET_PCSW(WC_F_SJIS_EXT_1)
#define WC_CCS_SJIS_EXT_2	WC_CCS_SET_PCSW(WC_F_SJIS_EXT_2)
#define WC_CCS_GBK		WC_CCS_SET_PCSW(WC_F_GBK)
#define WC_CCS_GBK_1		WC_CCS_SET_PCSW(WC_F_GBK_1)
#define WC_CCS_GBK_2		WC_CCS_SET_PCSW(WC_F_GBK_2)
#define WC_CCS_GBK_EXT		WC_CCS_SET_PCSW(WC_F_GBK_EXT)
#define WC_CCS_GBK_EXT_1	WC_CCS_SET_PCSW(WC_F_GBK_EXT_1)
#define WC_CCS_GBK_EXT_2	WC_CCS_SET_PCSW(WC_F_GBK_EXT_2)
#define WC_CCS_UHC		WC_CCS_SET_PCSW(WC_F_UHC)
#define WC_CCS_UHC_1		WC_CCS_SET_PCSW(WC_F_UHC_1)
#define WC_CCS_UHC_2		WC_CCS_SET_PCSW(WC_F_UHC_2)
#define WC_CCS_HKSCS		WC_CCS_SET_PCSW(WC_F_HKSCS)
#define WC_CCS_HKSCS_1		WC_CCS_SET_PCSW(WC_F_HKSCS_1)
#define WC_CCS_HKSCS_2		WC_CCS_SET_PCSW(WC_F_HKSCS_2)

#define WC_CCS_UCS2		WC_CCS_SET_WCS16(WC_F_UCS2)
#define WC_CCS_UCS4		WC_CCS_SET_WCS32(WC_F_UCS4)
#define WC_CCS_UCS_TAG		WC_CCS_SET_WCS32(WC_F_UCS_TAG)
#define WC_CCS_GB18030		WC_CCS_SET_WCS32(WC_F_GB18030)
#define WC_CCS_UCS2_W		WC_CCS_SET_WCS16W(WC_F_UCS2)
#define WC_CCS_UCS4_W		WC_CCS_SET_WCS32W(WC_F_UCS4)
#define WC_CCS_UCS_TAG_W	WC_CCS_SET_WCS32W(WC_F_UCS_TAG)
#define WC_CCS_GB18030_W	WC_CCS_SET_WCS32W(WC_F_GB18030)

#define WC_CCS_UNKNOWN		WC_CCS_A_UNKNOWN
#define WC_CCS_C1		(WC_CCS_A_UNKNOWN|WC_F_C1)
#define WC_CCS_UNKNOWN_W	WC_CCS_A_UNKNOWN_W

#define WC_CCS_IS_UNICODE(c)	(WC_CCS_SET(c) == WC_CCS_UCS2 || WC_CCS_SET(c) == WC_CCS_UCS4 || WC_CCS_SET(c) == WC_CCS_UCS_TAG)

extern wc_bool wc_is_combining(wc_wchar_t cc);

#endif
