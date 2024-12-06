#pragma once
#include <FileManager.h>
#include <GarinNatives.h>

class ResourcesManager
{
public:
    template <typename T>
    static T *get_resource(std::string file_path)
    {
        std::string full_file_path = FileManager::get_project_path() + "/" + file_path;
        return new T(full_file_path);
    }

    template <typename T, typename... Args>
    static T *get_resource_multi_constructor(const std::string &file_path, Args &&...args)
    {
        std::string full_file_path = FileManager::get_project_path() + "/" + file_path;
        return new T(full_file_path, std::forward<Args>(args)...);
    }
};