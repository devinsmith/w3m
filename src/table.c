/* $Id: table.c,v 1.58 2010/08/09 11:59:19 htrb Exp $ */
/*
 * HTML table
 */
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "fm.h"
#include "html.h"
#include "parsetagx.h"
#include "Str.h"

static int visible_length_offset = 0;
int symbol_width = 0;
int symbol_width0 = 0;

#define RULE_WIDTH symbol_width
#define RULE(mode,n) (((mode) == BORDER_THICK) ? ((n) + 16) : (n))
#define TK_VERTICALBAR(mode) RULE(mode,5)

#define BORDERWIDTH     2
#define BORDERHEIGHT    1
#define NOBORDERWIDTH   1
#define NOBORDERHEIGHT  0

#define HTT_X   1
#define HTT_Y   2
#define HTT_ALIGN  0x30
#define HTT_LEFT   0x00
#define HTT_CENTER 0x10
#define HTT_RIGHT  0x20
#define HTT_TRSET  0x40
#define HTT_VALIGN 0x700
#define HTT_TOP    0x100
#define HTT_MIDDLE 0x200
#define HTT_BOTTOM 0x400
#define HTT_VTRSET 0x800
#ifdef NOWRAP
#define HTT_NOWRAP  4
#endif				/* NOWRAP */
#define TAG_IS(s,tag,len) (strncasecmp(s,tag,len)==0&&(s[len] == '>' || isspace((unsigned char)s[len])))

#ifndef max
#define max(a,b)        ((a) > (b) ? (a) : (b))
#endif				/* not max */
#ifndef min
#define min(a,b)        ((a) > (b) ? (b) : (a))
#endif				/* not min */
#ifndef abs
#define abs(a)          ((a) >= 0. ? (a) : -(a))
#endif				/* not abs */

#define set_prevchar(x,y,n) Strcopy_charp_n((x),(y),(n))
#define set_space_to_prevchar(x) Strcopy_charp_n((x)," ",1)

#define LOG_MIN 1.0
static double
weight3(int x)
{
	if (x < 0.1)
		return 0.1;
	if (x < LOG_MIN)
		return (double) x;
	else
		return LOG_MIN * (log((double) x / LOG_MIN) + 1.);
}

static int
bsearch_2short(short e1, short *ent1, short e2, short *ent2, int base,
	       short *indexarray, int nent)
{
	int n = nent;
	int k = 0;

	int e = e1 * base + e2;
	while (n > 0) {
		int nn = n / 2;
		int idx = indexarray[k + nn];
		int ne = ent1[idx] * base + ent2[idx];
		if (ne == e) {
			k += nn;
			break;
		} else if (ne < e) {
			n -= nn + 1;
			k += nn + 1;
		} else {
			n = nn;
		}
	}
	return k;
}

static int
bsearch_double(double e, double *ent, short *indexarray, int nent)
{
	int n = nent;
	int k = 0;

	while (n > 0) {
		int nn = n / 2;
		int idx = indexarray[k + nn];
		double ne = ent[idx];
		if (ne == e) {
			k += nn;
			break;
		} else if (ne > e) {
			n -= nn + 1;
			k += nn + 1;
		} else {
			n = nn;
		}
	}
	return k;
}

static int
ceil_at_intervals(int x, int step)
{
	int mo = x % step;
	if (mo > 0)
		x += step - mo;
	else if (mo < 0)
		x -= mo;
	return x;
}

static int
floor_at_intervals(int x, int step)
{
	int mo = x % step;
	if (mo > 0)
		x -= mo;
	else if (mo < 0)
		x += step - mo;
	return x;
}

#define round(x) ((int)floor((x)+0.5))

static void
dv2sv(double *dv, short *iv, int size)
{
	int i, k, iw;
	short *indexarray;
	double *edv;
	double w = 0., x;

	indexarray = NewAtom_N(short, size);
	edv = NewAtom_N(double, size);
	for (i = 0; i < size; i++) {
		iv[i] = ceil(dv[i]);
		edv[i] = (double) iv[i] - dv[i];
	}

	w = 0.;
	for (k = 0; k < size; k++) {
		x = edv[k];
		w += x;
		i = bsearch_double(x, edv, indexarray, k);
		if (k > i) {
			int ii;
			for (ii = k; ii > i; ii--)
				indexarray[ii] = indexarray[ii - 1];
		}
		indexarray[i] = k;
	}
	iw = min((int) (w + 0.5), size);
	if (iw == 0)
		return;
	x = edv[(int) indexarray[iw - 1]];
	for (i = 0; i < size; i++) {
		k = indexarray[i];
		if (i >= iw && abs(edv[k] - x) > 1e-6)
			break;
		iv[k]--;
	}
}

static int
table_colspan(struct table * t, int row, int col)
{
	int i;
	for (i = col + 1; i <= t->maxcol && (t->tabattr[row][i] & HTT_X); i++);
	return i - col;
}

static int
table_rowspan(struct table * t, int row, int col)
{
	int i;
	if (!t->tabattr[row])
		return 0;
	for (i = row + 1; i <= t->maxrow && t->tabattr[i] &&
	     (t->tabattr[i][col] & HTT_Y); i++);
	return i - row;
}

static int
minimum_cellspacing(int border_mode)
{
	switch (border_mode) {
		case BORDER_THIN:
		case BORDER_THICK:
		case BORDER_NOWIN:
		return RULE_WIDTH;
	case BORDER_NONE:
		return 1;
	default:
		/* not reached */
		return 0;
	}
}

static int
table_border_width(struct table * t)
{
	switch (t->border_mode) {
		case BORDER_THIN:
		case BORDER_THICK:
		return t->maxcol * t->cellspacing + 2 * (RULE_WIDTH + t->cellpadding);
	case BORDER_NOWIN:
	case BORDER_NONE:
		return t->maxcol * t->cellspacing;
	default:
		/* not reached */
		return 0;
	}
}

struct table *
newTable()
{
	struct table *t;
	int i, j;

	t = New(struct table);
	t->max_rowsize = MAXROW;
	t->tabdata = New_N(GeneralList **, MAXROW);
	t->tabattr = New_N(table_attr *, MAXROW);
	t->tabheight = NewAtom_N(short, MAXROW);
#ifdef ID_EXT
	t->tabidvalue = New_N(Str *, MAXROW);
	t->tridvalue = New_N(Str, MAXROW);
#endif				/* ID_EXT */

	for (i = 0; i < MAXROW; i++) {
		t->tabdata[i] = NULL;
		t->tabattr[i] = 0;
		t->tabheight[i] = 0;
#ifdef ID_EXT
		t->tabidvalue[i] = NULL;
		t->tridvalue[i] = NULL;
#endif				/* ID_EXT */
	}
	for (j = 0; j < MAXCOL; j++) {
		t->tabwidth[j] = 0;
		t->minimum_width[j] = 0;
		t->fixed_width[j] = 0;
	}
	t->cell.maxcell = -1;
	t->cell.icell = -1;
	t->ntable = 0;
	t->tables_size = 0;
	t->tables = NULL;
#if 0
	t->tabcontentssize = 0;
	t->indent = 0;
	t->linfo.prev_ctype = PC_ASCII;
	t->linfo.prev_spaces = -1;
#endif
	t->linfo.prevchar = Strnew_size(8);
	set_prevchar(t->linfo.prevchar, "", 0);
	t->trattr = 0;

	t->caption = Strnew();
	t->suspended_data = NULL;
#ifdef ID_EXT
	t->id = NULL;
#endif
	return t;
}

static void
check_row(struct table * t, int row)
{
	int i, r;
	GeneralList ***tabdata;
	table_attr **tabattr;
	short *tabheight;
#ifdef ID_EXT
	Str **tabidvalue;
	Str *tridvalue;
#endif				/* ID_EXT */

	if (row >= t->max_rowsize) {
		r = max(t->max_rowsize * 2, row + 1);
		tabdata = New_N(GeneralList **, r);
		tabattr = New_N(table_attr *, r);
		tabheight = NewAtom_N(short, r);
#ifdef ID_EXT
		tabidvalue = New_N(Str *, r);
		tridvalue = New_N(Str, r);
#endif				/* ID_EXT */
		for (i = 0; i < t->max_rowsize; i++) {
			tabdata[i] = t->tabdata[i];
			tabattr[i] = t->tabattr[i];
			tabheight[i] = t->tabheight[i];
#ifdef ID_EXT
			tabidvalue[i] = t->tabidvalue[i];
			tridvalue[i] = t->tridvalue[i];
#endif				/* ID_EXT */
		}
		for (; i < r; i++) {
			tabdata[i] = NULL;
			tabattr[i] = NULL;
			tabheight[i] = 0;
#ifdef ID_EXT
			tabidvalue[i] = NULL;
			tridvalue[i] = NULL;
#endif				/* ID_EXT */
		}
		t->tabdata = tabdata;
		t->tabattr = tabattr;
		t->tabheight = tabheight;
#ifdef ID_EXT
		t->tabidvalue = tabidvalue;
		t->tridvalue = tridvalue;
#endif				/* ID_EXT */
		t->max_rowsize = r;
	}
	if (t->tabdata[row] == NULL) {
		t->tabdata[row] = New_N(GeneralList *, MAXCOL);
		t->tabattr[row] = NewAtom_N(table_attr, MAXCOL);
#ifdef ID_EXT
		t->tabidvalue[row] = New_N(Str, MAXCOL);
#endif				/* ID_EXT */
		for (i = 0; i < MAXCOL; i++) {
			t->tabdata[row][i] = NULL;
			t->tabattr[row][i] = 0;
#ifdef ID_EXT
			t->tabidvalue[row][i] = NULL;
#endif				/* ID_EXT */
		}
	}
}

void
pushdata(struct table * t, int row, int col, char *data)
{
	check_row(t, row);
	if (t->tabdata[row][col] == NULL)
		t->tabdata[row][col] = newGeneralList();

	pushText(t->tabdata[row][col], data ? data : "");
}

void
suspend_or_pushdata(struct table * tbl, char *line)
{
	if (tbl->flag & TBL_IN_COL)
		pushdata(tbl, tbl->row, tbl->col, line);
	else {
		if (!tbl->suspended_data)
			tbl->suspended_data = newTextList();
		pushText(tbl->suspended_data, line ? line : "");
	}
}

#ifdef USE_M17N
#define PUSH_TAG(str,n) Strcat_charp_n(tagbuf, str, n)
#else
#define PUSH_TAG(str,n) Strcat_char(tagbuf, *str)
#endif

