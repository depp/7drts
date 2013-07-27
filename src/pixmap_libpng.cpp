#include "pixmap.hpp"
#include <png.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
namespace Seven {
namespace PNG {

struct Libpng {
    png_struct *pngp;
    png_info *infop;
    const char *buf_ptr, *buf_end;

    png_uint_32 width, height;
    int depth, ctype;
    int channel_count;

    static void read(png_struct *pngp, unsigned char *ptr, png_size_t len) {
        Libpng *obj = reinterpret_cast<Libpng *>(png_get_io_ptr(pngp));
        std::size_t rem = obj->buf_end - obj->buf_ptr;
        if (len <= rem) {
            std::memcpy(ptr, obj->buf_ptr, len);
            obj->buf_ptr += len;
        } else {
            std::fputs("PNG: unexpected end of file\n", stderr);
            longjmp(png_jmpbuf(pngp), 1);
        }
    }

    static void error(png_struct *pngp, const char *msg) {
        std::fprintf(stderr, "LibPNG: error: %s\n", msg);
        longjmp(png_jmpbuf(pngp), 1);
    }

    static void warning(png_struct *pngp, const char *msg) {
        (void) pngp;
        std::fprintf(stderr, "LibPNG: warning: %s\n", msg);
    }

    Libpng() : pngp(nullptr), infop(nullptr) {
        pngp = png_create_read_struct_2(
            PNG_LIBPNG_VER_STRING,
            nullptr, error, warning, nullptr, nullptr, nullptr);
        if (pngp == nullptr)
            throw std::runtime_error("Libpng::Libpng()");
        infop = png_create_info_struct(pngp);
        if (infop == nullptr) {
            png_destroy_read_struct(&pngp, nullptr, nullptr);
            throw std::runtime_error("Libpng::Libpng()");
        }
    }
    Libpng(const Libpng &) = delete;
    ~Libpng() {
        png_destroy_read_struct(&pngp, &infop, nullptr);
    }
    Libpng &operator=(const Libpng &) = delete;

    void begin_read(const void *data, std::size_t size) {
        buf_ptr = reinterpret_cast<const char *>(data);
        buf_end = buf_ptr + size;
        png_set_read_fn(pngp, reinterpret_cast<void *>(this), read);
    }

    bool read_info() {
        if (setjmp(png_jmpbuf(pngp)))
            return false;
        png_read_info(pngp, infop);
        png_get_IHDR(pngp, infop, &width, &height, &depth, &ctype,
                     nullptr, nullptr, nullptr);
        bool has_trns = png_get_valid(pngp, infop, PNG_INFO_tRNS) != 0;
        png_set_expand(pngp);
        switch (ctype) {
        case PNG_COLOR_TYPE_PALETTE:
            channel_count = has_trns ? 4 : 3;
            break;
        case PNG_COLOR_TYPE_GRAY:
            channel_count = has_trns ? 2 : 1;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            channel_count = 2;
            break;
        case PNG_COLOR_TYPE_RGB:
            channel_count = has_trns ? 4 : 3;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            channel_count = 4;
            break;
        default:
            std::fputs("PNG: unexpected image type\n", stderr);
            return false;
        }
        if (depth > 8)
            png_set_strip_16(pngp);
        return true;
    }

    bool read_data(Pixmap &pixmap) {
        unsigned char **rowp;
        rowp = reinterpret_cast<unsigned char **>(
            std::malloc(sizeof(*rowp) * height));
        if (rowp == nullptr)
            throw std::bad_alloc();
        if (setjmp(png_jmpbuf(pngp))) {
            std::free(rowp);
            return false;
        }
        for (unsigned i = 0; i < height; i++)
            rowp[i] = pixmap.row(static_cast<int>(i));
        png_read_image(pngp, rowp);
        png_read_end(pngp, nullptr);
        std::free(rowp);
        return true;
    }
};

Pixmap load(const void *data, std::size_t size) {
    Libpng obj;
    obj.begin_read(data, size);
    if (!obj.read_info())
        throw std::runtime_error("PNG::load()");

    if (obj.width < 1 || obj.width > Pixmap::MAX_SIZE ||
        obj.height < 1 || obj.height > Pixmap::MAX_SIZE) {
        std::fputs("PNG: image size out of range\n", stderr);
        throw std::runtime_error("PNG::load()");
    }

    Pixmap pixmap(obj.width, obj.height, obj.channel_count);
    if (!obj.read_data(pixmap))
        throw std::runtime_error("PNG::load()");

    pixmap.premultiply_alpha();

    return pixmap;
}

}
}
