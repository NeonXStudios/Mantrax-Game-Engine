#pragma once
#ifndef ENGINELIBSLOADER_H
#define ENGINELIBSLOADER_H

#include "Loader.h"
#include <FileManager.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "types.h"
#include "Core.h"

class Component;
class GARINLIBS_API DynamicLibLoader
{
public:
    std::unordered_map<std::string, Function<Shared<Component>()>> factories;
    std::unique_ptr<Loader> loader;
    uint64_t loader_dll_stamp;

    DynamicLibLoader()
    {
        loader = std::make_unique<Loader>();
    }

    void load_components();
    void check_components_reload();
    void reset_component_registry();

    // GARIN_API void register_component(const std::string &name, Function<Shared<Component>()> factory);
    Shared<Component> create_component(const std::string &name) const;

    // template <typename T>
    // void register_component(const std::string &name)
    // {
    //     register_component(
    //         name,
    //         [name]()
    //         {
    //             Shared<Component> c = new_shared<T>();
    //             c->_name = name;
    //             return c;
    //         });
    // }
};

#endif // ENGINELIBSLOADER_H
