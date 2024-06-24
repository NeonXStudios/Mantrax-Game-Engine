#pragma once
#include <GarinIO.h>
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class WindowsCompiler
{
public:
    static void copy_scene_files(const std::string &source_dir, const std::string &destination_dir)
    {
        if (!fs::exists(destination_dir))
        {
            fs::create_directories(destination_dir);
        }

        for (const auto &entry : fs::recursive_directory_iterator(source_dir))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".scene")
            {
                fs::path destination_path = destination_dir / entry.path().filename();
                if (fs::exists(destination_path))
                {
                    fs::remove(destination_path);
                }
                fs::copy(entry.path(), destination_path, fs::copy_options::overwrite_existing);
                std::cout << "Copied: " << entry.path() << " to " << destination_path << std::endl;
            }
        }
    }

    static void copy_shaders_files(const std::string &source_dir, const std::string &destination_dir)
    {
        if (!fs::exists(destination_dir))
        {
            fs::create_directories(destination_dir);
        }

        for (const auto &entry : fs::recursive_directory_iterator(source_dir))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".glsl")
            {
                fs::path destination_path = destination_dir / entry.path().filename();
                if (fs::exists(destination_path))
                {
                    fs::remove(destination_path);
                }
                fs::copy(entry.path(), destination_path, fs::copy_options::overwrite_existing);
                std::cout << "Copied: " << entry.path() << " to " << destination_path << std::endl;
            }
        }
    }

    static void copy_models_files(const std::string &source_dir, const std::string &destination_dir)
    {
        if (!fs::exists(destination_dir))
        {
            fs::create_directories(destination_dir);
        }

        for (const auto &entry : fs::recursive_directory_iterator(source_dir))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".fbx")
            {
                fs::path destination_path = destination_dir / entry.path().filename();
                if (fs::exists(destination_path))
                {
                    fs::remove(destination_path);
                }
                fs::copy(entry.path(), destination_path, fs::copy_options::overwrite_existing);
                std::cout << "Copied: " << entry.path() << " to " << destination_path << std::endl;
            }
        }
    }

    static void copy_audio_files(const std::string &source_dir, const std::string &destination_dir)
    {
        if (!fs::exists(destination_dir))
        {
            fs::create_directories(destination_dir);
        }

        for (const auto &entry : fs::recursive_directory_iterator(source_dir))
        {
            if (entry.is_regular_file() && (entry.path().extension() == ".wav" || entry.path().extension() == ".mp3"))
            {
                fs::path destination_path = destination_dir / entry.path().filename();
                if (fs::exists(destination_path))
                {
                    fs::remove(destination_path);
                }
                fs::copy(entry.path(), destination_path, fs::copy_options::overwrite_existing);
                std::cout << "Copied: " << entry.path() << " to " << destination_path << std::endl;
            }
        }
    }

    static void copy_textures_files(const std::string &source_dir, const std::string &destination_dir)
    {
        if (!fs::exists(destination_dir))
        {
            fs::create_directories(destination_dir);
        }

        for (const auto &entry : fs::recursive_directory_iterator(source_dir))
        {
            if (entry.is_regular_file() && (entry.path().extension() == ".png" || entry.path().extension() == ".jpg"))
            {
                fs::path destination_path = destination_dir / entry.path().filename();
                if (fs::exists(destination_path))
                {
                    fs::remove(destination_path);
                }
                fs::copy(entry.path(), destination_path, fs::copy_options::overwrite_existing);
                std::cout << "Copied: " << entry.path() << " to " << destination_path << std::endl;
            }
        }
    }

    static void copy_and_rename_file(const std::string &source_file, const std::string &destination_dir, const std::string &new_name)
    {
        fs::path source_path = source_file;
        fs::path destination_path = fs::path(destination_dir) / new_name;

        if (!fs::exists(source_path))
        {
            std::cerr << "Source file does not exist: " << source_path << std::endl;
            return;
        }

        if (!fs::exists(destination_dir))
        {
            fs::create_directories(destination_dir);
        }

        try
        {
            fs::copy_file(source_path, destination_path, fs::copy_options::overwrite_existing);
            std::cout << "Copied: " << source_path << " to " << destination_path << std::endl;
        }
        catch (const fs::filesystem_error &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    static void compile_windows()
    {
        std::string data_path = FileManager::get_execute_path() + "projects/test/wlibsgpp";

        copy_scene_files(FileManager::get_execute_path() + "projects/test/assets", FileManager::get_execute_path() + "projects/test/build/data/scenes/");
        copy_shaders_files(FileManager::get_execute_path() + "projects/test/assets", FileManager::get_execute_path() + "projects/test/build/data/");
        copy_models_files(FileManager::get_execute_path() + "projects/test/assets", FileManager::get_execute_path() + "projects/test/build/data/");
        copy_audio_files(FileManager::get_execute_path() + "projects/test/assets", FileManager::get_execute_path() + "projects/test/build/data/");
        copy_textures_files(FileManager::get_execute_path() + "projects/test/assets", FileManager::get_execute_path() + "projects/test/build/data/");
        copy_and_rename_file(FileManager::get_execute_path() + "projects/test/EngineSettings.data", FileManager::get_execute_path() + "projects/test/build/", FileManager::get_execute_path() + "projects/test/build/GameSettings.data");

        std::filesystem::current_path(data_path);

        int result = system("mingw32-make");

        if (result == 0)
        {
            std::cout << "Game compiled platform => Window" << std::endl;
        }
        else
        {
            std::cout << "Error compiling Makefile" << std::endl;
        }
    }
};
