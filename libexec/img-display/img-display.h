/* $Id: w3mimg.h,v 1.9 2010/12/21 10:13:55 htrb Exp $ */
#ifndef W3MIMG_H
#define W3MIMG_H

typedef struct {
	void *pixmap;	/* driver specific */
	int width;
	int height;
} W3MImage;

typedef struct _w3mimg_op {
	void *priv;	/* driver specific data */
	int width, height;	/* window width, height */
	int offset_x, offset_y;	/* offset */
	int clear_margin;
	int max_anim;

	int (*init) (struct _w3mimg_op * self);
	int (*finish) (struct _w3mimg_op * self);
	int (*active) (struct _w3mimg_op * self);
	void (*set_background) (struct _w3mimg_op * self, char *background);
	void (*sync) (struct _w3mimg_op * self);
	void (*close) (struct _w3mimg_op * self);

	int (*load_image) (struct _w3mimg_op * self, W3MImage * img,
	    char *fname, int w, int h);

	int (*show_image) (struct _w3mimg_op * self, W3MImage * img,
	    int sx, int sy, int sw, int sh, int x, int y);

	void (*free_image) (struct _w3mimg_op * self, W3MImage * img);

	int (*get_image_size) (struct _w3mimg_op * self, W3MImage * img,
	    char *fname, int *w, int *h);

	int (*clear) (struct _w3mimg_op * self, int x, int y, int w, int h);
} w3mimg_op;

extern w3mimg_op *w3mimg_open(void);

#endif
