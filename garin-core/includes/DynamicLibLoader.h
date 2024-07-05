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

class Component;

class DynamicLibLoader
{
public:
    std::unique_ptr<Loader> loader = std::make_unique<Loader>();
    std::unordered_map<std::string, std::function<std::shared_ptr<Component>()>> factories;
    uint64_t loader_dll_stamp;

    std::shared_ptr<Component> create_component(const std::string &name) const;
    void register_component(const std::string &name, std::function<std::shared_ptr<Component>()> factory);

    void load_components();
    void check_components_reload();
    void reset_component_registry();

    template <typename T>
    void register_component_type(const std::string &name)
    {
        register_component(
            name,
            [name]()
            {
                std::shared_ptr<Component> c = std::make_shared<T>();
                c->_name = name;
                return c;
            });
    }
};

#endif // ENGINELIBSLOADER_H
