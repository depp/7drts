AUTOMAKE_OPTIONS = 1.5 foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = seven

seven_CXXFLAGS = @warning_cxxflags@ @GLFW_CFLAGS@ @GLEW_CFLAGS@
seven_LDADD = @GLFW_LIBS@ @GLEW_LIBS@
seven_LDFLAGS = -Wl,--as-needed

seven_SOURCES = \
	src/opengl.cpp \
	src/opengl.hpp \
	src/screen.cpp \
	src/screeen.hpp
