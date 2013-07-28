#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "opengl.hpp"
#include "resource.hpp"
#include <string>
namespace Seven {

struct Texture {
    int tex_width, tex_height;
    int img_width, img_height;
    GLuint texture;

    Texture() : tex_width(0), tex_height(0),
                img_width(0), img_height(0),
                texture(0) { }

    static const std::string PREFIX;
    void load(const std::string name);
    void unload();

    typedef ResourceRef<Texture> Ref;
};

}
#endif
