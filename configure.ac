# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([spa],[m4_esyscmd(./git-version)],[zack@buhman.org],[https://github.com/buhman/spa])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign 1.11 -Wall -Wno-portability subdir-objects silent-rules color-tests dist-xz tar-ustar])

AS_IF([! test -n "$VERSION"], [
         AC_MSG_ERROR([git-version failed])
])

AC_SUBST([ARK_VERSION], $VERSION)

AC_CANONICAL_HOST
AC_DEFINE_UNQUOTED([CANONICAL_HOST], "$host", [Canonical host string.])
AC_DEFINE([SCALE], [2], [Game object scale.])
AC_DEFINE([UI_SCALE], [2], [User interface scale.])
AC_DEFINE([ASPECT], [1], [Aspect ratio.])
AM_SILENT_RULES([yes])

## checks ###

# CC

AC_PROG_CC
AC_PROG_CC_C99
AC_GNU_SOURCE

# pkg-config

PKG_PROG_PKG_CONFIG

#### compiler flags ####

AX_APPEND_COMPILE_FLAGS(
        [-Wall -W -Wextra -pipe -Wno-long-long -Wno-overlength-strings -Wunsafe-loop-optimizations -Wundef -Wlogical-op -Wsign-compare -Wmissing-include-dirs -Wold-style-definition -Wpointer-arith -Winit-self -Wdeclaration-after-statement -Wfloat-equal -Wmissing-prototypes -Wstrict-prototypes -Wredundant-decls -Wmissing-declarations -Wmissing-noreturn -Wshadow -Wendif-labels -Wcast-align -Wstrict-aliasing -Wwrite-strings -Wno-unused-parameter -Wno-error=unused-function -ffast-math -fno-common -fdiagnostics-show-option],
        [], [-pedantic -pedantic-errors -Werror])

#### environment ####

AC_HEADER_STDC

PKG_CHECK_MODULES(ALLEGRO, [allegro-5])
PKG_CHECK_MODULES(ALLEGRO_FONT, [allegro_font-5])
PKG_CHECK_MODULES(ALLEGRO_TTF, [allegro_ttf-5])
PKG_CHECK_MODULES(ALLEGRO_PRIMITIVES, [allegro_primitives-5])
PKG_CHECK_MODULES(FONTCONFIG, [fontconfig])

AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT
