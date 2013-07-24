#include "opengl.hpp"
#include <cstdio>
namespace Seven {

struct GlError {
    unsigned short code;
    char name[30];
};

static const struct GlError GL_ERRORS[] = {
    { 0x0500, "INVALID_ENUM" },
    { 0x0501, "INVALID_VALUE" },
    { 0x0502, "INVALID_OPERATION" },
    { 0x0503, "STACK_OVERFLOW" },
    { 0x0504, "STACK_UNDERFLOW" },
    { 0x0505, "OUT_OF_MEMORY" },
    { 0x0506, "INVALID_FRAMEBUFFER_OPERATION" },
};

bool gl_error_check(const char *loc, ...) {
    va_list ap;
    va_start(ap, loc);
    bool val = gl_error_checkv(loc, ap);
    va_end(ap);
    return val;
}

bool gl_error_checkv(const char *loc, va_list ap) {
    char loc_buf[256], desc_buf[32];
    bool has_error = false;
    while (true) {
        GLenum err = glGetError();
        if (!err)
            return has_error;
        if (!has_error) {
            has_error = true;
            std::vsnprintf(loc_buf, sizeof(loc_buf), loc, ap);
        }
        const char *desc_ptr = nullptr;
        for (std::size_t i = 0;
             i < sizeof(GL_ERRORS) / sizeof(*GL_ERRORS); i++) {
            if (err == GL_ERRORS[i].code) {
                desc_ptr = GL_ERRORS[i].name;
                break;
            }
        }
        if (desc_ptr == nullptr) {
            std::snprintf(desc_buf, sizeof(desc_buf),
                          "unknown (0x%04x)", err);
            desc_ptr = desc_buf;
        }
        std::fprintf(stderr, "%s: OpenGL error %s\n",
                     loc_buf, desc_ptr);
    }
}

}
