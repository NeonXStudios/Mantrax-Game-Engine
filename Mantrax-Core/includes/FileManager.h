#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <filesystem>

#include <cstddef>
#include <windows.h>
#include <nlohmann/json.hpp>
#include "Core.h"

namespace fs = std::filesystem;

struct GARINLIBS_API FileData
{
    std::string file_name;
    std::string file_stem;
    std::string file_extension;
};

class GARINLIBS_API FileManager
{
public:
    template <typename T>
    static T get_file(const std::string &path);

    static std::vector<fs::directory_entry> get_files_noT(const std::string &path);
    static std::vector<std::string> get_files_by_extension(std::string dir_to_it, std::string file_extension);
    static std::string read_file(const std::string &file_name);
    static std::string get_execute_path();
    static std::string get_project_path();
    static std::string game_path;

    static bool make_dir(const std::string &path);
    static bool delete_folder(std::string folder_path);
    static bool open_explorer(const std::string &folder_path);
    static bool check_file_if_exist(const std::string &path);
    static bool check_file_extension(const std::string &path);
    static bool open_file(const std::string &path);

    static void write_file(const std::string &file_name, const std::string &content);
    static void copy_directory_contents(const std::filesystem::path &source, const std::filesystem::path &destination);

    static FileData get_file_info(std::string file_path);
};
