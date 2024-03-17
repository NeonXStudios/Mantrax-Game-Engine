#ifndef MY_APPLICATION_RESOURCELOADER_H
#define MY_APPLICATION_RESOURCELOADER_H

#include <memory>
#include <string>

class ResourceLoader {
public:
    template <typename T>
    static std::shared_ptr<T> Load(const std::string& path) {
        return T::loadAsset();
    }
};

#endif
