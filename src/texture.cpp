#include "texture.hpp"
#include "resource.hpp"
#include "file.hpp"
#include "pixmap.hpp"
#include <cassert>
namespace Seven {

const std::string Texture::prefix = "texture:";
static const std::string PNG_SUFFIX(".png");

class Texture::TexResource : public Resource {
public:
    const std::string path_;
    Info info_;
    Texture *first_;

    TexResource(const std::string &path)
        : Resource(Texture::prefix + path), path_(path), first_(nullptr) { }

    virtual ~TexResource() {
        if (info_.texture != 0)
            glDeleteTextures(1, &info_.texture);
    }

    virtual void load() {
        Buffer buf = File::read(path_ + PNG_SUFFIX);
        Pixmap pixmap = PNG::load(buf.data(), buf.size());
        GLenum ifmt, efmt;
        switch (pixmap.channel_count()) {
        case 1: ifmt = GL_R8; efmt = GL_RED; break;
        case 2: ifmt = GL_RG8; efmt = GL_RG; break;
        case 3: ifmt = GL_RGBA8; efmt = GL_RGB; break;
        case 4: ifmt = GL_RGBA8; efmt = GL_RGBA; break;
        }
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(
            GL_TEXTURE_2D, 0, ifmt, pixmap.buf_width(), pixmap.buf_height(),
            0, efmt, GL_UNSIGNED_BYTE, pixmap.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        gl_error_check("texture %s", path_.c_str());
        info_.tex_width = pixmap.buf_width();
        info_.tex_height = pixmap.buf_height();
        info_.img_width = pixmap.img_width();
        info_.img_height = pixmap.img_height();
        info_.texture = tex;
        for (Texture *tp = first_; tp != nullptr; tp = tp->next_)
            tp->info_ = info_;
    }

    void remove(Texture *tex) {
        Texture **last = &first_;
        while (true) {
            Texture *cur = *last;
            if (cur == tex) {
                *last = cur->next_;
                decref();
                break;
            } else if (cur == nullptr) {
                break;
            }
            last = &cur->next_;
        }
        tex->next_ = nullptr;
        tex->resource_ = nullptr;
        tex->info_ = Info();
    }

    void add(Texture *tex) {
        assert(tex->next_ == nullptr);
        assert(tex->resource_ == nullptr);
        tex->next_ = first_;
        tex->info_ = info_;
        tex->resource_ = this;
        first_ = tex;
        incref();
    }
};

Texture::~Texture() {
    resource_->decref();
}

void Texture::load_file(const std::string &path) {
    info_ = Info();
    if (resource_)
        resource_->remove(this);
    resource_ = nullptr;
    Resource *r = Resource::get_resource(Texture::prefix + path);
    TexResource *tr;
    if (r != nullptr) {
        tr = dynamic_cast<TexResource *>(r);
        assert(tr != nullptr);
    } else {
        tr = new TexResource(path);
    }
    tr->add(this);
}

}