int
visible_length(char *str)
{
	int len = 0, n, max_len = 0;
	enum RTstatus status = R_ST_NORMAL;
	int prev_status = status;
	Str tagbuf = Strnew();
	char *t, *r2;
	int amp_len = 0;

	t = str;
	while (*str) {
		prev_status = status;
		if (next_status(*str, &status)) {
#ifdef USE_M17N
			len += get_mcwidth(str);
			n = get_mclen(str);
		} else {
			n = 1;
		}
#else
			len++;
		}
#endif
		if (status == R_ST_TAG0) {
			Strclear(tagbuf);
			PUSH_TAG(str, n);
		} else if (status == R_ST_TAG || status == R_ST_DQUOTE
			   || status == R_ST_QUOTE || status == R_ST_EQL
			   || status == R_ST_VALUE) {
			PUSH_TAG(str, n);
		} else if (status == R_ST_AMP) {
			if (prev_status == R_ST_NORMAL) {
				Strclear(tagbuf);
				len--;
				amp_len = 0;
			} else {
				PUSH_TAG(str, n);
				amp_len++;
			}
		} else if (status == R_ST_NORMAL && prev_status == R_ST_AMP) {
			PUSH_TAG(str, n);
			r2 = tagbuf->ptr;
			t = getescapecmd(&r2);
			if (!*r2 && (*t == '\r' || *t == '\n')) {
				if (len > max_len)
					max_len = len;
				len = 0;
			} else
				len += get_strwidth(t) + get_strwidth(r2);
		} else if (status == R_ST_NORMAL && ST_IS_REAL_TAG(prev_status)) {
			;
		} else if (*str == '\t') {
			len--;
			do {
				len++;
			} while ((visible_length_offset + len) % Tabstop != 0);
		} else if (*str == '\r' || *str == '\n') {
			len--;
			if (len > max_len)
				max_len = len;
			len = 0;
		}
#ifdef USE_M17N
		str += n;
#else
		str++;
#endif
	}
	if (status == R_ST_AMP) {
		r2 = tagbuf->ptr;
		t = getescapecmd(&r2);
		if (*t != '\r' && *t != '\n')
			len += get_strwidth(t) + get_strwidth(r2);
	}
	return len > max_len ? len : max_len;
}

int
visible_length_plain(char *str)
{
	int len = 0, max_len = 0;

	while (*str) {
		if (*str == '\t') {
			do {
				len++;
			} while ((visible_length_offset + len) % Tabstop != 0);
			str++;
		} else if (*str == '\r' || *str == '\n') {
			if (len > max_len)
				max_len = len;
			len = 0;
			str++;
		} else {
#ifdef USE_M17N
			len += get_mcwidth(str);
			str += get_mclen(str);
#else
			len++;
			str++;
#endif
		}
	}
	return len > max_len ? len : max_len;
}

static int
maximum_visible_length(char *str, int offset)
{
	visible_length_offset = offset;
	return visible_length(str);
}

static int
maximum_visible_length_plain(char *str, int offset)
{
	visible_length_offset = offset;
	return visible_length_plain(str);
}

void
align(TextLine * lbuf, int width, enum Align mode)
{
	int i, l, l1, l2;
	Str buf, line = lbuf->line;

	if (line->length == 0) {
		for (i = 0; i < width; i++)
			Strcat_char(line, ' ');
		lbuf->pos = width;
		return;
	}
	buf = Strnew();
	l = width - lbuf->pos;
	switch (mode) {
	case ALIGN_CENTER:
		l1 = l / 2;
		l2 = l - l1;
		for (i = 0; i < l1; i++)
			Strcat_char(buf, ' ');
		Strcat(buf, line);
		for (i = 0; i < l2; i++)
			Strcat_char(buf, ' ');
		break;
	case ALIGN_LEFT:
		Strcat(buf, line);
		for (i = 0; i < l; i++)
			Strcat_char(buf, ' ');
		break;
	case ALIGN_RIGHT:
		for (i = 0; i < l; i++)
			Strcat_char(buf, ' ');
		Strcat(buf, line);
		break;
	default:
		return;
	}
	lbuf->line = buf;
	if (lbuf->pos < width)
		lbuf->pos = width;
}

void
print_item(struct table * t, int row, int col, int width, Str buf)
{
	enum Align alignment;
	TextLine *lbuf;

	if (t->tabdata[row])
		lbuf = popTextLine(t->tabdata[row][col]);
	else
		lbuf = NULL;

	if (lbuf != NULL) {
		check_row(t, row);
		alignment = ALIGN_CENTER;
		if ((t->tabattr[row][col] & HTT_ALIGN) == HTT_LEFT)
			alignment = ALIGN_LEFT;
		else if ((t->tabattr[row][col] & HTT_ALIGN) == HTT_RIGHT)
			alignment = ALIGN_RIGHT;
		else if ((t->tabattr[row][col] & HTT_ALIGN) == HTT_CENTER)
			alignment = ALIGN_CENTER;
		align(lbuf, width, alignment);
		Strcat(buf, lbuf->line);
	} else {
		lbuf = newTextLine(NULL, 0);
		align(lbuf, width, ALIGN_CENTER);
		Strcat(buf, lbuf->line);
	}
}


#define T_TOP           0
#define T_MIDDLE        1
#define T_BOTTOM        2

void
print_sep(struct table * t, int row, int type, int maxcol, Str buf)
{
	int forbid;
	int rule_mode;
	int i, k, l, m;

	if (row >= 0)
		check_row(t, row);
	check_row(t, row + 1);
	if ((type == T_TOP || type == T_BOTTOM) && t->border_mode == BORDER_THICK) {
		rule_mode = BORDER_THICK;
	} else {
		rule_mode = BORDER_THIN;
	}
	forbid = 1;
	if (type == T_TOP)
		forbid |= 2;
	else if (type == T_BOTTOM)
		forbid |= 8;
	else if (t->tabattr[row + 1][0] & HTT_Y) {
		forbid |= 4;
	}
	if (t->border_mode != BORDER_NOWIN) {
		push_symbol(buf, RULE(t->border_mode, forbid), symbol_width, 1);
	}
	for (i = 0; i <= maxcol; i++) {
		forbid = 10;
		if (type != T_BOTTOM && (t->tabattr[row + 1][i] & HTT_Y)) {
			if (t->tabattr[row + 1][i] & HTT_X) {
				goto do_last_sep;
			} else {
				for (k = row;
				     k >= 0 && t->tabattr[k] && (t->tabattr[k][i] & HTT_Y);
				     k--);
				m = t->tabwidth[i] + 2 * t->cellpadding;
				for (l = i + 1; l <= t->maxcol && (t->tabattr[row][l] & HTT_X);
				     l++)
					m += t->tabwidth[l] + t->cellspacing;
				print_item(t, k, i, m, buf);
			}
		} else {
			int w = t->tabwidth[i] + 2 * t->cellpadding;
			if (RULE_WIDTH == 2)
				w = (w + 1) / RULE_WIDTH;
			push_symbol(buf, RULE(rule_mode, forbid), symbol_width, w);
		}
do_last_sep:
		if (i < maxcol) {
			forbid = 0;
			if (type == T_TOP)
				forbid |= 2;
			else if (t->tabattr[row][i + 1] & HTT_X) {
				forbid |= 2;
			}
			if (type == T_BOTTOM)
				forbid |= 8;
			else {
				if (t->tabattr[row + 1][i + 1] & HTT_X) {
					forbid |= 8;
				}
				if (t->tabattr[row + 1][i + 1] & HTT_Y) {
					forbid |= 4;
				}
				if (t->tabattr[row + 1][i] & HTT_Y) {
					forbid |= 1;
				}
			}
			if (forbid != 15)	/* forbid==15 means 'no rule
						 * at all' */
				push_symbol(buf, RULE(rule_mode, forbid), symbol_width, 1);
		}
	}
	forbid = 4;
	if (type == T_TOP)
		forbid |= 2;
	if (type == T_BOTTOM)
		forbid |= 8;
	if (t->tabattr[row + 1][maxcol] & HTT_Y) {
		forbid |= 1;
	}
	if (t->border_mode != BORDER_NOWIN)
		push_symbol(buf, RULE(t->border_mode, forbid), symbol_width, 1);
}

static int
get_spec_cell_width(struct table * tbl, int row, int col)
{
	int i, w;

	w = tbl->tabwidth[col];
	for (i = col + 1; i <= tbl->maxcol; i++) {
		check_row(tbl, row);
		if (tbl->tabattr[row][i] & HTT_X)
			w += tbl->tabwidth[i] + tbl->cellspacing;
		else
			break;
	}
	return w;
}

void
do_refill(struct table * tbl, int row, int col, int maxlimit)
{
	TextList *orgdata;
	TextListItem *l;
	struct readbuffer obuf;
	struct html_feed_environ h_env;
	struct environment envs[MAX_ENV_LEVEL];
	int colspan, icell;

	if (tbl->tabdata[row] == NULL || tbl->tabdata[row][col] == NULL)
		return;
	orgdata = (TextList *) tbl->tabdata[row][col];
	tbl->tabdata[row][col] = newGeneralList();

	init_henv(&h_env, &obuf, envs, MAX_ENV_LEVEL,
		  (TextLineList *) tbl->tabdata[row][col],
		  get_spec_cell_width(tbl, row, col), 0);
	obuf.flag |= RB_INTABLE;
	if (h_env.limit > maxlimit)
		h_env.limit = maxlimit;
	if (tbl->border_mode != BORDER_NONE && tbl->vcellpadding > 0)
		do_blankline(&h_env, &obuf, 0, 0, h_env.limit);
	for (l = orgdata->first; l != NULL; l = l->next) {
		if (TAG_IS(l->ptr, "<table_alt", 10)) {
			int id = -1;
			char *p = l->ptr;
			struct parsed_tag *tag;
			if ((tag = parse_tag(&p, TRUE)) != NULL)
				parsedtag_get_value(tag, ATTR_TID, &id);
			if (id >= 0 && id < tbl->ntable) {
				enum Align alignment;
				TextLineListItem *ti;
				struct table *t = tbl->tables[id].ptr;
				int limit = tbl->tables[id].indent + t->total_width;
				tbl->tables[id].ptr = NULL;
				save_fonteffect(&h_env, h_env.obuf);
				flushline(&h_env, &obuf, 0, 2, h_env.limit);
				if (t->vspace > 0 && !(obuf.flag & RB_IGNORE_P))
					do_blankline(&h_env, &obuf, 0, 0, h_env.limit);
				if (RB_GET_ALIGN(h_env.obuf) == RB_CENTER)
					alignment = ALIGN_CENTER;
				else if (RB_GET_ALIGN(h_env.obuf) == RB_RIGHT)
					alignment = ALIGN_RIGHT;
				else
					alignment = ALIGN_LEFT;

				if (alignment != ALIGN_LEFT) {
					for (ti = tbl->tables[id].buf->first;
					     ti != NULL; ti = ti->next)
						align(ti->ptr, h_env.limit, alignment);
				}
				(void)appendTextLineList(h_env.buf, tbl->tables[id].buf);
				if (h_env.maxlimit < limit)
					h_env.maxlimit = limit;
				restore_fonteffect(&h_env, h_env.obuf);
				obuf.flag &= ~RB_IGNORE_P;
				h_env.blank_lines = 0;
				if (t->vspace > 0) {
					do_blankline(&h_env, &obuf, 0, 0, h_env.limit);
					obuf.flag |= RB_IGNORE_P;
				}
			}
		} else
			HTMLlineproc1(l->ptr, &h_env);
	}
	if (obuf.status != R_ST_NORMAL) {
		obuf.status = R_ST_EOL;
		HTMLlineproc1("\n", &h_env);
	}
	completeHTMLstream(&h_env, &obuf);
	flushline(&h_env, &obuf, 0, 2, h_env.limit);
	if (tbl->border_mode == BORDER_NONE) {
		int rowspan = table_rowspan(tbl, row, col);
		if (row + rowspan <= tbl->maxrow) {
			if (tbl->vcellpadding > 0 && !(obuf.flag & RB_IGNORE_P))
				do_blankline(&h_env, &obuf, 0, 0, h_env.limit);
		} else {
			if (tbl->vspace > 0)
				purgeline(&h_env);
		}
	} else {
		if (tbl->vcellpadding > 0) {
			if (!(obuf.flag & RB_IGNORE_P))
				do_blankline(&h_env, &obuf, 0, 0, h_env.limit);
		} else
			purgeline(&h_env);
	}
	if ((colspan = table_colspan(tbl, row, col)) > 1) {
		struct table_cell *cell = &tbl->cell;
		int k;
		k = bsearch_2short(colspan, cell->colspan, col, cell->col, MAXCOL,
				   cell->index, cell->maxcell + 1);
		icell = cell->index[k];
		if (cell->minimum_width[icell] < h_env.maxlimit)
			cell->minimum_width[icell] = h_env.maxlimit;
	} else {
		if (tbl->minimum_width[col] < h_env.maxlimit)
			tbl->minimum_width[col] = h_env.maxlimit;
	}
}

