/* $Id$ */
#include "hash.h"
static HashItem_si MyHashItem[] = {
	 /* 0 */ {"otimes", 0x2297, &MyHashItem[1]},
	 /* 1 */ {"laquo", 0xAB, NULL},
	 /* 2 */ {"cap", 0x2229, &MyHashItem[3]},
	 /* 3 */ {"dArr", 0x21D3, &MyHashItem[4]},
	 /* 4 */ {"euml", 0xEB, NULL},
	 /* 5 */ {"sum", 0x2211, &MyHashItem[6]},
	 /* 6 */ {"Ocirc", 0xD4, NULL},
	 /* 7 */ {"dagger", 0x2020, &MyHashItem[8]},
	 /* 8 */ {"Scaron", 0x0160, &MyHashItem[9]},
	 /* 9 */ {"Omicron", 0x039F, &MyHashItem[10]},
	 /* 10 */ {"brvbar", 0xA6, NULL},
	 /* 11 */ {"Eta", 0x0397, &MyHashItem[12]},
	 /* 12 */ {"iacute", 0xED, &MyHashItem[13]},
	 /* 13 */ {"aelig", 0xE6, &MyHashItem[14]},
	 /* 14 */ {"Ugrave", 0xD9, &MyHashItem[15]},
	 /* 15 */ {"deg", 0xB0, NULL},
	 /* 16 */ {"Yuml", 0x0178, &MyHashItem[17]},
	 /* 17 */ {"sup", 0x2283, &MyHashItem[18]},
	 /* 18 */ {"middot", 0xB7, NULL},
	 /* 19 */ {"ge", 0x2265, &MyHashItem[20]},
	 /* 20 */ {"alefsym", 0x2135, &MyHashItem[21]},
	 /* 21 */ {"sigma", 0x03C3, &MyHashItem[22]},
	 /* 22 */ {"aring", 0xE5, &MyHashItem[23]},
	 /* 23 */ {"Icirc", 0xCE, NULL},
	 /* 24 */ {"and", 0x2227, &MyHashItem[25]},
	 /* 25 */ {"weierp", 0x2118, &MyHashItem[26]},
	 /* 26 */ {"frac12", 0xBD, NULL},
	 /* 27 */ {"radic", 0x221A, &MyHashItem[28]},
	 /* 28 */ {"chi", 0x03C7, &MyHashItem[29]},
	 /* 29 */ {"zeta", 0x03B6, &MyHashItem[30]},
	 /* 30 */ {"Theta", 0x0398, &MyHashItem[31]},
	 /* 31 */ {"Atilde", 0xC3, &MyHashItem[32]},
	 /* 32 */ {"para", 0xB6, NULL},
	 /* 33 */ {"frac14", 0xBC, &MyHashItem[34]},
	 /* 34 */ {"cedil", 0xB8, &MyHashItem[35]},
	 /* 35 */ {"quot", 0x22, NULL},
	 /* 36 */ {"ang", 0x2220, &MyHashItem[37]},
	 /* 37 */ {"ucirc", 0xFB, NULL},
	 /* 38 */ {"supe", 0x2287, &MyHashItem[39]},
	 /* 39 */ {"iota", 0x03B9, &MyHashItem[40]},
	 /* 40 */ {"Ograve", 0xD2, NULL},
	 /* 41 */ {"rArr", 0x21D2, &MyHashItem[42]},
	 /* 42 */ {"Auml", 0xC4, &MyHashItem[43]},
	 /* 43 */ {"frac34", 0xBE, &MyHashItem[44]},
	 /* 44 */ {"nbsp", 0xA0, NULL},
	 /* 45 */ {"euro", 0x20AC, &MyHashItem[46]},
	 /* 46 */ {"ocirc", 0xF4, NULL},
	 /* 47 */ {"equiv", 0x2261, &MyHashItem[48]},
	 /* 48 */ {"upsilon", 0x03C5, &MyHashItem[49]},
	 /* 49 */ {"sigmaf", 0x03C2, &MyHashItem[50]},
	 /* 50 */ {"ETH", 0xD0, NULL},
	 /* 51 */ {"le", 0x2264, &MyHashItem[52]},
	 /* 52 */ {"beta", 0x03B2, &MyHashItem[53]},
	 /* 53 */ {"yacute", 0xFD, &MyHashItem[54]},
	 /* 54 */ {"egrave", 0xE8, NULL},
	 /* 55 */ {"lowast", 0x2217, &MyHashItem[56]},
	 /* 56 */ {"real", 0x211C, &MyHashItem[57]},
	 /* 57 */ {"amp", 0x26, NULL},
	 /* 58 */ {"icirc", 0xEE, &MyHashItem[59]},
	 /* 59 */ {"micro", 0xB5, NULL},
	 /* 60 */ {"isin", 0x2208, &MyHashItem[61]},
	 /* 61 */ {"curren", 0xA4, NULL},
	 /* 62 */ {"rdquo", 0x201D, &MyHashItem[63]},
	 /* 63 */ {"sbquo", 0x201A, &MyHashItem[64]},
	 /* 64 */ {"ne", 0x2260, &MyHashItem[65]},
	 /* 65 */ {"theta", 0x03B8, &MyHashItem[66]},
	 /* 66 */ {"Igrave", 0xCC, NULL},
	 /* 67 */ {"gt", 0x3E, NULL},
	 /* 68 */ {"hearts", 0x2665, &MyHashItem[69]},
	 /* 69 */ {"rang", 0x232A, NULL},
	 /* 70 */ {"rfloor", 0x230B, NULL},
	 /* 71 */ {"ldquo", 0x201C, &MyHashItem[72]},
	 /* 72 */ {"ni", 0x220B, &MyHashItem[73]},
	 /* 73 */ {"Ntilde", 0xD1, &MyHashItem[74]},
	 /* 74 */ {"Aacute", 0xC1, NULL},
	 /* 75 */ {"crarr", 0x21B5, &MyHashItem[76]},
	 /* 76 */ {"Ouml", 0xD6, &MyHashItem[77]},
	 /* 77 */ {"GT", 0x3E, NULL},
	 /* 78 */ {"clubs", 0x2663, NULL},
	 /* 79 */ {"scaron", 0x0161, &MyHashItem[80]},
	 /* 80 */ {"part", 0x2202, NULL},
	 /* 81 */ {"tilde", 0x02DC, &MyHashItem[82]},
	 /* 82 */ {"oelig", 0x0153, &MyHashItem[83]},
	 /* 83 */ {"pi", 0x03C0, &MyHashItem[84]},
	 /* 84 */ {"ugrave", 0xF9, NULL},
	 /* 85 */ {"darr", 0x2193, &MyHashItem[86]},
	 /* 86 */ {"uuml", 0xFC, &MyHashItem[87]},
	 /* 87 */ {"QUOT", 0x22, NULL},
	 /* 88 */ {"Prime", 0x2033, NULL},
	 /* 89 */ {"zwj", 0x200D, &MyHashItem[90]},
	 /* 90 */ {"lfloor", 0x230A, &MyHashItem[91]},
	 /* 91 */ {"notin", 0x2209, &MyHashItem[92]},
	 /* 92 */ {"cent", 0xA2, &MyHashItem[93]},
	 /* 93 */ {"lt", 0x3C, NULL},
	 /* 94 */ {"eta", 0x03B7, &MyHashItem[95]},
	 /* 95 */ {"Phi", 0x03A6, &MyHashItem[96]},
	 /* 96 */ {"atilde", 0xE3, NULL},
	 /* 97 */ {"hArr", 0x21D4, &MyHashItem[98]},
	 /* 98 */ {"iuml", 0xEF, &MyHashItem[99]},
	 /* 99 */ {"NBSP", 0xA0, NULL},
	 /* 100 */ {"mu", 0x03BC, NULL},
	 /* 101 */ {"or", 0x2228, &MyHashItem[102]},
	 /* 102 */ {"plusmn", 0xB1, &MyHashItem[103]},
	 /* 103 */ {"LT", 0x3C, NULL},
	 /* 104 */ {"nu", 0x03BD, &MyHashItem[105]},
	 /* 105 */ {"ograve", 0xF2, &MyHashItem[106]},
	 /* 106 */ {"AElig", 0xC6, NULL},
	 /* 107 */ {"rceil", 0x2309, &MyHashItem[108]},
	 /* 108 */ {"uArr", 0x21D1, &MyHashItem[109]},
	 /* 109 */ {"sect", 0xA7, NULL},
	 /* 110 */ {"circ", 0x02C6, NULL},
	 /* 111 */ {"perp", 0x22A5, &MyHashItem[112]},
	 /* 112 */ {"eth", 0xF0, NULL},
	 /* 113 */ {"rsquo", 0x2019, &MyHashItem[114]},
	 /* 114 */ {"nabla", 0x2207, NULL},
	 /* 115 */ {"lceil", 0x2308, &MyHashItem[116]},
	 /* 116 */ {"cup", 0x222A, &MyHashItem[117]},
	 /* 117 */ {"exist", 0x2203, &MyHashItem[118]},
	 /* 118 */ {"rarr", 0x2192, &MyHashItem[119]},
	 /* 119 */ {"upsih", 0x03D2, NULL},
	 /* 120 */ {"prime", 0x2032, &MyHashItem[121]},
	 /* 121 */ {"Omega", 0x03A9, &MyHashItem[122]},
	 /* 122 */ {"Ecirc", 0xCA, NULL},
	 /* 123 */ {"Epsilon", 0x0395, NULL},
	 /* 124 */ {"lsquo", 0x2018, &MyHashItem[125]},
	 /* 125 */ {"xi", 0x03BE, &MyHashItem[126]},
	 /* 126 */ {"Lambda", 0x039B, &MyHashItem[127]},
	 /* 127 */ {"Kappa", 0x039A, &MyHashItem[128]},
	 /* 128 */ {"divide", 0xF7, &MyHashItem[129]},
	 /* 129 */ {"igrave", 0xEC, &MyHashItem[130]},
	 /* 130 */ {"acute", 0xB4, NULL},
	 /* 131 */ {"Euml", 0xCB, &MyHashItem[132]},
	 /* 132 */ {"ordf", 0xAA, NULL},
	 /* 133 */ {"image", 0x2111, &MyHashItem[134]},
	 /* 134 */ {"Tau", 0x03A4, &MyHashItem[135]},
	 /* 135 */ {"Rho", 0x03A1, NULL},
	 /* 136 */ {"ntilde", 0xF1, &MyHashItem[137]},
	 /* 137 */ {"aacute", 0xE1, &MyHashItem[138]},
	 /* 138 */ {"times", 0xD7, NULL},
	 /* 139 */ {"omicron", 0x03BF, NULL},
	 /* 140 */ {"oplus", 0x2295, &MyHashItem[141]},
	 /* 141 */ {"Zeta", 0x0396, &MyHashItem[142]},
	 /* 142 */ {"Eacute", 0xC9, &MyHashItem[143]},
	 /* 143 */ {"ordm", 0xBA, NULL},
	 /* 144 */ {"Oslash", 0xD8, &MyHashItem[145]},
	 /* 145 */ {"Ccedil", 0xC7, &MyHashItem[146]},
	 /* 146 */ {"iquest", 0xBF, NULL},
	 /* 147 */ {"omega", 0x03C9, &MyHashItem[148]},
	 /* 148 */ {"Psi", 0x03A8, &MyHashItem[149]},
	 /* 149 */ {"ecirc", 0xEA, NULL},
	 /* 150 */ {"int", 0x222B, NULL},
	 /* 151 */ {"trade", 0x2122, &MyHashItem[152]},
	 /* 152 */ {"kappa", 0x03BA, &MyHashItem[153]},
	 /* 153 */ {"Iota", 0x0399, &MyHashItem[154]},
	 /* 154 */ {"Delta", 0x0394, &MyHashItem[155]},
	 /* 155 */ {"Alpha", 0x0391, &MyHashItem[156]},
	 /* 156 */ {"Otilde", 0xD5, NULL},
	 /* 157 */ {"sdot", 0x22C5, NULL},
	 /* 158 */ {"cong", 0x2245, NULL},
	 /* 159 */ {"rsaquo", 0x203A, &MyHashItem[160]},
	 /* 160 */ {"OElig", 0x0152, &MyHashItem[161]},
	 /* 161 */ {"diams", 0x2666, &MyHashItem[162]},
	 /* 162 */ {"phi", 0x03C6, &MyHashItem[163]},
	 /* 163 */ {"Beta", 0x0392, &MyHashItem[164]},
	 /* 164 */ {"szlig", 0xDF, &MyHashItem[165]},
	 /* 165 */ {"sup1", 0xB9, &MyHashItem[166]},
	 /* 166 */ {"reg", 0xAE, NULL},
	 /* 167 */ {"harr", 0x2194, &MyHashItem[168]},
	 /* 168 */ {"hellip", 0x2026, &MyHashItem[169]},
	 /* 169 */ {"yuml", 0xFF, &MyHashItem[170]},
	 /* 170 */ {"sup2", 0xB2, NULL},
	 /* 171 */ {"Gamma", 0x0393, &MyHashItem[172]},
	 /* 172 */ {"sup3", 0xB3, NULL},
	 /* 173 */ {"forall", 0x2200, NULL},
	 /* 174 */ {"bdquo", 0x201E, &MyHashItem[175]},
	 /* 175 */ {"spades", 0x2660, &MyHashItem[176]},
	 /* 176 */ {"Pi", 0x03A0, &MyHashItem[177]},
	 /* 177 */ {"Uacute", 0xDA, &MyHashItem[178]},
	 /* 178 */ {"Agrave", 0xC0, NULL},
	 /* 179 */ {"permil", 0x2030, &MyHashItem[180]},
	 /* 180 */ {"mdash", 0x2014, &MyHashItem[181]},
	 /* 181 */ {"lArr", 0x21D0, &MyHashItem[182]},
	 /* 182 */ {"uarr", 0x2191, NULL},
	 /* 183 */ {"Upsilon", 0x03A5, &MyHashItem[184]},
	 /* 184 */ {"pound", 0xA3, NULL},
	 /* 185 */ {"lsaquo", 0x2039, &MyHashItem[186]},
	 /* 186 */ {"lrm", 0x200E, &MyHashItem[187]},
	 /* 187 */ {"lambda", 0x03BB, &MyHashItem[188]},
	 /* 188 */ {"delta", 0x03B4, &MyHashItem[189]},
	 /* 189 */ {"alpha", 0x03B1, NULL},
	 /* 190 */ {"frasl", 0x2044, &MyHashItem[191]},
	 /* 191 */ {"thorn", 0xFE, &MyHashItem[192]},
	 /* 192 */ {"auml", 0xE4, NULL},
	 /* 193 */ {"Mu", 0x039C, NULL},
	 /* 194 */ {"nsub", 0x2284, &MyHashItem[195]},
	 /* 195 */ {"macr", 0xAF, NULL},
	 /* 196 */ {"minus", 0x2212, &MyHashItem[197]},
	 /* 197 */ {"Nu", 0x039D, &MyHashItem[198]},
	 /* 198 */ {"Oacute", 0xD3, NULL},
	 /* 199 */ {"prod", 0x220F, &MyHashItem[200]},
	 /* 200 */ {"Uuml", 0xDC, &MyHashItem[201]},
	 /* 201 */ {"iexcl", 0xA1, NULL},
	 /* 202 */ {"lang", 0x2329, &MyHashItem[203]},
	 /* 203 */ {"tau", 0x03C4, &MyHashItem[204]},
	 /* 204 */ {"rho", 0x03C1, &MyHashItem[205]},
	 /* 205 */ {"gamma", 0x03B3, NULL},
	 /* 206 */ {"loz", 0x25CA, &MyHashItem[207]},
	 /* 207 */ {"bull", 0x2022, &MyHashItem[208]},
	 /* 208 */ {"piv", 0x03D6, NULL},
	 /* 209 */ {"eacute", 0xE9, NULL},
	 /* 210 */ {"zwnj", 0x200C, &MyHashItem[211]},
	 /* 211 */ {"oslash", 0xF8, &MyHashItem[212]},
	 /* 212 */ {"ccedil", 0xE7, &MyHashItem[213]},
	 /* 213 */ {"THORN", 0xDE, &MyHashItem[214]},
	 /* 214 */ {"Iuml", 0xCF, &MyHashItem[215]},
	 /* 215 */ {"not", 0xAC, NULL},
	 /* 216 */ {"sim", 0x223C, &MyHashItem[217]},
	 /* 217 */ {"thetasym", 0x03D1, &MyHashItem[218]},
	 /* 218 */ {"Acirc", 0xC2, NULL},
	 /* 219 */ {"Dagger", 0x2021, &MyHashItem[220]},
	 /* 220 */ {"fnof", 0x0192, NULL},
	 /* 221 */ {"rlm", 0x200F, &MyHashItem[222]},
	 /* 222 */ {"oline", 0x203E, &MyHashItem[223]},
	 /* 223 */ {"Chi", 0x03A7, &MyHashItem[224]},
	 /* 224 */ {"Xi", 0x039E, &MyHashItem[225]},
	 /* 225 */ {"otilde", 0xF5, &MyHashItem[226]},
	 /* 226 */ {"Iacute", 0xCD, &MyHashItem[227]},
	 /* 227 */ {"copy", 0xA9, NULL},
	 /* 228 */ {"ndash", 0x2013, &MyHashItem[229]},
	 /* 229 */ {"ouml", 0xF6, NULL},
	 /* 230 */ {"psi", 0x03C8, NULL},
	 /* 231 */ {"sube", 0x2286, NULL},
	 /* 232 */ {"emsp", 0x2003, &MyHashItem[233]},
	 /* 233 */ {"asymp", 0x2248, &MyHashItem[234]},
	 /* 234 */ {"prop", 0x221D, NULL},
	 /* 235 */ {"infin", 0x221E, NULL},
	 /* 236 */ {"empty", 0x2205, &MyHashItem[237]},
	 /* 237 */ {"uacute", 0xFA, &MyHashItem[238]},
	 /* 238 */ {"agrave", 0xE0, &MyHashItem[239]},
	 /* 239 */ {"shy", 0xAD, NULL},
	 /* 240 */ {"ensp", 0x2002, NULL},
	 /* 241 */ {"acirc", 0xE2, NULL},
	 /* 242 */ {"sub", 0x2282, &MyHashItem[243]},
	 /* 243 */ {"epsilon", 0x03B5, NULL},
	 /* 244 */ {"Yacute", 0xDD, &MyHashItem[245]},
	 /* 245 */ {"Egrave", 0xC8, NULL},
	 /* 246 */ {"there4", 0x2234, &MyHashItem[247]},
	 /* 247 */ {"larr", 0x2190, &MyHashItem[248]},
	 /* 248 */ {"uml", 0xA8, &MyHashItem[249]},
	 /* 249 */ {"AMP", 0x26, NULL},
	 /* 250 */ {"Sigma", 0x03A3, &MyHashItem[251]},
	 /* 251 */ {"Aring", 0xC5, NULL},
	 /* 252 */ {"yen", 0xA5, NULL},
	 /* 253 */ {"oacute", 0xF3, &MyHashItem[254]},
	 /* 254 */ {"raquo", 0xBB, NULL},
	 /* 255 */ {"thinsp", 0x2009, NULL},
	 /* 256 */ {"Ucirc", 0xDB, NULL},
};

static HashItem_si *MyHashItemTbl[] = {
	NULL,
	&MyHashItem[0],
	&MyHashItem[2],
	&MyHashItem[5],
	&MyHashItem[7],
	&MyHashItem[11],
	&MyHashItem[16],
	&MyHashItem[19],
	&MyHashItem[24],
	&MyHashItem[27],
	&MyHashItem[33],
	&MyHashItem[36],
	NULL,
	&MyHashItem[38],
	&MyHashItem[41],
	&MyHashItem[45],
	&MyHashItem[47],
	&MyHashItem[51],
	&MyHashItem[55],
	&MyHashItem[58],
	&MyHashItem[60],
	&MyHashItem[62],
	&MyHashItem[67],
	&MyHashItem[68],
	&MyHashItem[70],
	&MyHashItem[71],
	&MyHashItem[75],
	&MyHashItem[78],
	&MyHashItem[79],
	&MyHashItem[81],
	&MyHashItem[85],
	&MyHashItem[88],
	&MyHashItem[89],
	&MyHashItem[94],
	&MyHashItem[97],
	&MyHashItem[100],
	&MyHashItem[101],
	&MyHashItem[104],
	&MyHashItem[107],
	&MyHashItem[110],
	&MyHashItem[111],
	&MyHashItem[113],
	&MyHashItem[115],
	&MyHashItem[120],
	&MyHashItem[123],
	&MyHashItem[124],
	&MyHashItem[131],
	&MyHashItem[133],
	NULL,
	&MyHashItem[136],
	NULL,
	NULL,
	&MyHashItem[139],
	&MyHashItem[140],
	&MyHashItem[144],
	&MyHashItem[147],
	&MyHashItem[150],
	&MyHashItem[151],
	&MyHashItem[157],
	&MyHashItem[158],
	NULL,
	&MyHashItem[159],
	&MyHashItem[167],
	&MyHashItem[171],
	&MyHashItem[173],
	&MyHashItem[174],
	&MyHashItem[179],
	NULL,
	&MyHashItem[183],
	&MyHashItem[185],
	&MyHashItem[190],
	&MyHashItem[193],
	&MyHashItem[194],
	&MyHashItem[196],
	&MyHashItem[199],
	&MyHashItem[202],
	&MyHashItem[206],
	&MyHashItem[209],
	&MyHashItem[210],
	&MyHashItem[216],
	&MyHashItem[219],
	&MyHashItem[221],
	&MyHashItem[228],
	&MyHashItem[230],
	NULL,
	&MyHashItem[231],
	&MyHashItem[232],
	NULL,
	&MyHashItem[235],
	&MyHashItem[236],
	&MyHashItem[240],
	&MyHashItem[241],
	&MyHashItem[242],
	&MyHashItem[244],
	&MyHashItem[246],
	&MyHashItem[250],
	&MyHashItem[252],
	&MyHashItem[253],
	&MyHashItem[255],
	&MyHashItem[256],
};

Hash_si entity = {100, MyHashItemTbl};
