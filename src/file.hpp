#ifndef FILE_HPP
#define FILE_HPP
#include <cstdlib>
#include <string>
#include <new>
namespace Seven {

class Buffer {
    void *data_;
    std::size_t size_;

public:
    Buffer() : data_(nullptr), size_(0) { }
    Buffer(const Buffer &) = delete;
    Buffer(Buffer &&b)
        : data_(b.data_), size_(b.size_) {
        b.data_ = nullptr;
        b.size_ = 0;
    }
    Buffer(std::size_t size) : data_(nullptr), size_(0) {
        if (size > 0) {
            data_ = std::malloc(size);
            if (data_ == nullptr)
                throw std::bad_alloc();
            size_ = size;
        }
    }
    ~Buffer() {
        std::free(data_);
    }

    Buffer &operator=(const Buffer &) = delete;
    Buffer &operator=(Buffer &&b) {
        std::free(data_);
        data_ = b.data_;
        size_ = b.size_;
        b.data_ = nullptr;
        b.size_ = 0;
        return *this;
    }

    void *data() { return data_; }
    const void *data() const { return data_; }
    std::size_t size() const { return size_; }
};

namespace File {
Buffer read(const std::string &path);
}

}
#endif
