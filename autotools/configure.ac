AC_INIT([hello],[0.01])
# Force autoconf to be atleast this version number:
AC_PREREQ([2.69])
# Safety check - list a source file that wouldn't be in other directories:
AC_CONFIG_SRCDIR([src/hello.c])
# Put configuration results here, so we can easily #include them:
AC_CONFIG_HEADERS([config.h])
# Put autotools auxiliary files in a subdir, so they don't clutter top dir:
AC_CONFIG_AUX_DIR([build-aux])

# Enable "automake" to simplify creating makefiles:
AM_INIT_AUTOMAKE([1.11 subdir-objects -Wall -Werror])
AC_CONFIG_FILES([
        Makefile
        src/Makefile
        src2/Makefile
        ])

# Look for a C compiler:
AC_PROG_CC

# Perhaps check for: (more) programs, libbraries, header  files, types, 
# structures, compiler characteristics, library functions, system services.

# Do final output.
AC_OUTPUT
