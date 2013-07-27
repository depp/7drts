#include "pixmap.hpp"
#include <new>
#include <stdexcept>
namespace Seven {

static int round_up_pow2(int x) {
    unsigned v = static_cast<unsigned>(x) - 1;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return static_cast<int>(v + 1);
}

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
    img_width_ = width;
    img_height_ = height;
    buf_width_ = round_up_pow2(width);
    buf_height_ = round_up_pow2(height);
    rowbytes_ = calculate_rowbytes(buf_width_, channel_count);
    channel_count_ = channel_count;
    data_ = reinterpret_cast<unsigned char *>(
        std::malloc(rowbytes_ * buf_height_));
    if (data_ == nullptr)
        throw std::bad_alloc();
}

void Pixmap::premultiply_alpha() {
    if (channel_count_ == 4) {
        unsigned char *data = data_;
        int width = buf_width_, height = buf_height_;
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

Pixmap downsample_pixmap(const Pixmap &pixmap) {
    int width = pixmap.buf_width() / 2, height = pixmap.buf_height() / 2;
    std::size_t irb = pixmap.rowbytes(), orb;
    const unsigned char *idata = pixmap.data();
    unsigned char *odata;
    int x, y;
    unsigned r, g, b, a;

    if (pixmap.buf_width() < 1 || pixmap.buf_height() < 1 ||
        (pixmap.buf_width() != width * 2 && pixmap.buf_width() != 1) ||
        (pixmap.buf_height() != height * 2 && pixmap.buf_height() != 1) ||
        (pixmap.buf_width() == 1 && pixmap.buf_height() == 1))
        throw std::domain_error("downsample_pixmap()");
    width = width ? width : 1;
    height = height ? height : 1;
    Pixmap new_pixmap(width, height, pixmap.channel_count());
    orb = new_pixmap.rowbytes();
    odata = new_pixmap.data();

    if (pixmap.buf_width() == 1) {
        switch (pixmap.channel_count()) {
        case 4:
            for (y = 0; y < height; y++) {
                r = idata[(2*y+0)*irb+0] +
                    idata[(2*y+1)*irb+0];
                g = idata[(2*y+0)*irb+1] +
                    idata[(2*y+1)*irb+1];
                b = idata[(2*y+0)*irb+2] +
                    idata[(2*y+1)*irb+2];
                a = idata[(2*y+0)*irb+3] +
                    idata[(2*y+1)*irb+3];
                odata[y*orb+0] = r >> 1;
                odata[y*orb+1] = g >> 1;
                odata[y*orb+2] = b >> 1;
                odata[y*orb+3] = a >> 1;
            }
            break;
        default:
            throw std::domain_error("downsample_pixmap()");
        }
    } else if (pixmap.buf_height() == 1) {
        switch (pixmap.channel_count()) {
        case 4:
            for (x = 0; x < width; x++) {
                r = idata[8*x+0] +
                    idata[8*x+4];
                g = idata[8*x+1] +
                    idata[8*x+5];
                b = idata[8*x+2] +
                    idata[8*x+6];
                a = idata[8*x+3] +
                    idata[8*x+7];
                odata[4*x+0] = r >> 1;
                odata[4*x+1] = g >> 1;
                odata[4*x+2] = b >> 1;
                odata[4*x+3] = a >> 1;
            }
            break;
        default:
            throw std::domain_error("downsample_pixmap()");
        }
    } else {
        switch (pixmap.channel_count()) {
        case 4:
            for (y = 0; y < height; y++) {
                for (x = 0; x < width; x++) {
                    r = idata[(2*y+0)*irb+8*x+0] +
                        idata[(2*y+0)*irb+8*x+4] +
                        idata[(2*y+1)*irb+8*x+0] +
                        idata[(2*y+1)*irb+8*x+4];
                    g = idata[(2*y+0)*irb+8*x+1] +
                        idata[(2*y+0)*irb+8*x+5] +
                        idata[(2*y+1)*irb+8*x+1] +
                        idata[(2*y+1)*irb+8*x+5];
                    b = idata[(2*y+0)*irb+8*x+2] +
                        idata[(2*y+0)*irb+8*x+6] +
                        idata[(2*y+1)*irb+8*x+2] +
                        idata[(2*y+1)*irb+8*x+6];
                    a = idata[(2*y+0)*irb+8*x+3] +
                        idata[(2*y+0)*irb+8*x+7] +
                        idata[(2*y+1)*irb+8*x+3] +
                        idata[(2*y+1)*irb+8*x+7];
                    odata[y*orb+4*x+0] = r >> 2;
                    odata[y*orb+4*x+1] = g >> 2;
                    odata[y*orb+4*x+2] = b >> 2;
                    odata[y*orb+4*x+3] = a >> 2;
                }
            }
            break;
        default:
            throw std::domain_error("downsample_pixmap()");
        }
    }
    return new_pixmap;
}

}
