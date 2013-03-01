AC_PREREQ([2.60])
AC_INIT([x-on-resize], [0.2],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], [x-on-resize])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Checks for pkg-config packages
PKG_CHECK_MODULES(XONRESIZE, xrandr x11)

AC_CONFIG_FILES([
	Makefile])
AC_OUTPUT