static int
table_rule_width(struct table * t)
{
	if (t->border_mode == BORDER_NONE)
		return 1;
	return RULE_WIDTH;
}

static void
check_cell_width(short *tabwidth, short *cellwidth,
		 short *col, short *colspan, short maxcell,
		 short *indexarray, int space, int dir)
{
	int i, j, k, bcol, ecol;
	int swidth, width;

	for (k = 0; k <= maxcell; k++) {
		j = indexarray[k];
		if (cellwidth[j] <= 0)
			continue;
		bcol = col[j];
		ecol = bcol + colspan[j];
		swidth = 0;
		for (i = bcol; i < ecol; i++)
			swidth += tabwidth[i];

		width = cellwidth[j] - (colspan[j] - 1) * space;
		if (width > swidth) {
			int w = (width - swidth) / colspan[j];
			int r = (width - swidth) % colspan[j];
			for (i = bcol; i < ecol; i++)
				tabwidth[i] += w;
			/* dir {0: horizontal, 1: vertical} */
			if (dir == 1 && r > 0)
				r = colspan[j];
			for (i = 1; i <= r; i++)
				tabwidth[ecol - i]++;
		}
	}
}

void
check_minimum_width(struct table * t, short *tabwidth)
{
	int i;
	struct table_cell *cell = &t->cell;

	for (i = 0; i <= t->maxcol; i++) {
		if (tabwidth[i] < t->minimum_width[i])
			tabwidth[i] = t->minimum_width[i];
	}

	check_cell_width(tabwidth, cell->minimum_width, cell->col, cell->colspan,
			 cell->maxcell, cell->index, t->cellspacing, 0);
}

void
check_maximum_width(struct table * t)
{
	struct table_cell *cell = &t->cell;
	check_cell_width(t->tabwidth, cell->width, cell->col, cell->colspan,
			 cell->maxcell, cell->index, t->cellspacing, 0);
	check_minimum_width(t, t->tabwidth);
}

void
set_table_width(struct table * t, short *newwidth, int maxwidth)
{
	int i, j, k, bcol, ecol;
	struct table_cell *cell = &t->cell;
	char *fixed;
	int swidth, fwidth, width, nvar;
	double s;
	double *dwidth;
	int try_again;

	fixed = NewAtom_N(char, t->maxcol + 1);
	memset(fixed, 0, t->maxcol + 1);
	dwidth = NewAtom_N(double, t->maxcol + 1);

	for (i = 0; i <= t->maxcol; i++) {
		dwidth[i] = 0.0;
		if (t->fixed_width[i] < 0) {
			t->fixed_width[i] = -t->fixed_width[i] * maxwidth / 100;
		}
		if (t->fixed_width[i] > 0) {
			newwidth[i] = t->fixed_width[i];
			fixed[i] = 1;
		} else
			newwidth[i] = 0;
		if (newwidth[i] < t->minimum_width[i])
			newwidth[i] = t->minimum_width[i];
	}

	for (k = 0; k <= cell->maxcell; k++) {
		if (cell->indexarray == NULL) {
			continue;
		}
		j = cell->indexarray[k];
		bcol = cell->col[j];
		ecol = bcol + cell->colspan[j];

		if (cell->fixed_width[j] < 0)
			cell->fixed_width[j] = -cell->fixed_width[j] * maxwidth / 100;

		swidth = 0;
		fwidth = 0;
		nvar = 0;
		for (i = bcol; i < ecol; i++) {
			if (fixed[i]) {
				fwidth += newwidth[i];
			} else {
				swidth += newwidth[i];
				nvar++;
			}
		}
		width = max(cell->fixed_width[j], cell->minimum_width[j])
			- (cell->colspan[j] - 1) * t->cellspacing;
		if (nvar > 0 && width > fwidth + swidth) {
			s = 0.;
			for (i = bcol; i < ecol; i++) {
				if (!fixed[i])
					s += weight3(t->tabwidth[i]);
			}
			for (i = bcol; i < ecol; i++) {
				if (!fixed[i])
					dwidth[i] = (width - fwidth) * weight3(t->tabwidth[i]) / s;
				else
					dwidth[i] = (double) newwidth[i];
			}
			dv2sv(dwidth, newwidth, cell->colspan[j]);
			if (cell->fixed_width[j] > 0) {
				for (i = bcol; i < ecol; i++)
					fixed[i] = 1;
			}
		}
	}

	do {
		nvar = 0;
		swidth = 0;
		fwidth = 0;
		for (i = 0; i <= t->maxcol; i++) {
			if (fixed[i]) {
				fwidth += newwidth[i];
			} else {
				swidth += newwidth[i];
				nvar++;
			}
		}
		width = maxwidth - t->maxcol * t->cellspacing;
		if (nvar == 0 || width <= fwidth + swidth)
			break;

		s = 0.;
		for (i = 0; i <= t->maxcol; i++) {
			if (!fixed[i])
				s += weight3(t->tabwidth[i]);
		}
		for (i = 0; i <= t->maxcol; i++) {
			if (!fixed[i])
				dwidth[i] = (width - fwidth) * weight3(t->tabwidth[i]) / s;
			else
				dwidth[i] = (double) newwidth[i];
		}
		dv2sv(dwidth, newwidth, t->maxcol + 1);

		try_again = 0;
		for (i = 0; i <= t->maxcol; i++) {
			if (!fixed[i]) {
				if (newwidth[i] > t->tabwidth[i]) {
					newwidth[i] = t->tabwidth[i];
					fixed[i] = 1;
					try_again = 1;
				} else if (newwidth[i] < t->minimum_width[i]) {
					newwidth[i] = t->minimum_width[i];
					fixed[i] = 1;
					try_again = 1;
				}
			}
		}
	} while (try_again);
}

void
check_table_height(struct table * t)
{
	int i, j, k;
	struct {
		short *row;
		short *rowspan;
		short *indexarray;
		short maxcell;
		short size;
		short *height;
	} cell = {0};
	int space = 0;

	cell.size = 0;
	cell.maxcell = -1;

	for (j = 0; j <= t->maxrow; j++) {
		if (!t->tabattr[j])
			continue;
		for (i = 0; i <= t->maxcol; i++) {
			int t_dep, rowspan;
			if (t->tabattr[j][i] & (HTT_X | HTT_Y))
				continue;

			if (t->tabdata[j][i] == NULL)
				t_dep = 0;
			else
				t_dep = t->tabdata[j][i]->nitem;

			rowspan = table_rowspan(t, j, i);
			if (rowspan > 1) {
				int c = cell.maxcell + 1;
				k = bsearch_2short(rowspan, cell.rowspan,
				j, cell.row, t->maxrow + 1, cell.indexarray,
						   c);
				if (k <= cell.maxcell) {
					int idx = cell.indexarray[k];
					if (cell.row[idx] == j && cell.rowspan[idx] == rowspan)
						c = idx;
				}
				if (c >= MAXROWCELL)
					continue;
				if (c >= cell.size) {
					if (cell.size == 0) {
						cell.size = max(MAXCELL, c + 1);
						cell.row = NewAtom_N(short, cell.size);
						cell.rowspan = NewAtom_N(short, cell.size);
						cell.indexarray = NewAtom_N(short, cell.size);
						cell.height = NewAtom_N(short, cell.size);
					} else {
						cell.size = max(cell.size + MAXCELL, c + 1);
						cell.row = New_Reuse(short, cell.row, cell.size);
						cell.rowspan = New_Reuse(short, cell.rowspan,
								 cell.size);
						cell.indexarray = New_Reuse(short, cell.indexarray,
								 cell.size);
						cell.height = New_Reuse(short, cell.height, cell.size);
					}
				}
				if (c > cell.maxcell) {
					cell.maxcell++;
					cell.row[cell.maxcell] = j;
					cell.rowspan[cell.maxcell] = rowspan;
					cell.height[cell.maxcell] = 0;
					if (cell.maxcell > k) {
						int ii;
						for (ii = cell.maxcell; ii > k; ii--)
							cell.indexarray[ii] = cell.indexarray[ii - 1];
					}
					cell.indexarray[k] = cell.maxcell;
				}
				if (cell.height[c] < t_dep)
					cell.height[c] = t_dep;
				continue;
			}
			if (t->tabheight[j] < t_dep)
				t->tabheight[j] = t_dep;
		}
	}

	switch (t->border_mode) {
	case BORDER_THIN:
	case BORDER_THICK:
	case BORDER_NOWIN:
		space = 1;
		break;
	case BORDER_NONE:
		space = 0;
	}
	check_cell_width(t->tabheight, cell.height, cell.row, cell.rowspan,
			 cell.maxcell, cell.indexarray, space, 1);
}

#define CHECK_MINIMUM	1
#define CHECK_FIXED	2

int
get_table_width(struct table * t, short *orgwidth, short *cellwidth, int flag)
{
#ifdef __GNUC__
	short newwidth[t->maxcol + 1];
#else				/* not __GNUC__ */
	short newwidth[MAXCOL];
#endif				/* not __GNUC__ */
	int i;
	int swidth;
	struct table_cell *cell = &t->cell;
	int rulewidth = table_rule_width(t);

	for (i = 0; i <= t->maxcol; i++)
		newwidth[i] = max(orgwidth[i], 0);

	if (flag & CHECK_FIXED) {
#ifdef __GNUC__
		short ccellwidth[cell->maxcell + 1];
#else				/* not __GNUC__ */
		short ccellwidth[MAXCELL];
#endif				/* not __GNUC__ */
		for (i = 0; i <= t->maxcol; i++) {
			if (newwidth[i] < t->fixed_width[i])
				newwidth[i] = t->fixed_width[i];
		}
		for (i = 0; i <= cell->maxcell; i++) {
			ccellwidth[i] = cellwidth[i];
			if (ccellwidth[i] < cell->fixed_width[i])
				ccellwidth[i] = cell->fixed_width[i];
		}
		check_cell_width(newwidth, ccellwidth, cell->col, cell->colspan,
			     cell->maxcell, cell->index, t->cellspacing, 0);
	} else {
		check_cell_width(newwidth, cellwidth, cell->col, cell->colspan,
			     cell->maxcell, cell->index, t->cellspacing, 0);
	}
	if (flag & CHECK_MINIMUM)
		check_minimum_width(t, newwidth);

	swidth = 0;
	for (i = 0; i <= t->maxcol; i++) {
		swidth += ceil_at_intervals(newwidth[i], rulewidth);
	}
	swidth += table_border_width(t);
	return swidth;
}

#define minimum_table_width(t)\
(get_table_width(t,t->minimum_width,t->cell.minimum_width,0))
#define maximum_table_width(t)\
  (get_table_width(t,t->tabwidth,t->cell.width,CHECK_FIXED))
#define fixed_table_width(t)\
  (get_table_width(t,t->fixed_width,t->cell.fixed_width,CHECK_MINIMUM))

void
renderCoTable(struct table * tbl, int maxlimit)
{
	struct readbuffer obuf;
	struct html_feed_environ h_env;
	struct environment envs[MAX_ENV_LEVEL];
	struct table *t;
	int i, col, row;
	int indent, maxwidth;

	for (i = 0; i < tbl->ntable; i++) {
		t = tbl->tables[i].ptr;
		col = tbl->tables[i].col;
		row = tbl->tables[i].row;
		indent = tbl->tables[i].indent;

		init_henv(&h_env, &obuf, envs, MAX_ENV_LEVEL, tbl->tables[i].buf,
			  get_spec_cell_width(tbl, row, col), indent);
		check_row(tbl, row);
		if (h_env.limit > maxlimit)
			h_env.limit = maxlimit;
		if (t->total_width == 0)
			maxwidth = h_env.limit - indent;
		else if (t->total_width > 0)
			maxwidth = t->total_width;
		else
			maxwidth = t->total_width = -t->total_width * h_env.limit / 100;
		renderTable(t, maxwidth, &h_env);
	}
}

static void
make_caption(struct table * t, struct html_feed_environ * h_env)
{
	struct html_feed_environ henv;
	struct readbuffer obuf;
	struct environment envs[MAX_ENV_LEVEL];
	int limit;

	if (t->caption->length <= 0)
		return;

	if (t->total_width > 0)
		limit = t->total_width;
	else
		limit = h_env->limit;
	init_henv(&henv, &obuf, envs, MAX_ENV_LEVEL, newTextLineList(),
		  limit, h_env->envs[h_env->envc].indent);
	HTMLlineproc1("<center>", &henv);
	HTMLlineproc0(t->caption->ptr, &henv, FALSE);
	HTMLlineproc1("</center>", &henv);

	if (t->total_width < henv.maxlimit)
		t->total_width = henv.maxlimit;
	limit = h_env->limit;
	h_env->limit = t->total_width;
	HTMLlineproc1("<center>", h_env);
	HTMLlineproc0(t->caption->ptr, h_env, FALSE);
	HTMLlineproc1("</center>", h_env);
	h_env->limit = limit;
}

void
renderTable(struct table * t, int max_width, struct html_feed_environ * h_env)
{
	int i, j, w, r, h;
	Str renderbuf;
	short new_tabwidth[MAXCOL];
	int width;
	int rulewidth;
	Str vrulea = NULL, vruleb = NULL, vrulec = NULL;
#ifdef ID_EXT
	Str idtag;
#endif				/* ID_EXT */

	t->total_height = 0;
	if (t->maxcol < 0) {
		make_caption(t, h_env);
		return;
	}
	if (t->sloppy_width > max_width)
		max_width = t->sloppy_width;

	rulewidth = table_rule_width(t);

	max_width -= table_border_width(t);

	if (rulewidth > 1)
		max_width = floor_at_intervals(max_width, rulewidth);

	if (max_width < rulewidth)
		max_width = rulewidth;

	check_maximum_width(t);

	set_table_width(t, new_tabwidth, max_width);
	for (i = 0; i <= t->maxcol; i++) {
		t->tabwidth[i] = new_tabwidth[i];
	}

	check_minimum_width(t, t->tabwidth);
	for (i = 0; i <= t->maxcol; i++)
		t->tabwidth[i] = ceil_at_intervals(t->tabwidth[i], rulewidth);

	renderCoTable(t, h_env->limit);

	for (i = 0; i <= t->maxcol; i++) {
		for (j = 0; j <= t->maxrow; j++) {
			check_row(t, j);
			if (t->tabattr[j][i] & HTT_Y)
				continue;
			do_refill(t, j, i, h_env->limit);
		}
	}

	check_minimum_width(t, t->tabwidth);
	t->total_width = 0;
	for (i = 0; i <= t->maxcol; i++) {
		t->tabwidth[i] = ceil_at_intervals(t->tabwidth[i], rulewidth);
		t->total_width += t->tabwidth[i];
	}

	t->total_width += table_border_width(t);

	check_table_height(t);

	for (i = 0; i <= t->maxcol; i++) {
		for (j = 0; j <= t->maxrow; j++) {
			TextLineList *l;
			int k;
			if ((t->tabattr[j][i] & HTT_Y) ||
			    (t->tabattr[j][i] & HTT_TOP) || (t->tabdata[j][i] == NULL))
				continue;
			h = t->tabheight[j];
			for (k = j + 1; k <= t->maxrow; k++) {
				if (!(t->tabattr[k][i] & HTT_Y))
					break;
				h += t->tabheight[k];
				switch (t->border_mode) {
				case BORDER_THIN:
				case BORDER_THICK:
				case BORDER_NOWIN:
					h += 1;
					break;
				}
			}
			h -= t->tabdata[j][i]->nitem;
			if (t->tabattr[j][i] & HTT_MIDDLE)
				h /= 2;
			if (h <= 0)
				continue;
			l = newTextLineList();
			for (k = 0; k < h; k++)
				pushTextLine(l, newTextLine(NULL, 0));
			t->tabdata[j][i] = appendGeneralList((GeneralList *) l,
							  t->tabdata[j][i]);
		}
	}

	/* table output */
	width = t->total_width;

	make_caption(t, h_env);

	HTMLlineproc1("<pre for_table>", h_env);
#ifdef ID_EXT
	if (t->id != NULL) {
		idtag = Sprintf("<_id id=\"%s\">", html_quote((t->id)->ptr));
		HTMLlineproc1(idtag->ptr, h_env);
	}
#endif				/* ID_EXT */
	switch (t->border_mode) {
	case BORDER_THIN:
	case BORDER_THICK:
		renderbuf = Strnew();
		print_sep(t, -1, T_TOP, t->maxcol, renderbuf);
		push_render_image(renderbuf, width, t->total_width, h_env);
		t->total_height += 1;
		break;
	}
	vruleb = Strnew();
	switch (t->border_mode) {
	case BORDER_THIN:
	case BORDER_THICK:
		vrulea = Strnew();
		vrulec = Strnew();
		push_symbol(vrulea, TK_VERTICALBAR(t->border_mode), symbol_width, 1);
		for (i = 0; i < t->cellpadding; i++) {
			Strcat_char(vrulea, ' ');
			Strcat_char(vruleb, ' ');
			Strcat_char(vrulec, ' ');
		}
		push_symbol(vrulec, TK_VERTICALBAR(t->border_mode), symbol_width, 1);
	case BORDER_NOWIN:
		push_symbol(vruleb, TK_VERTICALBAR(BORDER_THIN), symbol_width, 1);
		for (i = 0; i < t->cellpadding; i++)
			Strcat_char(vruleb, ' ');
		break;
	case BORDER_NONE:
		for (i = 0; i < t->cellspacing; i++)
			Strcat_char(vruleb, ' ');
	}

	for (r = 0; r <= t->maxrow; r++) {
		for (h = 0; h < t->tabheight[r]; h++) {
			renderbuf = Strnew();
			if (t->border_mode == BORDER_THIN
			    || t->border_mode == BORDER_THICK)
				Strcat(renderbuf, vrulea);
#ifdef ID_EXT
			if (t->tridvalue[r] != NULL && h == 0) {
				idtag = Sprintf("<_id id=\"%s\">",
					html_quote((t->tridvalue[r])->ptr));
				Strcat(renderbuf, idtag);
			}
#endif				/* ID_EXT */
			for (i = 0; i <= t->maxcol; i++) {
				check_row(t, r);
#ifdef ID_EXT
				if (t->tabidvalue[r][i] != NULL && h == 0) {
					idtag = Sprintf("<_id id=\"%s\">",
							html_quote((t->tabidvalue[r][i])->ptr));
					Strcat(renderbuf, idtag);
				}
#endif				/* ID_EXT */
				if (!(t->tabattr[r][i] & HTT_X)) {
					w = t->tabwidth[i];
					for (j = i + 1;
					     j <= t->maxcol && (t->tabattr[r][j] & HTT_X); j++)
						w += t->tabwidth[j] + t->cellspacing;
					if (t->tabattr[r][i] & HTT_Y) {
						for (j = r - 1; j >= 0 && t->tabattr[j]
						     && (t->tabattr[j][i] & HTT_Y); j--);
						print_item(t, j, i, w, renderbuf);
					} else
						print_item(t, r, i, w, renderbuf);
				}
				if (i < t->maxcol && !(t->tabattr[r][i + 1] & HTT_X))
					Strcat(renderbuf, vruleb);
			}
			switch (t->border_mode) {
			case BORDER_THIN:
			case BORDER_THICK:
				Strcat(renderbuf, vrulec);
				t->total_height += 1;
				break;
			}
			push_render_image(renderbuf, width, t->total_width, h_env);
		}
		if (r < t->maxrow && t->border_mode != BORDER_NONE) {
			renderbuf = Strnew();
			print_sep(t, r, T_MIDDLE, t->maxcol, renderbuf);
			push_render_image(renderbuf, width, t->total_width, h_env);
		}
		t->total_height += t->tabheight[r];
	}
	switch (t->border_mode) {
	case BORDER_THIN:
	case BORDER_THICK:
		renderbuf = Strnew();
		print_sep(t, t->maxrow, T_BOTTOM, t->maxcol, renderbuf);
		push_render_image(renderbuf, width, t->total_width, h_env);
		t->total_height += 1;
		break;
	}
	if (t->total_height == 0) {
		renderbuf = Strnew_charp(" ");
		t->total_height++;
		t->total_width = 1;
		push_render_image(renderbuf, 1, t->total_width, h_env);
	}
	HTMLlineproc1("</pre>", h_env);
}

#ifdef TABLE_NO_COMPACT
#define THR_PADDING 2
#else
#define THR_PADDING 4
#endif

