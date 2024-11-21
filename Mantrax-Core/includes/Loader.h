#pragma once
#include "Core.h"
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__linux__)
#include <dlfcn.h>
#endif

class GARINLIBS_API Loader
{
public:
    std::unique_ptr<void, void (*)(void *)> handle{nullptr, [](void *h)
                                                   {
#if defined(_WIN32) || defined(_WIN64)
                                                       if (h)
                                                           FreeLibrary(static_cast<HMODULE>(h));
#elif defined(__linux__)
                                                       if (h)
                                                           dlclose(h);
#endif
                                                   }};

    Loader() = default;
    Loader(const char *path) { load(path); }
    ~Loader() = default;

    void load(const char *path)
    {
        if (handle)
            return;

#if defined(_WIN32) || defined(_WIN64)
        HMODULE h = LoadLibrary(path);
        if (!h)
        {
            std::cerr << "Error al cargar la DLL: " << path << " - Código de error: " << GetLastError() << std::endl;
            return;
        }
        handle.reset(h);
#elif defined(__linux__)
        void *h = dlopen(path, RTLD_LAZY);
        if (!h)
        {
            std::cerr << "Error al cargar la biblioteca dinámica: " << dlerror() << std::endl;
            return;
        }
        handle.reset(h);
#endif
        std::cout << "DLL/biblioteca cargada correctamente: " << path << std::endl;
    }

    void free()
    {
        handle.reset();
    }

    bool is_loaded() const { return handle != nullptr; }

    template <typename T>
    T get_function(const char *name)
    {
#if defined(_WIN32) || defined(_WIN64)
        return (T)GetProcAddress(static_cast<HMODULE>(handle.get()), name);
#elif defined(__linux__)
        return (T)dlsym(handle.get(), name);
#endif
    }
};