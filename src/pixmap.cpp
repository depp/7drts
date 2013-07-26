#include "pixmap.hpp"
#include <new>
#include <stdexcept>
namespace Seven {

std::size_t Pixmap::calculate_rowbytes(int width, int channel_count) {
    std::size_t sz = static_cast<std::size_t>(width) *
        static_cast<std::size_t>(channel_count);
    return (sz + 3) & ~static_cast<std::size_t>(3);
}


Pixmap::Pixmap(int width, int height, int channel_count) {
    if (width < 1 || width > MAX_SIZE ||
        height < 1 || height > MAX_SIZE ||
        channel_count < 1 || channel_count > 4)
        throw std::runtime_error("Pixmap::Pixmap()");
    width_ = width;
    height_ = height;
    rowbytes_ = calculate_rowbytes(width, channel_count);
    channel_count_ = channel_count;
    data_ = reinterpret_cast<unsigned char *>(
        std::malloc(rowbytes_ * height_));
    if (data_ == nullptr)
        throw std::bad_alloc();
}

void Pixmap::premultiply_alpha() {
    if (channel_count_ == 4) {
        unsigned char *data = data_;
        int width = width_, height = height_;
        std::size_t rowbytes = rowbytes_;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                unsigned char *ptr = data + rowbytes * y + 4 * x;
                unsigned r = ptr[0], g = ptr[1], b = ptr[2], a = ptr[3];
                a = a | (a << 8) | (a << 16);
                ptr[0] = (unsigned char)((r * a) >> 24);
                ptr[1] = (unsigned char)((g * a) >> 24);
                ptr[2] = (unsigned char)((b * a) >> 24);
            }
        }
    }
}

}
