AC_PREREQ(2.61)
AC_INIT([seven], [0.1], [depp@zdome.net])
AC_CONFIG_SRCDIR([src/screen.cpp])
AC_CONFIG_FILES([Makefile])
AM_INIT_AUTOMAKE

AC_PROG_CC
AX_CXX_COMPILE_STDCXX_11
ENABLE_WARNINGS

PKG_CHECK_EXISTS([glfw3], [
  GLFW_CFLAGS=`$PKG_CONFIG --cflags glfw3`
  GLFW_LIBS=`$PKG_CONFIG --static --libs glfw3`
  AC_SUBST([GLFW_CFLAGS])
  AC_SUBST([GLFW_LIBS])
], [AC_MSG_ERROR([GLFW 3 must be installed.])])

PKG_CHECK_EXISTS([glew], [
  GLEW_CFLAGS=`$PKG_CONFIG --cflags glew`
  GLEW_LIBS=`$PKG_CONFIG --libs glew`
  AC_SUBST([GLEW_CFLAGS])
  AC_SUBST([GLEW_LIBS])
], [AC_MSG_ERROR([GLEW must be installed.])])

PKG_CHECK_EXISTS([libpng], [
  LIBNG_CFLAGS=`$PKG_CONFIG --cflags libpng`
  LIBPNG_LIBS=`$PKG_CONFIG --libs libpng`
  AC_SUBST([LIBPNG_CFLAGS])
  AC_SUBST([LIBPNG_LIBS])
], [AC_MSG_ERROR([LibPNG must be installed.])])

AC_OUTPUT
