/* $Id: x11_w3mimg.c,v 1.29 2004/11/08 17:14:06 ukai Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>

#include "img-display.h"

#define OFFSET_X	2
#define OFFSET_Y	2

struct x11_info {
	Display *display;
	Window window, parent;
	unsigned long background_pixel;
	GC imageGC;
};

static int
x11_init(w3mimg_op * self)
{
	struct x11_info *xi;
	if (self == NULL)
		return 0;
	xi = (struct x11_info *) self->priv;
	if (xi == NULL)
		return 0;
	if (!xi->imageGC) {
		xi->imageGC = XCreateGC(xi->display, xi->parent, 0, NULL);
		if (!xi->imageGC)
			return 0;
	}
	return 1;
}

static int
x11_finish(w3mimg_op * self)
{
	struct x11_info *xi;
	if (self == NULL)
		return 0;
	xi = (struct x11_info *) self->priv;
	if (xi == NULL)
		return 0;
	if (xi->imageGC) {
		XFreeGC(xi->display, xi->imageGC);
		xi->imageGC = NULL;
	}
	return 1;
}

static int
x11_clear(w3mimg_op * self, int x, int y, int w, int h)
{
	struct x11_info *xi;
	if (self == NULL)
		return 0;
	xi = (struct x11_info *) self->priv;
	if (xi == NULL)
		return 0;

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	XClearArea(xi->display, xi->window, x, y, w, h, False);
	return 1;
}

static int
x11_active(w3mimg_op * self)
{
	struct x11_info *xi;
	if (self == NULL)
		return 0;
	xi = (struct x11_info *) self->priv;
	if (xi == NULL)
		return 0;
	if (!xi->imageGC)
		return 0;
	return 1;
}

static void
x11_set_background(w3mimg_op * self, char *background)
{
	XColor screen_def, exact_def;
	struct x11_info *xi;
	if (self == NULL)
		return;
	xi = (struct x11_info *) self->priv;
	if (xi == NULL)
		return;

	if (background &&
	    XAllocNamedColor(xi->display, DefaultColormap(xi->display, 0),
			     background, &screen_def, &exact_def))
		xi->background_pixel = screen_def.pixel;
	else {
		Pixmap p;
		GC gc;
		XImage *i;

		p = XCreatePixmap(xi->display, xi->window, 1, 1,
				  DefaultDepth(xi->display, 0));
		gc = XCreateGC(xi->display, xi->window, 0, NULL);
		if (!p || !gc)
			exit(1);/* XXX */
		XCopyArea(xi->display, xi->window, p, gc,
			  (self->offset_x >= 1) ? (self->offset_x - 1) : 0,
			  (self->offset_y >= 1) ? (self->offset_y - 1) : 0,
			  1, 1, 0, 0);
		i = XGetImage(xi->display, p, 0, 0, 1, 1, -1, ZPixmap);
		if (!i)
			exit(1);
		xi->background_pixel = XGetPixel(i, 0, 0);
		XDestroyImage(i);
		XFreeGC(xi->display, gc);
		XFreePixmap(xi->display, p);
	}
}

static void
x11_sync(w3mimg_op * self)
{
	struct x11_info *xi;
	if (self == NULL)
		return;
	xi = (struct x11_info *) self->priv;
	if (xi == NULL)
		return;
	XSync(xi->display, False);
}

static void
x11_close(w3mimg_op * self)
{
	/* XCloseDisplay(xi->display); */
}

static int
x11_load_image(w3mimg_op * self, W3MImage * img, char *fname, int w, int h)
{
	struct x11_info *xi;
	Imlib_Image im;
	if (self == NULL)
		return 0;
	xi = (struct x11_info *) self->priv;
	if (xi == NULL)
		return 0;

	im = imlib_load_image(fname);
	if (!im)
		return 0;
	imlib_context_set_image(im);
	if (w <= 0)
		w = imlib_image_get_width();
	if (h <= 0)
		h = imlib_image_get_height();
	img->pixmap = (void *) XCreatePixmap(xi->display, xi->parent, w, h,
					     DefaultDepth(xi->display, 0));
	if (!img->pixmap)
		return 0;
	XSetForeground(xi->display, xi->imageGC, xi->background_pixel);
	XFillRectangle(xi->display, (Pixmap) img->pixmap, xi->imageGC, 0, 0, w, h);
	imlib_context_set_display(xi->display);
	imlib_context_set_visual(DefaultVisual(xi->display, 0));
	imlib_context_set_colormap(DefaultColormap(xi->display, 0));
	imlib_context_set_drawable((Drawable) img->pixmap);
	imlib_render_image_on_drawable_at_size(0, 0, w, h);
	imlib_free_image();

	img->width = w;
	img->height = h;
	return 1;
}

