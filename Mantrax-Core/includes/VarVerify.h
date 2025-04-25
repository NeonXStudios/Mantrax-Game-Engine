#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <Core.h>

using namespace nlohmann;

class VarVerify
{
public:
    template <typename T>
    static bool set_value_if_exists(const json &j, const std::string &key, T &value)
    {
        try
        {
            if (j.contains(key))
            {
                value = j[key].get<T>();
                return true;
            }
        }
        catch (const json::type_error &e)
        {
            std::cerr << "JSON type error: " << e.what() << std::endl;
        }
        catch (const json::parse_error &e)
        {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown error occurred." << std::endl;
        }

        return false;
    }
};