struct table *
begin_table(int border, int spacing, int padding, int vspace)
{
	struct table *t;
	int mincell = minimum_cellspacing(border);
	int rcellspacing;
	int mincell_pixels = round(mincell * pixel_per_char);
	int ppc = round(pixel_per_char);

	t = newTable();
	t->row = t->col = -1;
	t->maxcol = -1;
	t->maxrow = -1;
	t->border_mode = border;
	t->flag = 0;
	if (border == BORDER_NOWIN)
		t->flag |= TBL_EXPAND_OK;

	rcellspacing = spacing + 2 * padding;
	switch (border) {
	case BORDER_THIN:
	case BORDER_THICK:
	case BORDER_NOWIN:
		t->cellpadding = padding - (mincell_pixels - 4) / 2;
		break;
	case BORDER_NONE:
		t->cellpadding = rcellspacing - mincell_pixels;
	}
	if (t->cellpadding >= ppc)
		t->cellpadding /= ppc;
	else if (t->cellpadding > 0)
		t->cellpadding = 1;
	else
		t->cellpadding = 0;

	switch (border) {
	case BORDER_THIN:
	case BORDER_THICK:
	case BORDER_NOWIN:
		t->cellspacing = 2 * t->cellpadding + mincell;
		break;
	case BORDER_NONE:
		t->cellspacing = t->cellpadding + mincell;
	}

	if (border == BORDER_NONE) {
		if (rcellspacing / 2 + vspace <= 1)
			t->vspace = 0;
		else
			t->vspace = 1;
	} else {
		if (vspace < ppc)
			t->vspace = 0;
		else
			t->vspace = 1;
	}

	if (border == BORDER_NONE) {
		if (rcellspacing <= THR_PADDING)
			t->vcellpadding = 0;
		else
			t->vcellpadding = 1;
	} else {
		if (padding < 2 * ppc - 2)
			t->vcellpadding = 0;
		else
			t->vcellpadding = 1;
	}

	return t;
}

void
end_table(struct table * tbl)
{
	struct table_cell *cell = &tbl->cell;
	int i, rulewidth = table_rule_width(tbl);
	if (rulewidth > 1) {
		if (tbl->total_width > 0)
			tbl->total_width = ceil_at_intervals(tbl->total_width, rulewidth);
		for (i = 0; i <= tbl->maxcol; i++) {
			tbl->minimum_width[i] =
				ceil_at_intervals(tbl->minimum_width[i], rulewidth);
			tbl->tabwidth[i] = ceil_at_intervals(tbl->tabwidth[i], rulewidth);
			if (tbl->fixed_width[i] > 0)
				tbl->fixed_width[i] =
					ceil_at_intervals(tbl->fixed_width[i], rulewidth);
		}
		for (i = 0; i <= cell->maxcell; i++) {
			cell->minimum_width[i] =
				ceil_at_intervals(cell->minimum_width[i], rulewidth);
			cell->width[i] = ceil_at_intervals(cell->width[i], rulewidth);
			if (cell->fixed_width[i] > 0)
				cell->fixed_width[i] =
					ceil_at_intervals(cell->fixed_width[i], rulewidth);
		}
	}
	tbl->sloppy_width = fixed_table_width(tbl);
	if (tbl->total_width > tbl->sloppy_width)
		tbl->sloppy_width = tbl->total_width;
}

static void
check_minimum0(struct table * t, int min)
{
	int i, w, ww;
	struct table_cell *cell;

	if (t->col < 0)
		return;
	if (t->tabwidth[t->col] < 0)
		return;
	check_row(t, t->row);
	w = table_colspan(t, t->row, t->col);
	min += t->indent;
	if (w == 1)
		ww = min;
	else {
		cell = &t->cell;
		ww = 0;
		if (cell->icell >= 0 && cell->minimum_width[cell->icell] < min)
			cell->minimum_width[cell->icell] = min;
	}
	for (i = t->col;
	 i <= t->maxcol && (i == t->col || (t->tabattr[t->row][i] & HTT_X));
	     i++) {
		if (t->minimum_width[i] < ww)
			t->minimum_width[i] = ww;
	}
}

static int
setwidth0(struct table * t, struct table_mode * mode)
{
	int w;
	int width = t->tabcontentssize;
	struct table_cell *cell = &t->cell;

	if (t->col < 0)
		return -1;
	if (t->tabwidth[t->col] < 0)
		return -1;
	check_row(t, t->row);
	if (t->linfo.prev_spaces > 0)
		width -= t->linfo.prev_spaces;
	w = table_colspan(t, t->row, t->col);
	if (w == 1) {
		if (t->tabwidth[t->col] < width)
			t->tabwidth[t->col] = width;
	} else if (cell->icell >= 0) {
		if (cell->width[cell->icell] < width)
			cell->width[cell->icell] = width;
	}
	return width;
}

static void
setwidth(struct table * t, struct table_mode * mode)
{
	int width = setwidth0(t, mode);
	if (width < 0)
		return;
#ifdef NOWRAP
	if (t->tabattr[t->row][t->col] & HTT_NOWRAP)
		check_minimum0(t, width);
#endif				/* NOWRAP */
	if (mode->pre_mode & (TBLM_NOBR | TBLM_PRE | TBLM_PRE_INT) &&
	    mode->nobr_offset >= 0)
		check_minimum0(t, width - mode->nobr_offset);
}

static void
addcontentssize(struct table * t, int width)
{

	if (t->col < 0)
		return;
	if (t->tabwidth[t->col] < 0)
		return;
	check_row(t, t->row);
	t->tabcontentssize += width;
}

static void table_close_anchor0(struct table * tbl, struct table_mode * mode);

static void
clearcontentssize(struct table * t, struct table_mode * mode)
{
	table_close_anchor0(t, mode);
	mode->nobr_offset = 0;
	t->linfo.prev_spaces = -1;
	set_space_to_prevchar(t->linfo.prevchar);
	t->linfo.prev_ctype = PC_ASCII;
	t->linfo.length = 0;
	t->tabcontentssize = 0;
}

static void
begin_cell(struct table * t, struct table_mode * mode)
{
	clearcontentssize(t, mode);
	mode->indent_level = 0;
	mode->nobr_level = 0;
	mode->pre_mode = 0;
	t->indent = 0;
	t->flag |= TBL_IN_COL;

	if (t->suspended_data) {
		check_row(t, t->row);
		if (t->tabdata[t->row][t->col] == NULL)
			t->tabdata[t->row][t->col] = newGeneralList();
		appendGeneralList(t->tabdata[t->row][t->col],
				  (GeneralList *) t->suspended_data);
		t->suspended_data = NULL;
	}
}

void
check_rowcol(struct table * tbl, struct table_mode * mode)
{
	int row = tbl->row, col = tbl->col;

	if (!(tbl->flag & TBL_IN_ROW)) {
		tbl->flag |= TBL_IN_ROW;
		tbl->row++;
		if (tbl->row > tbl->maxrow)
			tbl->maxrow = tbl->row;
		tbl->col = -1;
	}
	if (tbl->row == -1)
		tbl->row = 0;
	if (tbl->col == -1)
		tbl->col = 0;

	for (;; tbl->row++) {
		check_row(tbl, tbl->row);
		for (; tbl->col < MAXCOL &&
		     tbl->tabattr[tbl->row][tbl->col] & (HTT_X | HTT_Y); tbl->col++);
		if (tbl->col < MAXCOL)
			break;
		tbl->col = 0;
	}
	if (tbl->row > tbl->maxrow)
		tbl->maxrow = tbl->row;
	if (tbl->col > tbl->maxcol)
		tbl->maxcol = tbl->col;

	if (tbl->row != row || tbl->col != col)
		begin_cell(tbl, mode);
	tbl->flag |= TBL_IN_COL;
}

int
skip_space(struct table * t, char *line, struct table_linfo * linfo,
	   int checkminimum)
{
	int skip = 0, s = linfo->prev_spaces;
	Lineprop ctype, prev_ctype = linfo->prev_ctype;
	Str prevchar = linfo->prevchar;
	int w = linfo->length;
	int min = 1;

	if (*line == '<' && line[strlen(line) - 1] == '>') {
		if (checkminimum)
			check_minimum0(t, visible_length(line));
		return 0;
	}
	while (*line) {
		char *save = line, *c = line;
		int ec, len, wlen, plen;
		ctype = get_mctype(line);
		len = get_mcwidth(line);
		wlen = plen = get_mclen(line);

		if (min < w)
			min = w;
		if (ctype == PC_ASCII && isspace((unsigned char)*c)) {
			w = 0;
			s++;
		} else {
			if (*c == '&') {
				ec = getescapechar(&line);
				if (ec >= 0) {
					c = conv_entity(ec);
					ctype = get_mctype(c);
					len = get_strwidth(c);
					wlen = line - save;
					plen = get_mclen(c);
				}
			}
			if (prevchar->length && is_boundary((unsigned char *) prevchar->ptr,
						     (unsigned char *) c)) {
				w = len;
			} else {
				w += len;
			}
			if (s > 0) {
#ifdef USE_M17N
				if (ctype == PC_KANJI1 && prev_ctype == PC_KANJI1)
					skip += s;
				else
#endif
					skip += s - 1;
			}
			s = 0;
			prev_ctype = ctype;
		}
		set_prevchar(prevchar, c, plen);
		line = save + wlen;
	}
	if (s > 1) {
		skip += s - 1;
		linfo->prev_spaces = 1;
	} else {
		linfo->prev_spaces = s;
	}
	linfo->prev_ctype = prev_ctype;
	linfo->prevchar = prevchar;

	if (checkminimum) {
		if (min < w)
			min = w;
		linfo->length = w;
		check_minimum0(t, min);
	}
	return skip;
}

static void
feed_table_inline_tag(struct table * tbl,
		      char *line, struct table_mode * mode, int width)
{
	check_rowcol(tbl, mode);
	pushdata(tbl, tbl->row, tbl->col, line);
	if (width >= 0) {
		check_minimum0(tbl, width);
		addcontentssize(tbl, width);
		setwidth(tbl, mode);
	}
}

static void
feed_table_block_tag(struct table * tbl,
		  char *line, struct table_mode * mode, int indent, int cmd)
{
	int offset;
	if (mode->indent_level <= 0 && indent == -1)
		return;
	setwidth(tbl, mode);
	feed_table_inline_tag(tbl, line, mode, -1);
	clearcontentssize(tbl, mode);
	if (indent == 1) {
		mode->indent_level++;
		if (mode->indent_level <= MAX_INDENT_LEVEL)
			tbl->indent += INDENT_INCR;
	} else if (indent == -1) {
		mode->indent_level--;
		if (mode->indent_level < MAX_INDENT_LEVEL)
			tbl->indent -= INDENT_INCR;
	}
	offset = tbl->indent;
	if (cmd == HTML_DT) {
		if (mode->indent_level > 0 && mode->indent_level <= MAX_INDENT_LEVEL)
			offset -= INDENT_INCR;
	}
	if (tbl->indent > 0) {
		check_minimum0(tbl, 0);
		addcontentssize(tbl, offset);
	}
}

