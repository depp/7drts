#ifndef OPENGL_HPP
#define OPENGL_HPP
#include <GL/glew.h>
#include <cstdarg>
namespace Seven {

// Check for OpenGL errors.  Return true if no errors were detected,
// return false if some errors were detected.  Errors are logged using
// the location and format arguments.
bool gl_error_check(const char *loc, ...)
    __attribute__((format(printf, 1, 2)));

// Same as above, but with va_list.
bool gl_error_checkv(const char *loc, va_list ap);

};
#endif
