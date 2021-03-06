w3s: terminal-based web browser & pager
=======================================

This repository is for the continued development of the w3m web
browser and pager, since upstream seems to have died down. It is
based on w3m 0.5.3, released 2011-01-15.

w3s is ideal for browsing content-focused websites where the
clutter displayed by graphical web browsers gets in the way of
the actual content.

It currently runs on OpenBSD, and compiles using BSD make.

Features
--------
* Protocols: HTTPS, HTTP, FTP, Gopher
* Renders tables, hyperlinks, forms, lists, horizontal rules,
headings, images (in xterm, using imlib2).
* vi(1)-like control scheme
* Support for multiple character encodings: UTF-8, Shift\_JIS,
Big5, KOI8-R, etc (see charset-list)
* and cookies.

Changes since 0.5.3
-------------------
* NULL pointer dereference and buffer overflow bugs fixed.
* Format string fixes (including Y2038 fixes).
* Inserted checks for integer overflow in allocation functions.
* Use asprintf() to implement gc-based Sprintf() instead of
hand-rolled evilness.
* Thousands of lines of code removed:
	* Support for old revisions of POSIX and DOS is gone.
	* Usenet (NNTP) support has been removed.
	* Improperly licensed MATRIX table rendering code that wasn't
	doing anything useful beyond the non-MATRIX code.
	* Unused character encodings (UTF-7, Johab...)
	* Replacements for standard interfaces (curses, C library stuff...)
* Use modern C features (size\_t, const, enums, snprintf, memcpy 
instead of bcopy, etc).
* Formatted the code to a more readable style where every block
is indented.
* Improved Gopher support.

## Building and installing

#### OpenBSD

	# pkg_add boehm-gc imlib2
	# make
	# make install
	$ w3s https://en.wikipedia.org

Additionally, it's possible to build without m17n and/or image
support:

	# pkg_add boehm-gc
	# USE_IMAGE=no USE_M17N=no make
	# make install
	$ w3s https://en.wikipedia.org

#### Linux
Under Linux based distributions you can should use CMake to build
this program.

    $ sudo apt install libgc-dev libbsd-dev libimlib2-dev
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ make install
    $ w3s https://en.wikipedia.org