static void
table_close_select(struct table * tbl, struct table_mode * mode, int width)
{
	Str tmp = process_n_select();
	mode->pre_mode &= ~TBLM_INSELECT;
	mode->end_tag = 0;
	feed_table1(tbl, tmp, mode, width);
}

static void
table_close_textarea(struct table * tbl, struct table_mode * mode, int width)
{
	Str tmp = process_n_textarea();
	mode->pre_mode &= ~TBLM_INTXTA;
	mode->end_tag = 0;
	feed_table1(tbl, tmp, mode, width);
}

static void
table_close_anchor0(struct table * tbl, struct table_mode * mode)
{
	if (!(mode->pre_mode & TBLM_ANCHOR))
		return;
	mode->pre_mode &= ~TBLM_ANCHOR;
	if (tbl->tabcontentssize == mode->anchor_offset) {
		check_minimum0(tbl, 1);
		addcontentssize(tbl, 1);
		setwidth(tbl, mode);
	} else if (tbl->linfo.prev_spaces > 0 &&
		   tbl->tabcontentssize - 1 == mode->anchor_offset) {
		if (tbl->linfo.prev_spaces > 0)
			tbl->linfo.prev_spaces = -1;
	}
}

#define TAG_ACTION_NONE 0
#define TAG_ACTION_FEED 1
#define TAG_ACTION_TABLE 2
#define TAG_ACTION_N_TABLE 3
#define TAG_ACTION_PLAIN 4

#define CASE_TABLE_TAG \
	case HTML_TABLE:\
	case HTML_N_TABLE:\
	case HTML_TR:\
	case HTML_N_TR:\
	case HTML_TD:\
	case HTML_N_TD:\
	case HTML_TH:\
	case HTML_N_TH:\
	case HTML_THEAD:\
	case HTML_N_THEAD:\
	case HTML_TBODY:\
	case HTML_N_TBODY:\
	case HTML_TFOOT:\
	case HTML_N_TFOOT:\
	case HTML_COLGROUP:\
	case HTML_N_COLGROUP:\
	case HTML_COL

#define ATTR_ROWSPAN_MAX 32766