static int
x11_show_image(w3mimg_op * self, W3MImage * img, int sx, int sy, int sw,
	       int sh, int x, int y)
{
	struct x11_info *xi;
	if (self == NULL)
		return 0;

	if (img->pixmap == NULL)
		return 0;

	xi = (struct x11_info *) self->priv;
	if (xi == NULL)
		return 0;

	XCopyArea(xi->display, (Pixmap) img->pixmap, xi->window, xi->imageGC,
		  sx, sy,
		  (sw ? sw : img->width),
	   (sh ? sh : img->height), x + self->offset_x, y + self->offset_y);
	return 1;
}

static void
x11_free_image(w3mimg_op * self, W3MImage * img)
{
	struct x11_info *xi;
	if (self == NULL)
		return;
	xi = (struct x11_info *) self->priv;
	if (xi == NULL)
		return;
	if (img && img->pixmap) {
		XFreePixmap(xi->display, (Pixmap) img->pixmap);
		img->pixmap = NULL;
		img->width = 0;
		img->height = 0;
	}
}

static int
x11_get_image_size(w3mimg_op * self, W3MImage * img, char *fname, int *w,
		   int *h)
{
	struct x11_info *xi;
	Imlib_Image im;

	if (self == NULL)
		return 0;
	xi = (struct x11_info *) self->priv;
	if (xi == NULL)
		return 0;

	im = imlib_load_image(fname);
	if (im == NULL)
		return 0;

	imlib_context_set_image(im);
	*w = imlib_image_get_width();
	*h = imlib_image_get_height();
	imlib_free_image();
	return 1;
}

/* *INDENT-OFF* */
/*
  xterm/kterm/hanterm/cxterm
    top window (WINDOWID)
      +- text window
           +- scrollbar
  rxvt/aterm/Eterm/wterm
    top window (WINDOWID)
      +- text window
      +- scrollbar
      +- menubar (etc.)
  gnome-terminal
    top window
      +- text window (WINDOWID)
      +- scrollbar
      +- menubar
  mlterm (-s)
    top window
      +- text window (WINDOWID)
      +- scrollbar
  mlterm
    top window = text window (WINDOWID)

  powershell
    top window
      +- window
      |    +- text window
      |    +- scrollbar
      +- menubar (etc.)
  dtterm
    top window
      +- window
           +- window
           |    +- window
           |         +- text window
           |         +- scrollbar
           +- menubar
  hpterm
    top window
      +- window
           +- text window
           +- scrollbar
           +- (etc.)
*/
/* *INDENT-ON* */

w3mimg_op *
w3mimg_x11open()
{
	w3mimg_op *wop = NULL;
	struct x11_info *xi = NULL;
	char *id;
	int revert, i;
	unsigned int nchildren;
	XWindowAttributes attr;
	Window root, *children;

	wop = (w3mimg_op *) calloc(1, sizeof(w3mimg_op));
	if (wop == NULL)
		return NULL;

	xi = (struct x11_info *) calloc(1, sizeof(struct x11_info));
	if (xi == NULL)
		goto error;

	xi->display = XOpenDisplay(NULL);
	if (xi->display == NULL) {
		goto error;
	}
	if ((id = getenv("WINDOWID")) != NULL)
		xi->window = (Window) atoi(id);
	else
		XGetInputFocus(xi->display, &xi->window, &revert);
	if (!xi->window)
		exit(1);

	XGetWindowAttributes(xi->display, xi->window, &attr);
	wop->width = attr.width;
	wop->height = attr.height;

	while (1) {
		Window p_window;

		XQueryTree(xi->display, xi->window, &root, &xi->parent,
			   &children, &nchildren);
		p_window = xi->window;
		for (i = 0; i < nchildren; i++) {
			XGetWindowAttributes(xi->display, children[i], &attr);
			if (attr.width > wop->width * 0.7 &&
			    attr.height > wop->height * 0.7) {
				/* maybe text window */
				wop->width = attr.width;
				wop->height = attr.height;
				xi->window = children[i];
			}
		}
		if (p_window == xi->window)
			break;
	}
	wop->offset_x = OFFSET_X;
	for (i = 0; i < nchildren; i++) {
		XGetWindowAttributes(xi->display, children[i], &attr);
		if (attr.x <= 0 && attr.width < 30 && attr.height > wop->height * 0.7) {
			/* scrollbar of xterm/kterm ? */
			wop->offset_x += attr.x + attr.width + attr.border_width * 2;
			break;
		}
	}
	wop->offset_y = OFFSET_Y;

	wop->priv = xi;

	wop->init = x11_init;
	wop->finish = x11_finish;
	wop->active = x11_active;
	wop->set_background = x11_set_background;
	wop->sync = x11_sync;
	wop->close = x11_close;
	wop->clear = x11_clear;

	wop->load_image = x11_load_image;
	wop->show_image = x11_show_image;
	wop->free_image = x11_free_image;
	wop->get_image_size = x11_get_image_size;

	return wop;
error:
	free(xi);
	free(wop);
	return NULL;
}

w3mimg_op *
w3mimg_open()
{
	w3mimg_op *w_op = NULL;
	w_op = w3mimg_x11open();
	return w_op;
}
