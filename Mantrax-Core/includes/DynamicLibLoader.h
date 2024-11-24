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
    static DynamicLibLoader *instance;
    std::unordered_map<std::string, Function<Shared<Component>()>> factories;
    std::unique_ptr<Loader> loader;
    uint64_t loader_dll_stamp;

    std::string from_dll_path;
    std::string dll_path;
    bool dll_in_recompile = false;

    DynamicLibLoader()
    {
        loader = std::make_unique<Loader>();
    }

    void create();
    void release();
    void load_components(std::string _path);
    void check_components_reload();
    void reset_component_registry();
    void update();
    void assign_data();

    // GARIN_API void register_component(const std::string &name, Function<Shared<Component>()> factory);
    Shared<Component> create_component(const std::string &name) const;
};

#endif // ENGINELIBSLOADER_H
