#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "opengl.hpp"
#include <string>
namespace Seven {

class Texture {
    class TexResource;
    friend class TexResource;

    static const std::string prefix;

    struct Info {
        Info() : width(0), height(0), texture(0) { }
        int width, height;
        GLuint texture;
    };

    Info info_;
    TexResource *resource_;
    Texture *next_;

public:
    Texture() : resource_(nullptr), next_(nullptr) { }
    explicit Texture(const std::string &path)
        : resource_(nullptr), next_(nullptr) {
        load_file(path);
    }

    ~Texture();

    int width() const { return info_.width; }
    int height() const { return info_.height; }
    GLuint texture() const { return info_.texture; }

    void load_file(const std::string &path);
};

}
#endif
