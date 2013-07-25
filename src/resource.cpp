#include "resource.hpp"
#include <vector>
#include <stdexcept>
#include <cstdio>
namespace Seven {

std::unordered_map<std::string, Resource *> Resource::all_resources;

Resource::Resource(std::string name)
    : name_(name), refcount_(0), is_loaded_(false) {
    auto val = all_resources.insert(
        std::pair<std::string, Resource *>(name_, this));
    if (!val.second)
        throw std::logic_error("Resource::Resource()");
}

Resource::~Resource() {
    auto val = all_resources.find(name_);
    if (val != all_resources.end())
        all_resources.erase(val);
}

Resource *Resource::get_resource(const std::string &name) {
    auto val = all_resources.find(name);
    return val == all_resources.end() ? nullptr : val->second;
}

void Resource::load_all() {
    std::vector<Resource *> need_load, need_unload;
    for (auto i : all_resources) {
        Resource *r = i.second;
        if (r->refcount_ > 0 && !r->is_loaded_)
            need_load.push_back(r);
        else if (r->refcount_ == 0 && r->is_loaded_)
            need_unload.push_back(r);
    }
    for (auto r : need_unload) {
        all_resources.erase(r->name_);
        delete r;
    }
    for (auto r : need_load) {
        std::fprintf(stderr, "loading %s\n", r->name_.c_str());
        r->load();
        r->is_loaded_ = true;
    }
}

}
