/* $Id: myctype.h,v 1.6 2003/09/22 21:02:20 ukai Exp $ */
#ifndef _MYCTYPE_H
#define _MYCTYPE_H

#define MYCTYPE_CNTRL 1
#define MYCTYPE_SPACE 2
#define MYCTYPE_ALPHA 4
#define MYCTYPE_DIGIT 8
#define MYCTYPE_PRINT 16
#define MYCTYPE_HEX   32
#define MYCTYPE_INTSPACE 64
#define MYCTYPE_ASCII (MYCTYPE_CNTRL|MYCTYPE_PRINT)
#define MYCTYPE_ALNUM (MYCTYPE_ALPHA|MYCTYPE_DIGIT)
#define MYCTYPE_XDIGIT (MYCTYPE_HEX|MYCTYPE_DIGIT)

#define GET_MYCTYPE(x) (MYCTYPE_MAP[(int)(unsigned char)(x)])

#define IS_INTSPACE(x) (MYCTYPE_MAP[(unsigned char)(x)] & MYCTYPE_INTSPACE)

extern unsigned char MYCTYPE_MAP[];

#endif
