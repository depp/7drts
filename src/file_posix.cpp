#include "file.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits>
#include <stdexcept>
#include <string.h>
#include <errno.h>
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

static void error(int err_code, const std::string &path, const char *what) {
    if (err_code != 0) {
        std::fprintf(stderr, "%s: %s: %s\n", path.c_str(), what,
                     strerror(err_code));
    } else {
        std::fprintf(stderr, "%s: %s\n", path.c_str(), what);
    }
    throw std::runtime_error("File::read()");
}

Buffer read(const std::string &path) {
    int fdes = ::open(path.c_str(), O_RDONLY);
    if (fdes < 0)
        error(errno, path, "open()");
    Fdes fdobj(fdes);

    struct stat st;
    int r = ::fstat(fdes, &st);
    if (r != 0)
        error(errno, path, "fstat()");

    if (!S_ISREG(st.st_mode))
        error(0, path, "not a regular file");

    if (st.st_size > std::numeric_limits<std::size_t>::max())
        error(0, path, "file too large");
    std::size_t size = static_cast<std::size_t>(st.st_size);
    Buffer buf(size);
    std::size_t pos = 0;

    while (pos < size) {
        ssize_t amt = ::read(
            fdes,
            reinterpret_cast<unsigned char *>(buf.data()) + pos,
            size - pos);
        if (amt < 0)
            error(errno, path, "read()");
        if (amt == 0)
            error(0, path, "unexpected EOF");
        pos += amt;
    }

    return buf;
}

}
}
