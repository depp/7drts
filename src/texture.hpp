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
        Info() : tex_width(0), tex_height(0),
                 img_width(0), img_height(0), texture(0) { }
        int tex_width, tex_height;
        int img_width, img_height;
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

    int tex_width() const { return info_.tex_width; }
    int tex_height() const { return info_.tex_height; }
    int img_width() const { return info_.img_width; }
    int img_height() const { return info_.img_height; }
    GLuint texture() const { return info_.texture; }

    void load_file(const std::string &path);
};

}
#endif
