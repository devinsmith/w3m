w3m: terminal-based web browser & pager
=======================================

This repository is for the continued development of the w3m web
browser and pager, since upstream seems to have died down. It is
based on w3m 0.5.3, released 2011-01-15.

w3m is ideal for browsing content-focused websites where the
clutter displayed by graphical web browsers gets in the way of
the actual content.

It currently runs on OpenBSD, and compiles using BSD make.

Features
--------
* Protocols: HTTPS, HTTP, FTP, Gopher
* Renders tables, hyperlinks, forms, lists, horizontal rules,
headings, images (in xterm, using imlib2).
* vi(1)-like control scheme
* Support for multiple character encodings: UTF-8, Shift_JIS,
Big5, KOI8-R, etc (see charset-list)
* and cookies.

Changes since 0.5.3
-------------------
* Use libtls from [libressl](http://libressl.org) to implement
Transport Layer Security (HTTPS) cleanly.
* NULL pointer dereference and buffer overflow bugs fixed.
* Format string fixes (including Y2038 fixes).
* Inserted checks for integer overflow in allocation functions.
* Use asprintf() to implement gc-based Sprintf() instead of
hand-rolled evilness.
* Thousands of lines of codes removed:
 * Support for old revisions of POSIX and DOS are gone.
 * Usenet (NNTP) support has been removed.
 * Improperly licensed MATRIX table rendering code that wasn't
 doing anything useful beyond the non-MATRIX code.
 * Unused character encodings (UTF-7, Johab...)
 * Replacements for standard interfaces (curses, C library stuff...)
* Use modern C features (size_t, const, enums, snprintf, memcpy 
instead of bcopy, etc).
* Formatted the code to a more readable style where every block
is indented.
* Improved Gopher support.

Work that still needs to be done
--------------------------------
* Privilege separation: decode images and do other nasty
stuff in a jail.
* Lots of compiler warnings need to be fixed.
* Remove and clean up more code!

Building and installing
-----------------------
	# pkg_add boehm-gc imlib2
	# make
	# make install
	$ w3m https://en.wikipedia.org

Additionally, it's possible to build without m17n and/or image
support:

	# pkg_add boehm-gc
	# USE_IMAGE=no USE_M17N=no make
	# make install
	$ w3m https://en.wikipedia.org
