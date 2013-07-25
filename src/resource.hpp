#ifndef RESOURCE_HPP
#define RESOURCE_HPP
#include <string>
#include <unordered_map>
namespace Seven {

// Object which loads a resource
class Resource {
private:
    const std::string name_;
    unsigned refcount_;
    bool is_loaded_;

    static std::unordered_map<std::string, Resource *> all_resources;

protected:
    // Create a new resource loader for the resource with the given
    // unique and canonical name.  An exception is raised if a
    // resource with this name already exists.  The resource will
    // start with a refcount of zero.
    Resource(std::string name);

    virtual ~Resource();

    // Load the resource.
    virtual void load() = 0;

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

}
#endif