static int
feed_table_tag(struct table * tbl, char *line, struct table_mode * mode,
	       int width, struct parsed_tag * tag)
{
	int cmd;
#ifdef ID_EXT
	char *p;
#endif
	struct table_cell *cell = &tbl->cell;
	int colspan, rowspan;
	int col, prev_col;
	int i, j, k, v, v0, w, id;
	Str tok, tmp, anchor;
	table_attr halign, valign;

	cmd = tag->tagid;

	if (mode->pre_mode & TBLM_PLAIN) {
		if (mode->end_tag == cmd) {
			mode->pre_mode &= ~TBLM_PLAIN;
			mode->end_tag = 0;
			feed_table_block_tag(tbl, line, mode, 0, cmd);
			return TAG_ACTION_NONE;
		}
		return TAG_ACTION_PLAIN;
	}
	if (mode->pre_mode & TBLM_INTXTA) {
		switch (cmd) {
	CASE_TABLE_TAG:
		case HTML_N_TEXTAREA:
			table_close_textarea(tbl, mode, width);
			if (cmd == HTML_N_TEXTAREA)
				return TAG_ACTION_NONE;
			break;
		default:
			return TAG_ACTION_FEED;
		}
	}
	if (mode->pre_mode & TBLM_SCRIPT) {
		if (mode->end_tag == cmd) {
			mode->pre_mode &= ~TBLM_SCRIPT;
			mode->end_tag = 0;
			return TAG_ACTION_NONE;
		}
		return TAG_ACTION_PLAIN;
	}
	if (mode->pre_mode & TBLM_STYLE) {
		if (mode->end_tag == cmd) {
			mode->pre_mode &= ~TBLM_STYLE;
			mode->end_tag = 0;
			return TAG_ACTION_NONE;
		}
		return TAG_ACTION_PLAIN;
	}
	/*
	 * failsafe: a tag other than <option></option>and </select> in *
	 * <select> environment is regarded as the end of <select>.
	 */
	if (mode->pre_mode & TBLM_INSELECT) {
		switch (cmd) {
	CASE_TABLE_TAG:
		case HTML_N_FORM:
		case HTML_N_SELECT:	/* mode->end_tag */
			table_close_select(tbl, mode, width);
			if (cmd == HTML_N_SELECT)
				return TAG_ACTION_NONE;
			break;
		default:
			return TAG_ACTION_FEED;
		}
	}
	if (mode->caption) {
		switch (cmd) {
	CASE_TABLE_TAG:
		case HTML_N_CAPTION:
			mode->caption = 0;
			if (cmd == HTML_N_CAPTION)
				return TAG_ACTION_NONE;
			break;
		default:
			return TAG_ACTION_FEED;
		}
	}
	if (mode->pre_mode & TBLM_PRE) {
		switch (cmd) {
		case HTML_NOBR:
		case HTML_N_NOBR:
		case HTML_PRE_INT:
		case HTML_N_PRE_INT:
			return TAG_ACTION_NONE;
		}
	}
	switch (cmd) {
	case HTML_TABLE:
		check_rowcol(tbl, mode);
		return TAG_ACTION_TABLE;
	case HTML_N_TABLE:
		if (tbl->suspended_data)
			check_rowcol(tbl, mode);
		return TAG_ACTION_N_TABLE;
	case HTML_TR:
		if (tbl->col >= 0 && tbl->tabcontentssize > 0)
			setwidth(tbl, mode);
		tbl->col = -1;
		tbl->row++;
		tbl->flag |= TBL_IN_ROW;
		tbl->flag &= ~TBL_IN_COL;
		halign = 0;
		valign = 0;
		if (parsedtag_get_value(tag, ATTR_ALIGN, &i)) {
			switch (i) {
			case ALIGN_LEFT:
				halign = (HTT_LEFT | HTT_TRSET);
				break;
			case ALIGN_RIGHT:
				halign = (HTT_RIGHT | HTT_TRSET);
				break;
			case ALIGN_CENTER:
				halign = (HTT_CENTER | HTT_TRSET);
				break;
			}
		}
		if (parsedtag_get_value(tag, ATTR_VALIGN, &i)) {
			switch (i) {
			case VALIGN_TOP:
				valign = (HTT_TOP | HTT_VTRSET);
				break;
			case VALIGN_MIDDLE:
				valign = (HTT_MIDDLE | HTT_VTRSET);
				break;
			case VALIGN_BOTTOM:
				valign = (HTT_BOTTOM | HTT_VTRSET);
				break;
			}
		}
#ifdef ID_EXT
		if (parsedtag_get_value(tag, ATTR_ID, &p))
			tbl->tridvalue[tbl->row] = Strnew_charp(p);
#endif				/* ID_EXT */
		tbl->trattr = halign | valign;
		break;
	case HTML_TH:
	case HTML_TD:
		prev_col = tbl->col;
		if (tbl->col >= 0 && tbl->tabcontentssize > 0)
			setwidth(tbl, mode);
		if (tbl->row == -1) {
			/* for broken HTML... */
			tbl->row = -1;
			tbl->col = -1;
			tbl->maxrow = tbl->row;
		}
		if (tbl->col == -1) {
			if (!(tbl->flag & TBL_IN_ROW)) {
				tbl->row++;
				tbl->flag |= TBL_IN_ROW;
			}
			if (tbl->row > tbl->maxrow)
				tbl->maxrow = tbl->row;
		}
		tbl->col++;
		check_row(tbl, tbl->row);
		while (tbl->tabattr[tbl->row][tbl->col]) {
			tbl->col++;
		}
		if (tbl->col > MAXCOL - 1) {
			tbl->col = prev_col;
			return TAG_ACTION_NONE;
		}
		if (tbl->col > tbl->maxcol) {
			tbl->maxcol = tbl->col;
		}
		colspan = rowspan = 1;
		if (tbl->trattr & HTT_TRSET)
			halign = (tbl->trattr & HTT_ALIGN);
		else if (cmd == HTML_TH)
			halign = HTT_CENTER;
		else
			halign = HTT_LEFT;
		if (tbl->trattr & HTT_VTRSET)
			valign = (tbl->trattr & HTT_VALIGN);
		else
			valign = HTT_MIDDLE;
		if (parsedtag_get_value(tag, ATTR_ROWSPAN, &rowspan)) {
			if (rowspan > ATTR_ROWSPAN_MAX) {
				rowspan = ATTR_ROWSPAN_MAX;
			}
			if ((tbl->row + rowspan) >= tbl->max_rowsize)
				check_row(tbl, tbl->row + rowspan);
		}
		if (parsedtag_get_value(tag, ATTR_COLSPAN, &colspan)) {
			if ((tbl->col + colspan) >= MAXCOL) {
				/* Can't expand column */
				colspan = MAXCOL - tbl->col;
			}
		}
		if (parsedtag_get_value(tag, ATTR_ALIGN, &i)) {
			switch (i) {
			case ALIGN_LEFT:
				halign = HTT_LEFT;
				break;
			case ALIGN_RIGHT:
				halign = HTT_RIGHT;
				break;
			case ALIGN_CENTER:
				halign = HTT_CENTER;
				break;
			}
		}
		if (parsedtag_get_value(tag, ATTR_VALIGN, &i)) {
			switch (i) {
			case VALIGN_TOP:
				valign = HTT_TOP;
				break;
			case VALIGN_MIDDLE:
				valign = HTT_MIDDLE;
				break;
			case VALIGN_BOTTOM:
				valign = HTT_BOTTOM;
				break;
			}
		}
#ifdef NOWRAP
		if (parsedtag_exists(tag, ATTR_NOWRAP))
			tbl->tabattr[tbl->row][tbl->col] |= HTT_NOWRAP;
#endif				/* NOWRAP */
		v = 0;
		if (parsedtag_get_value(tag, ATTR_WIDTH, &v)) {
#ifdef TABLE_EXPAND
			if (v > 0) {
				if (tbl->real_width > 0)
					v = -(v * 100) / (tbl->real_width * pixel_per_char);
				else
					v = (int) (v / pixel_per_char);
			}
#else
			v = RELATIVE_WIDTH(v);
#endif				/* not TABLE_EXPAND */
		}
#ifdef ID_EXT
		if (parsedtag_get_value(tag, ATTR_ID, &p))
			tbl->tabidvalue[tbl->row][tbl->col] = Strnew_charp(p);
#endif				/* ID_EXT */
#ifdef NOWRAP
		if (v != 0) {
			/* NOWRAP and WIDTH= conflicts each other */
			tbl->tabattr[tbl->row][tbl->col] &= ~HTT_NOWRAP;
		}
#endif				/* NOWRAP */
		tbl->tabattr[tbl->row][tbl->col] &= ~(HTT_ALIGN | HTT_VALIGN);
		tbl->tabattr[tbl->row][tbl->col] |= (halign | valign);
		if (colspan > 1) {
			col = tbl->col;

			cell->icell = cell->maxcell + 1;
			k = bsearch_2short(colspan, cell->colspan, col, cell->col, MAXCOL,
					   cell->index, cell->icell);
			if (k <= cell->maxcell) {
				i = cell->index[k];
				if (cell->col[i] == col && cell->colspan[i] == colspan)
					cell->icell = i;
			}
			if (cell->icell > cell->maxcell && cell->icell < MAXCELL) {
				cell->maxcell++;
				cell->col[cell->maxcell] = col;
				cell->colspan[cell->maxcell] = colspan;
				cell->width[cell->maxcell] = 0;
				cell->minimum_width[cell->maxcell] = 0;
				cell->fixed_width[cell->maxcell] = 0;
				if (cell->maxcell > k) {
					int ii;
					for (ii = cell->maxcell; ii > k; ii--)
						cell->index[ii] = cell->index[ii - 1];
				}
				cell->index[k] = cell->maxcell;
			}
			if (cell->icell > cell->maxcell)
				cell->icell = -1;
		}
		if (v != 0) {
			if (colspan == 1) {
				v0 = tbl->fixed_width[tbl->col];
				if (v0 == 0 || (v0 > 0 && v > v0) || (v0 < 0 && v < v0)) {
#ifdef FEED_TABLE_DEBUG
					fprintf(stderr, "width(%d) = %d\n", tbl->col, v);
#endif				/* TABLE_DEBUG */
					tbl->fixed_width[tbl->col] = v;
				}
			} else if (cell->icell >= 0) {
				v0 = cell->fixed_width[cell->icell];
				if (v0 == 0 || (v0 > 0 && v > v0) || (v0 < 0 && v < v0))
					cell->fixed_width[cell->icell] = v;
			}
		}
		for (i = 0; i < rowspan; i++) {
			check_row(tbl, tbl->row + i);
			for (j = 0; j < colspan; j++) {
#if 0
				tbl->tabattr[tbl->row + i][tbl->col + j] &= ~(HTT_X | HTT_Y);
#endif
				if (!(tbl->tabattr[tbl->row + i][tbl->col + j] &
				      (HTT_X | HTT_Y))) {
					tbl->tabattr[tbl->row + i][tbl->col + j] |=
						((i > 0) ? HTT_Y : 0) | ((j > 0) ? HTT_X : 0);
				}
				if (tbl->col + j > tbl->maxcol) {
					tbl->maxcol = tbl->col + j;
				}
			}
			if (tbl->row + i > tbl->maxrow) {
				tbl->maxrow = tbl->row + i;
			}
		}
		begin_cell(tbl, mode);
		break;
	case HTML_N_TR:
		setwidth(tbl, mode);
		tbl->col = -1;
		tbl->flag &= ~(TBL_IN_ROW | TBL_IN_COL);
		return TAG_ACTION_NONE;
	case HTML_N_TH:
	case HTML_N_TD:
		setwidth(tbl, mode);
		tbl->flag &= ~TBL_IN_COL;
#ifdef FEED_TABLE_DEBUG
		{
			TextListItem *it;
			int i = tbl->col, j = tbl->row;
			fprintf(stderr, "(a) row,col: %d, %d\n", j, i);
			if (tbl->tabdata[j] && tbl->tabdata[j][i]) {
				for (it = ((TextList *) tbl->tabdata[j][i])->first;
				     it; it = it->next)
					fprintf(stderr, "  [%s] \n", it->ptr);
			}
		}
#endif
		return TAG_ACTION_NONE;
	case HTML_P:
	case HTML_BR:
	case HTML_CENTER:
	case HTML_N_CENTER:
	case HTML_DIV:
	case HTML_N_DIV:
		if (!(tbl->flag & TBL_IN_ROW))
			break;
	case HTML_DT:
	case HTML_DD:
	case HTML_H:
	case HTML_N_H:
	case HTML_LI:
	case HTML_PRE:
	case HTML_N_PRE:
	case HTML_HR:
	case HTML_LISTING:
	case HTML_XMP:
	case HTML_PLAINTEXT:
	case HTML_PRE_PLAIN:
	case HTML_N_PRE_PLAIN:
		feed_table_block_tag(tbl, line, mode, 0, cmd);
		switch (cmd) {
		case HTML_PRE:
		case HTML_PRE_PLAIN:
			mode->pre_mode |= TBLM_PRE;
			break;
		case HTML_N_PRE:
		case HTML_N_PRE_PLAIN:
			mode->pre_mode &= ~TBLM_PRE;
			break;
		case HTML_LISTING:
			mode->pre_mode |= TBLM_PLAIN;
			mode->end_tag = HTML_N_LISTING;
			break;
		case HTML_XMP:
			mode->pre_mode |= TBLM_PLAIN;
			mode->end_tag = HTML_N_XMP;
			break;
		case HTML_PLAINTEXT:
			mode->pre_mode |= TBLM_PLAIN;
			mode->end_tag = MAX_HTMLTAG;
			break;
		}
		break;
	case HTML_DL:
	case HTML_BLQ:
	case HTML_OL:
	case HTML_UL:
		feed_table_block_tag(tbl, line, mode, 1, cmd);
		break;
	case HTML_N_DL:
	case HTML_N_BLQ:
	case HTML_N_OL:
	case HTML_N_UL:
		feed_table_block_tag(tbl, line, mode, -1, cmd);
		break;
	case HTML_NOBR:
	case HTML_WBR:
		if (!(tbl->flag & TBL_IN_ROW))
			break;
	case HTML_PRE_INT:
		feed_table_inline_tag(tbl, line, mode, -1);
		switch (cmd) {
		case HTML_NOBR:
			mode->nobr_level++;
			if (mode->pre_mode & TBLM_NOBR)
				return TAG_ACTION_NONE;
			mode->pre_mode |= TBLM_NOBR;
			break;
		case HTML_PRE_INT:
			if (mode->pre_mode & TBLM_PRE_INT)
				return TAG_ACTION_NONE;
			mode->pre_mode |= TBLM_PRE_INT;
			tbl->linfo.prev_spaces = 0;
			break;
		}
		mode->nobr_offset = -1;
		if (tbl->linfo.length > 0) {
			check_minimum0(tbl, tbl->linfo.length);
			tbl->linfo.length = 0;
		}
		break;
	case HTML_N_NOBR:
		if (!(tbl->flag & TBL_IN_ROW))
			break;
		feed_table_inline_tag(tbl, line, mode, -1);
		if (mode->nobr_level > 0)
			mode->nobr_level--;
		if (mode->nobr_level == 0)
			mode->pre_mode &= ~TBLM_NOBR;
		break;
	case HTML_N_PRE_INT:
		feed_table_inline_tag(tbl, line, mode, -1);
		mode->pre_mode &= ~TBLM_PRE_INT;
		break;
	case HTML_IMG:
		check_rowcol(tbl, mode);
		w = tbl->fixed_width[tbl->col];
		if (w < 0) {
			if (tbl->total_width > 0)
				w = -tbl->total_width * w / 100;
			else if (width > 0)
				w = -width * w / 100;
			else
				w = 0;
		} else if (w == 0) {
			if (tbl->total_width > 0)
				w = tbl->total_width;
			else if (width > 0)
				w = width;
		}
		tok = process_img(tag, w);
		feed_table1(tbl, tok, mode, width);
		break;
	case HTML_FORM:
		feed_table_block_tag(tbl, "", mode, 0, cmd);
		tmp = process_form(tag);
		if (tmp)
			feed_table1(tbl, tmp, mode, width);
		break;
	case HTML_N_FORM:
		feed_table_block_tag(tbl, "", mode, 0, cmd);
		process_n_form();
		break;
	case HTML_INPUT:
		tmp = process_input(tag);
		feed_table1(tbl, tmp, mode, width);
		break;
	case HTML_SELECT:
		tmp = process_select(tag);
		if (tmp)
			feed_table1(tbl, tmp, mode, width);
		mode->pre_mode |= TBLM_INSELECT;
		mode->end_tag = HTML_N_SELECT;
		break;
	case HTML_N_SELECT:
	case HTML_OPTION:
		/* nothing */
		break;
	case HTML_TEXTAREA:
		w = 0;
		check_rowcol(tbl, mode);
		if (tbl->col + 1 <= tbl->maxcol &&
		    tbl->tabattr[tbl->row][tbl->col + 1] & HTT_X) {
			if (cell->icell >= 0 && cell->fixed_width[cell->icell] > 0)
				w = cell->fixed_width[cell->icell];
		} else {
			if (tbl->fixed_width[tbl->col] > 0)
				w = tbl->fixed_width[tbl->col];
		}
		tmp = process_textarea(tag, w);
		if (tmp)
			feed_table1(tbl, tmp, mode, width);
		mode->pre_mode |= TBLM_INTXTA;
		mode->end_tag = HTML_N_TEXTAREA;
		break;
	case HTML_A:
		table_close_anchor0(tbl, mode);
		anchor = NULL;
		i = 0;
		parsedtag_get_value(tag, ATTR_HREF, &anchor);
		parsedtag_get_value(tag, ATTR_HSEQ, &i);
		if (anchor) {
			check_rowcol(tbl, mode);
			if (i == 0) {
				Str atmp = process_anchor(tag, line);
				if (displayLinkNumber) {
					Str t = getLinkNumberStr(-1);
					feed_table_inline_tag(tbl, NULL, mode, t->length);
					Strcat(atmp, t);
				}
				pushdata(tbl, tbl->row, tbl->col, atmp->ptr);
			} else
				pushdata(tbl, tbl->row, tbl->col, line);
			if (i >= 0) {
				mode->pre_mode |= TBLM_ANCHOR;
				mode->anchor_offset = tbl->tabcontentssize;
			}
		} else
			suspend_or_pushdata(tbl, line);
		break;
	case HTML_DEL:
		switch (displayInsDel) {
		case DISPLAY_INS_DEL_SIMPLE:
			mode->pre_mode |= TBLM_DEL;
			break;
		case DISPLAY_INS_DEL_NORMAL:
			feed_table_inline_tag(tbl, line, mode, 5);	/* [DEL: */
			break;
		case DISPLAY_INS_DEL_FONTIFY:
			feed_table_inline_tag(tbl, line, mode, -1);
			break;
		}
		break;
	case HTML_N_DEL:
		switch (displayInsDel) {
		case DISPLAY_INS_DEL_SIMPLE:
			mode->pre_mode &= ~TBLM_DEL;
			break;
		case DISPLAY_INS_DEL_NORMAL:
			feed_table_inline_tag(tbl, line, mode, 5);	/* :DEL] */
			break;
		case DISPLAY_INS_DEL_FONTIFY:
			feed_table_inline_tag(tbl, line, mode, -1);
			break;
		}
		break;
	case HTML_S:
		switch (displayInsDel) {
		case DISPLAY_INS_DEL_SIMPLE:
			mode->pre_mode |= TBLM_S;
			break;
		case DISPLAY_INS_DEL_NORMAL:
			feed_table_inline_tag(tbl, line, mode, 3);	/* [S: */
			break;
		case DISPLAY_INS_DEL_FONTIFY:
			feed_table_inline_tag(tbl, line, mode, -1);
			break;
		}
		break;
	case HTML_N_S:
		switch (displayInsDel) {
		case DISPLAY_INS_DEL_SIMPLE:
			mode->pre_mode &= ~TBLM_S;
			break;
		case DISPLAY_INS_DEL_NORMAL:
			feed_table_inline_tag(tbl, line, mode, 3);	/* :S] */
			break;
		case DISPLAY_INS_DEL_FONTIFY:
			feed_table_inline_tag(tbl, line, mode, -1);
			break;
		}
		break;
	case HTML_INS:
	case HTML_N_INS:
		switch (displayInsDel) {
		case DISPLAY_INS_DEL_SIMPLE:
			break;
		case DISPLAY_INS_DEL_NORMAL:
			feed_table_inline_tag(tbl, line, mode, 5);	/* [INS:, :INS] */
			break;
		case DISPLAY_INS_DEL_FONTIFY:
			feed_table_inline_tag(tbl, line, mode, -1);
			break;
		}
		break;
	case HTML_SUP:
	case HTML_SUB:
	case HTML_N_SUB:
		if (!(mode->pre_mode & (TBLM_DEL | TBLM_S)))
			feed_table_inline_tag(tbl, line, mode, 1);	/* ^, [, ] */
		break;
	case HTML_N_SUP:
		break;
	case HTML_TABLE_ALT:
		id = -1;
		w = 0;
		parsedtag_get_value(tag, ATTR_TID, &id);
		if (id >= 0 && id < tbl->ntable) {
			struct table *tbl1 = tbl->tables[id].ptr;
			feed_table_block_tag(tbl, line, mode, 0, cmd);
			addcontentssize(tbl, maximum_table_width(tbl1));
			check_minimum0(tbl, tbl1->sloppy_width);
#ifdef TABLE_EXPAND
			w = tbl1->total_width;
			v = 0;
			colspan = table_colspan(tbl, tbl->row, tbl->col);
			if (colspan > 1) {
				if (cell->icell >= 0)
					v = cell->fixed_width[cell->icell];
			} else
				v = tbl->fixed_width[tbl->col];
			if (v < 0 && tbl->real_width > 0 && tbl1->real_width > 0)
				w = -(tbl1->real_width * 100) / tbl->real_width;
			else
				w = tbl1->real_width;
			if (w > 0)
				check_minimum0(tbl, w);
			else if (w < 0 && v < w) {
				if (colspan > 1) {
					if (cell->icell >= 0)
						cell->fixed_width[cell->icell] = w;
				} else
					tbl->fixed_width[tbl->col] = w;
			}
#endif
			setwidth0(tbl, mode);
			clearcontentssize(tbl, mode);
		}
		break;
	case HTML_CAPTION:
		mode->caption = 1;
		break;
	case HTML_N_CAPTION:
	case HTML_THEAD:
	case HTML_N_THEAD:
	case HTML_TBODY:
	case HTML_N_TBODY:
	case HTML_TFOOT:
	case HTML_N_TFOOT:
	case HTML_COLGROUP:
	case HTML_N_COLGROUP:
	case HTML_COL:
		break;
	case HTML_SCRIPT:
		mode->pre_mode |= TBLM_SCRIPT;
		mode->end_tag = HTML_N_SCRIPT;
		break;
	case HTML_STYLE:
		mode->pre_mode |= TBLM_STYLE;
		mode->end_tag = HTML_N_STYLE;
		break;
	case HTML_N_A:
		table_close_anchor0(tbl, mode);
	case HTML_FONT:
	case HTML_N_FONT:
	case HTML_NOP:
		suspend_or_pushdata(tbl, line);
		break;
	case HTML_INTERNAL:
	case HTML_N_INTERNAL:
	case HTML_FORM_INT:
	case HTML_N_FORM_INT:
	case HTML_INPUT_ALT:
	case HTML_N_INPUT_ALT:
	case HTML_SELECT_INT:
	case HTML_N_SELECT_INT:
	case HTML_OPTION_INT:
	case HTML_TEXTAREA_INT:
	case HTML_N_TEXTAREA_INT:
	case HTML_IMG_ALT:
	case HTML_SYMBOL:
	case HTML_N_SYMBOL:
	default:
		/* unknown tag: put into table */
		return TAG_ACTION_FEED;
	}
	return TAG_ACTION_NONE;
}


