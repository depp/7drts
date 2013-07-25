#include "texture.hpp"
#include "resource.hpp"
#include <cassert>
namespace Seven {

const std::string Texture::prefix = "texture:";

class Texture::TexResource : public Resource {
public:
    const std::string path_;
    Info info_;
    Texture *first_;

    TexResource(const std::string &path)
        : Resource(Texture::prefix + path), path_(path) { }

    virtual ~TexResource() {
        if (info_.texture != 0)
            glDeleteTextures(1, &info_.texture);
    }

    virtual void load() {
        
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
