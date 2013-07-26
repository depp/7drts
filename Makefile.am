AUTOMAKE_OPTIONS = 1.5 foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = seven

seven_CXXFLAGS = @warning_cxxflags@ @GLFW_CFLAGS@ @GLEW_CFLAGS@ @LIBPNG_CFLAGS@
seven_LDADD = @GLFW_LIBS@ @GLEW_LIBS@ @LIBPNG_LIBS@
seven_LDFLAGS = -Wl,--as-needed

seven_SOURCES = \
	src/file_posix.cpp \
	src/file.hpp \
	src/opengl.cpp \
	src/opengl.hpp \
	src/pixmap.cpp \
	src/pixmap.hpp \
	src/pixmap_libpng.cpp \
	src/resource.cpp \
	src/resource.hpp \
	src/screen.cpp \
	src/screeen.hpp \
	src/texture.cpp \
	src/texture.hpp
