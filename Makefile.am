bin_PROGRAMS = x-on-resize

AM_CFLAGS = $(XONRESIZE_CFLAGS) $(CWARNFLAGS)

x_on_resize_LDADD = $(XONRESIZE_LIBS)

x_on_resize_SOURCES =		\
	x-on-resize.c

MAINTAINERCLEANFILES = ChangeLog INSTALL

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL
