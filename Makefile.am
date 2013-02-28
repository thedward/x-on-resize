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


appmandir = $(APP_MAN_DIR)
appman_PRE = x-on-resize.man
appman_DATA = $(appman_PRE:man=$(APP_MAN_SUFFIX))

EXTRA_DIST = $(appman_PRE)
CLEANFILES = $(appman_DATA)
SUFFIXES = .$(APP_MAN_SUFFIX) .man

# String replacements in MAN_SUBSTS now come from xorg-macros.m4 via configure
.man.$(APP_MAN_SUFFIX):
	$(AM_V_GEN)$(SED) $(MAN_SUBSTS) < $< > $@
