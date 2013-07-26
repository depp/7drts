#include "file.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits>
#include <stdexcept>
#pragma GCC diagnostic ignored "-Wsign-compare"
namespace Seven {
namespace File {

struct Fdes {
    Fdes(const Fdes &) = delete;
    Fdes &operator=(const Fdes &) = delete;
    Fdes(int fd) : fd(fd) { }
    ~Fdes() { close(fd); }
    int fd;
};

Buffer read(const std::string &path) {
    int fdes = ::open(path.c_str(), O_RDONLY);
    if (fdes < 0)
        throw std::runtime_error("File::read()");
    Fdes fdobj(fdes);

    struct stat st;
    int r = ::fstat(fdes, &st);
    if (r != 0)
        throw std::runtime_error("File::read()");

    if (!S_ISREG(st.st_mode))
        throw std::runtime_error("File::read()");

    if (st.st_size > std::numeric_limits<std::size_t>::max())
        throw std::runtime_error("File::read()");
    std::size_t size = static_cast<std::size_t>(st.st_size);
    Buffer buf(size);
    std::size_t pos = 0;

    while (pos < size) {
        ssize_t amt = ::read(
            fdes,
            reinterpret_cast<unsigned char *>(buf.data()) + pos,
            size - pos);
        if (amt < 0)
            throw std::runtime_error("File::read()");
        if (amt == 0)
            throw std::runtime_error("File::read()");
        pos += amt;
    }

    return buf;
}

}
}
