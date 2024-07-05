#include "../includes/Loader.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__linux__)
#include <dlfcn.h>
#endif

Loader::Loader(const char *path)
{
    load(path);
}

void Loader::load(const char *path)
{
#if defined(_WIN32) || defined(_WIN64)
    handle = LoadLibrary(path);
#elif defined(__linux__)
    handle = dlopen(path, RTLD_LAZY);
#endif
}

void Loader::free()
{
#if defined(_WIN32) || defined(_WIN64)
    FreeLibrary((HMODULE)handle);
#elif defined(__linux__)
    dlclose(handle);
#endif
}

Loader::~Loader()
{
    free();
}