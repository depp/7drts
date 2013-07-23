AUTOMAKE_OPTIONS = 1.5 foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = seven

seven_CXXFLAGS = @warning_cxxflags@ @GLFW_CFLAGS@
seven_LDADD = @GLFW_LIBS@
seven_LDFLAGS = -Wl,--as-needed

seven_SOURCES = \
    src/screen.cpp \
    src/screeen.hpp
