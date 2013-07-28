#include "texture.hpp"
#include "resource.hpp"
#include "file.hpp"
#include "pixmap.hpp"
#include <cassert>
#include <cstdio>
namespace Seven {

const std::string Texture::PREFIX("texture:");

static const std::string PNG_SUFFIX(".png");

void Texture::load(const std::string name) {
    Buffer buf = File::read(name + PNG_SUFFIX);
    Pixmap pixmap = PNG::load(buf.data(), buf.size());
    tex_width = pixmap.buf_width();
    tex_height = pixmap.buf_height();
    img_width = pixmap.img_width();
    img_height = pixmap.img_height();
    GLenum ifmt, efmt;
    switch (pixmap.channel_count()) {
    case 1: ifmt = GL_R8; efmt = GL_RED; break;
    case 2: ifmt = GL_RG8; efmt = GL_RG; break;
    case 3: ifmt = GL_RGBA8; efmt = GL_RGB; break;
    case 4: ifmt = GL_RGBA8; efmt = GL_RGBA; break;
    default: throw std::runtime_error("Texture::load()");
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    for (int level = 0; ; level++) {
        glTexImage2D(
            GL_TEXTURE_2D, level, ifmt,
            pixmap.buf_width(), pixmap.buf_height(),
            0, efmt, GL_UNSIGNED_BYTE, pixmap.data());
        if (pixmap.buf_width() == 1 && pixmap.buf_height() == 1)
            break;
        pixmap = downsample_pixmap(pixmap);
    }
    // glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    gl_error_check("texture %s", name.c_str());
}

void Texture::unload() {
    glDeleteTextures(1, &texture);
}

}
