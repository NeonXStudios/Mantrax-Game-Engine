#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

class VarVerify
{
public:
    template <typename T>
    static void set_value_if_exists(const json &j, const std::string &key, T &value)
    {
        if (j.contains(key))
        {
            value = j[key].get<T>();
        }
    }
};
