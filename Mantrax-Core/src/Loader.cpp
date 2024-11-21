#include "../includes/Loader.h"
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__linux__)
#include <dlfcn.h>
#endif

// Loader::Loader(const char *path)
// {
//     load(path);
// }

// void Loader::load(const char *path)
// {
//     if (!path || std::string(path).empty())
//     {
//         std::cerr << "Ruta no válida para cargar la DLL/biblioteca dinámica." << std::endl;
//     }

//     if (handle)
//     {
//         std::cerr << "Ya hay una DLL/biblioteca cargada. Libérala antes de cargar otra." << std::endl;
//     }

// #if defined(_WIN32) || defined(_WIN64)
//     handle = LoadLibrary(path);
//     if (!handle)
//     {
//         std::cerr << "Error al cargar la DLL: " << path << " - Código de error: " << GetLastError() << std::endl;
//     }
// #elif defined(__linux__)
//     handle = dlopen(path, RTLD_LAZY);
//     if (!handle)
//     {
//         std::cerr << "Error al cargar la biblioteca dinámica: " << dlerror() << std::endl;
//     }
// #endif
//     std::cout << "DLL/biblioteca cargada correctamente: " << path << std::endl;
// }

// void Loader::free()
// {
//     if (!handle)
//     {
//         std::cerr << "No hay DLL/biblioteca cargada para liberar." << std::endl;
//         return;
//     }

// #if defined(_WIN32) || defined(_WIN64)
//     if (FreeLibrary(static_cast<HMODULE>(handle)))
//     {
//         std::cout << "DLL liberada correctamente." << std::endl;
//     }
//     else
//     {
//         std::cerr << "Error al liberar la DLL. Código de error: " << GetLastError() << std::endl;
//     }
// #elif defined(__linux__)
//     if (dlclose(handle) == 0)
//     {
//         std::cout << "Biblioteca dinámica liberada correctamente." << std::endl;
//     }
//     else
//     {
//         std::cerr << "Error al liberar la biblioteca dinámica: " << dlerror() << std::endl;
//     }
// #endif
//     handle = nullptr;
// }

// Loader::~Loader()
// {
//     free();
// }
