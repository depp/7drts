#include "pixmap.hpp"
#include <ApplicationServices/ApplicationServices.h>
#include <stdexcept>
#include <cstdio>
namespace Seven {
namespace PNG {

struct CGLoader {
    CGDataProviderRef provider;
    CGImageRef image;
    CGColorSpaceRef color_space;
    CGContextRef context;
    
    static void release_data(void *info, const void *data, std::size_t size) {
        (void) info;
        (void) data;
        (void) size;
    }
    
    CGLoader() : provider(nullptr), image(nullptr), color_space(nullptr),
                 context(nullptr) { }
    CGLoader(const CGLoader &) = delete;
    ~CGLoader() {
        if (provider != nullptr)
            CGDataProviderRelease(provider);
        if (image != nullptr)
            CGImageRelease(image);
        if (color_space != nullptr)
            CGColorSpaceRelease(color_space);
        if (context != nullptr)
            CGContextRelease(context);
    }
    CGLoader &operator=(const CGLoader &) = delete;
    
    Pixmap image_to_pixmap() {
        std::size_t width = CGImageGetWidth(image);
        std::size_t height = CGImageGetHeight(image);
        if (width < 1 || width > Pixmap::MAX_SIZE ||
            height < 1 || height > Pixmap::MAX_SIZE) {
            std::fputs("PNG: image size out of range\n", stderr);
            throw std::runtime_error("PNG::load()");
        }
        Pixmap pixmap(static_cast<int>(width),
                      static_cast<int>(height),
                      4);
        color_space = CGColorSpaceCreateDeviceRGB();
        context = CGBitmapContextCreate(
            pixmap.data(), width, height, 8, pixmap.rowbytes(), color_space,
            kCGImageAlphaPremultipliedLast);
        CGContextSetBlendMode(context, kCGBlendModeCopy);
        CGContextDrawImage(context, CGRectMake(0, 0, width, height), image);
        return pixmap;
    }
};

Pixmap load(const void *data, std::size_t size) {
    CGLoader obj;
    obj.provider = CGDataProviderCreateWithData(
        nullptr, data, size, CGLoader::release_data);
    obj.image = CGImageCreateWithPNGDataProvider(
        obj.provider, nullptr, false, kCGRenderingIntentDefault);
    return obj.image_to_pixmap();
}

}
}
