#pragma once
#include "Core.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__linux__)
#include <dlfcn.h>
#endif

class GARINLIBS_API Loader
{
public:
    void *handle = nullptr;

    Loader() = default;
    Loader(const char *path);
    ~Loader();

    void load(const char *path);

    void free();

    template <typename T>
    T get_function(const char *name)
    {
#if defined(_WIN32) || defined(_WIN64)
        return (T)GetProcAddress((HMODULE)handle, name);
#elif defined(__linux__)
        return (T)dlsym(handle, name);
#endif
    }
};