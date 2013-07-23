AC_DEFUN([ENABLE_WARNINGS],[
  AC_ARG_ENABLE(warnings,
    [  --enable-warnings       enable warnings for GCC/Clang ],
    [enable_warnings=$enableval], [enable_warnings=no])
  if test "x$enable_warnings" != xno ; then
    [warning_cxxflags="-Wall -Wextra"]
    if test "x$enable_warnings" != xyes ; then
      [warning_cxxflags="$warning_cxxflags $enable_warnings"]
    fi
  fi
  AC_SUBST([warning_cxxflags])
])
