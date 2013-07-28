#ifndef RESOURCE_HPP
#define RESOURCE_HPP
#include <string>
#include <unordered_map>
#include <cassert>
namespace Seven {

// Object which loads a resource
class Resource {
private:
    const std::string name_;
    unsigned refcount_;
    bool is_loaded_;

    static std::unordered_map<std::string, Resource *> all_resources;

    Resource(const Resource &) = delete;
    Resource &operator=(const Resource &) = delete;

protected:
    // Create a new resource loader for the resource with the given
    // unique and canonical name.  An exception is raised if a
    // resource with this name already exists.  The resource will
    // start with a refcount of zero.
    Resource(std::string name);

    virtual ~Resource();

    // Load the resource.
    virtual void load() = 0;

    // Unload the resource.
    virtual void unload() = 0;

public:
    // Unique name identifying this resource.  If multiple resources
    // have the same name, they might be coalesced.
    const std::string &name() const { return name_; }

    // Check whether the resource is loaded.
    bool is_loaded() const { return is_loaded_; }

    // Get the resource with the given name, or return NULL if no such
    // resource exists.
    static Resource *get_resource(const std::string &name);

    // Load all resources with references, and unload all resources
    // with no references.
    static void load_all();

    void incref() {
        refcount_++;
    }

    void decref() {
        refcount_--;
    }
};

/*
  In order to use this, define:

  struct MyResource {
      void load(const std::string *name);
      void unload();
      static const std::string PREFIX("prefix:");
  };
*/
template<typename ResourceType>
class ResourceRef {
private:
    class ResourceObj : public Resource {
        friend class ResourceRef;
        const std::string name_;
        ResourceType data_;
        ResourceRef *first_;

        ResourceObj(const std::string name)
            : Resource(ResourceType::PREFIX + name), name_(name),
              first_(nullptr) { }

    protected:
        virtual ~ResourceObj() { }
        virtual void load() {
            data_.load(name_);
            for (ResourceRef *r = first_; r != nullptr; r = r->next_)
                r->data_ = data_;
        }
        virtual void unload() {
            data_.unload();
            data_ = ResourceType();
            for (ResourceRef *r = first_; r != nullptr; r = r->next_)
                r->data_ = ResourceType();
        }
    };

    ResourceType data_;
    ResourceObj *resource_;
    ResourceRef *next_;

    void set_resource(ResourceObj *obj) {
        assert(resource_ == nullptr);
        assert(next_ == nullptr);
        data_ = obj->data_;
        resource_ = obj;
        next_ = obj->first_;
        obj->first_ = this;
        obj->incref();
    }

    void unset_resource() {
        assert(resource_ != nullptr);
        ResourceRef **last = &resource_->first_;
        while (true) {
            ResourceRef *cur = *last;
            if (cur == this) {
                *last = cur->next_;
                resource_->decref();
                break;
            }
            assert(cur != nullptr);
            last = &cur->next_;
        }
        data_ = ResourceType();
        resource_ = nullptr;
        next_ = nullptr;
    }

public:
    ResourceRef() : data_(), resource_(nullptr), next_(nullptr) { }
    explicit ResourceRef(const std::string &name)
        : data_(), resource_(nullptr), next_(nullptr) {
        load(name);
    }
    ResourceRef(const ResourceRef &rr)
        : data_(), resource_(nullptr), next_(nullptr) {
        set_resource(rr.resource_);
    }

    ResourceRef &operator=(const ResourceRef &rr) {
        ResourceObj *r = rr.resource_;
        if (resource_ != nullptr)
            unset_resource();
        if (r != nullptr)
            set_resource(r);
    }

    const ResourceType *operator->() const { return &data_; }
    const ResourceType &operator*() const { return data_; }

    void load(const std::string &name) {
        ResourceObj *r = dynamic_cast<ResourceObj *>(
            Resource::get_resource(ResourceType::PREFIX + name));
        if (r == nullptr)
            r = new ResourceObj(name);
        if (resource_ != nullptr)
            unset_resource();
        set_resource(r);
    }

    void clear() {
        if (resource_ != nullptr)
            unset_resource();
    }
};

}
#endif