int
feed_table(struct table * tbl, char *line, struct table_mode * mode,
	   int width, int internal)
{
	int i;
	char *p;
	Str tmp;
	struct table_linfo *linfo = &tbl->linfo;

	if (mode == NULL) {
		return -1;
	}

	if (*line == '<' && line[1] && REALLY_THE_BEGINNING_OF_A_TAG(line)) {
		struct parsed_tag *tag;
		p = line;
		tag = parse_tag(&p, internal);
		if (tag) {
			switch (feed_table_tag(tbl, line, mode, width, tag)) {
			case TAG_ACTION_NONE:
				return -1;
			case TAG_ACTION_N_TABLE:
				return 0;
			case TAG_ACTION_TABLE:
				return 1;
			case TAG_ACTION_PLAIN:
				break;
			case TAG_ACTION_FEED:
			default:
				if (parsedtag_need_reconstruct(tag))
					line = parsedtag2str(tag)->ptr;
			}
		} else {
			if (!(mode->pre_mode & (TBLM_PLAIN | TBLM_INTXTA | TBLM_INSELECT |
						TBLM_SCRIPT | TBLM_STYLE)))
				return -1;
		}
	} else {
		if (mode->pre_mode & (TBLM_DEL | TBLM_S))
			return -1;
	}
	if (mode->caption) {
		Strcat_charp(tbl->caption, line);
		return -1;
	}
	if (mode->pre_mode & TBLM_SCRIPT)
		return -1;
	if (mode->pre_mode & TBLM_STYLE)
		return -1;
	if (mode->pre_mode & TBLM_INTXTA) {
		feed_textarea(line);
		return -1;
	}
	if (mode->pre_mode & TBLM_INSELECT) {
		feed_select(line);
		return -1;
	}
	if (!(mode->pre_mode & TBLM_PLAIN) &&
	    !(*line == '<' && line[strlen(line) - 1] == '>') &&
	    strchr(line, '&') != NULL) {
		tmp = Strnew();
		for (p = line; *p;) {
			char *q, *r;
			if (*p == '&') {
				if (!strncasecmp(p, "&amp;", 5) ||
				    !strncasecmp(p, "&gt;", 4) || !strncasecmp(p, "&lt;", 4)) {
					/* do not convert */
					Strcat_char(tmp, *p);
					p++;
				} else {
					int ec;
					q = p;
					switch (ec = getescapechar(&p)) {
					case '<':
						Strcat_charp(tmp, "&lt;");
						break;
					case '>':
						Strcat_charp(tmp, "&gt;");
						break;
					case '&':
						Strcat_charp(tmp, "&amp;");
						break;
					case '\r':
						Strcat_char(tmp, '\n');
						break;
					default:
						r = conv_entity(ec);
						if (r != NULL && strlen(r) == 1 &&
						 ec == (unsigned char) *r) {
							Strcat_char(tmp, *r);
							break;
						}
					case -1:
						Strcat_char(tmp, *q);
						p = q + 1;
						break;
					}
				}
			} else {
				Strcat_char(tmp, *p);
				p++;
			}
		}
		line = tmp->ptr;
	}
	if (!(mode->pre_mode & (TBLM_SPECIAL & ~TBLM_NOBR))) {
		if (!(tbl->flag & TBL_IN_COL) || linfo->prev_spaces != 0)
			while (isspace((unsigned char)*line))
				line++;
		if (*line == '\0')
			return -1;
		check_rowcol(tbl, mode);
		if (mode->pre_mode & TBLM_NOBR && mode->nobr_offset < 0)
			mode->nobr_offset = tbl->tabcontentssize;

		/* count of number of spaces skipped in normal mode */
		i = skip_space(tbl, line, linfo, !(mode->pre_mode & TBLM_NOBR));
		addcontentssize(tbl, visible_length(line) - i);
		setwidth(tbl, mode);
		pushdata(tbl, tbl->row, tbl->col, line);
	} else if (mode->pre_mode & TBLM_PRE_INT) {
		check_rowcol(tbl, mode);
		if (mode->nobr_offset < 0)
			mode->nobr_offset = tbl->tabcontentssize;
		addcontentssize(tbl, maximum_visible_length(line, tbl->tabcontentssize));
		setwidth(tbl, mode);
		pushdata(tbl, tbl->row, tbl->col, line);
	} else {
		/* <pre> mode or something like it */
		check_rowcol(tbl, mode);
		while (*line) {
			int nl = FALSE;
			if ((p = strchr(line, '\r')) || (p = strchr(line, '\n'))) {
				if (*p == '\r' && p[1] == '\n')
					p++;
				if (p[1]) {
					p++;
					tmp = Strnew_charp_n(line, p - line);
					line = p;
					p = tmp->ptr;
				} else {
					p = line;
					line = "";
				}
				nl = TRUE;
			} else {
				p = line;
				line = "";
			}
			if (mode->pre_mode & TBLM_PLAIN)
				i = maximum_visible_length_plain(p, tbl->tabcontentssize);
			else
				i = maximum_visible_length(p, tbl->tabcontentssize);
			addcontentssize(tbl, i);
			setwidth(tbl, mode);
			if (nl)
				clearcontentssize(tbl, mode);
			pushdata(tbl, tbl->row, tbl->col, p);
		}
	}
	return -1;
}

void
feed_table1(struct table * tbl, Str tok, struct table_mode * mode, int width)
{
	Str tokbuf;
	enum RTstatus status;
	char *line;
	if (!tok)
		return;
	tokbuf = Strnew();
	status = R_ST_NORMAL;
	line = tok->ptr;
	while (read_token
	       (tokbuf, &line, &status, mode->pre_mode & TBLM_PREMODE, 0))
		feed_table(tbl, tokbuf->ptr, mode, width, TRUE);
}

void
pushTable(struct table * tbl, struct table * tbl1)
{
	int col;
	int row;

	col = tbl->col;
	row = tbl->row;

	if (tbl->ntable >= tbl->tables_size) {
		struct table_in *tmp;
		tbl->tables_size += MAX_TABLE_N;
		tmp = New_N(struct table_in, tbl->tables_size);
		if (tbl->tables) {
			memcpy(tmp, tbl->tables,
			    tbl->ntable * sizeof(struct table_in));
		}
		tbl->tables = tmp;
	}
	tbl->tables[tbl->ntable].ptr = tbl1;
	tbl->tables[tbl->ntable].col = col;
	tbl->tables[tbl->ntable].row = row;
	tbl->tables[tbl->ntable].indent = tbl->indent;
	tbl->tables[tbl->ntable].buf = newTextLineList();
	check_row(tbl, row);
	if (col + 1 <= tbl->maxcol && tbl->tabattr[row][col + 1] & HTT_X)
		tbl->tables[tbl->ntable].cell = tbl->cell.icell;
	else
		tbl->tables[tbl->ntable].cell = -1;
	tbl->ntable++;
}

/* Local Variables:    */
/* c-basic-offset: 4   */
/* tab-width: 8        */
/* End:                */
