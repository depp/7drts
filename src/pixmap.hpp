#ifndef PIXMAP_HPP
#define PIXMAP_HPP
#include <cstddef>
#include <cstdlib>
namespace Seven {

class Pixmap {
    unsigned char *data_;
    int width_;
    int height_;
    std::size_t rowbytes_;
    int channel_count_;

    void zero() {
        data_ = nullptr;
        width_ = 0;
        height_ = 0;
        rowbytes_ = 0;
        channel_count_ = 0;
    }

public:
    static const int MAX_SIZE = 32768;

    std::size_t calculate_rowbytes(int width, int channel_count);

    Pixmap() { zero(); }
    Pixmap(const Pixmap &) = delete;
    Pixmap(Pixmap &&p)
        : data_(p.data_), width_(p.width_), height_(p.height_),
          rowbytes_(p.rowbytes_), channel_count_(p.channel_count_) {
        p.zero();
    }
    Pixmap(int width, int height, int channel_count);

    Pixmap &operator=(const Pixmap &) = delete;
    Pixmap &operator=(Pixmap &&p) {
        std::free(data_);
        data_ = p.data_;
        width_ = p.width_;
        height_ = p.height_;
        rowbytes_ = p.rowbytes_;
        channel_count_ = p.channel_count_;
        p.zero();
        return *this;
    }
    ~Pixmap() {
        std::free(data_);
    }

    void clear() {
        std::free(data_);
        zero();
    }

    bool empty() const { return data_ == nullptr; }
    const unsigned char *data() const { return data_; }
    unsigned char *data() { return data_; }
    const unsigned char *row(int i) const { return data_ + rowbytes_ * i; }
    unsigned char *row(int i) { return data_ + rowbytes_ * i; }
    int width() const { return width_; }
    int height() const { return height_; }
    int rowbytes() const { return rowbytes_; }
    int channel_count() const { return channel_count_; }

    void premultiply_alpha();
};

Pixmap load_png(const void *data, std::size_t size);

}
#endif
