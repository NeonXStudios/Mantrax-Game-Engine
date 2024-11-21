#pragma once
#include "UIComponent.h"
#include <Core.h>
#include <nlohmann/json.hpp>
#include <GarinIO.h>
#include <FileManager.h>

using json = nlohmann::json;

class GARINLIBS_API InputSystemUI : public UIComponent
{
public:
    struct KeyBinding
    {
        GLuint key_code;
        std::string name;
    };

    std::unordered_map<int, KeyBinding> key_bindings;

    bool capturing_key = false;
    int key_to_capture = -1;

    void draw() override;

    void save_key_bindings(const std::string &filename)
    {
        json j;
        for (const auto &[key_id, key_binding] : key_bindings)
        {
            j[std::to_string(key_id)] = {
                {"key_code", key_binding.key_code},
                {"name", key_binding.name}};
        }
        std::ofstream file(filename);
        file << j.dump(4);
    }

    void load_key_bindings(const std::string &filename)
    {
        std::ifstream file(filename);
        if (file)
        {
            json j;
            file >> j;
            for (auto &[key_id, value] : j.items())
            {
                KeyBinding binding;
                binding.key_code = value["key_code"].get<GLuint>();
                binding.name = value["name"].get<std::string>();
                key_bindings[std::stoi(key_id)] = binding;
            }
        }
    }
};