#pragma once
#include <GarinIO.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <Core.h>
#include <UINotification.h>

namespace fs = std::filesystem;

class GARINLIBS_API WindowsCompiler
{
public:
    static void copy_file(const std::string &sourceFile, const std::string &destinationFile)
    {
        std::ifstream source(sourceFile, std::ios::binary);
        std::ofstream destination(destinationFile, std::ios::binary);

        // Verificar si los archivos se abrieron correctamente
        if (!source.is_open())
        {
            std::cerr << "Error on try copy DLL: " << sourceFile << std::endl;
            return;
        }

        if (!destination.is_open())
        {
            std::cerr << "There was an error when copying a DLL to the destination: " << destinationFile << std::endl;
            return;
        }

        // Leer y escribir el archivo
        destination << source.rdbuf();

        // Cerrar archivos
        source.close();
        destination.close();

        std::cout << "DLL Installed " << sourceFile << " to " << destinationFile << std::endl;
    }

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

    static bool copy_directory(const fs::path &source, const fs::path &destination)
    {
        try
        {
            // Comprobar si la carpeta de origen existe y es un directorio
            if (!fs::exists(source) || !fs::is_directory(source))
            {
                std::cerr << "Error: La carpeta de origen no existe o no es un directorio." << std::endl;
                return false;
            }

            // Crear la carpeta de destino si no existe
            if (!fs::exists(destination))
            {
                if (!fs::create_directories(destination))
                {
                    std::cerr << "Error: No se pudo crear la carpeta de destino." << std::endl;
                    return false;
                }
            }

            // Iterar sobre todos los archivos y subdirectorios en la carpeta de origen
            for (const auto &entry : fs::directory_iterator(source))
            {
                const auto destination_path = destination / entry.path().filename();

                // Si es un archivo, copiarlo
                if (fs::is_regular_file(entry))
                {
                    fs::copy_file(entry.path(), destination_path, fs::copy_options::overwrite_existing);
                }
                // Si es un directorio, llamar recursivamente a esta función
                else if (fs::is_directory(entry))
                {
                    if (!copy_directory(entry.path(), destination_path))
                    {
                        return false; // Error al copiar subdirectorio
                    }
                }
            }
            return true;
        }
        catch (const fs::filesystem_error &e)
        {
            std::cerr << "Error de sistema de archivos: " << e.what() << std::endl;
            return false;
        }
    }

    static void copy_all_data(/*std::string source_path, std::string destination_path*/)
    {
        fs::path source_path = FileManager::get_execute_path() + "projects/test/assets";
        fs::path destination_path = FileManager::get_execute_path() + "projects/test/build/assets";

        if (copy_directory(source_path, destination_path))
        {
            std::cout << "Carpeta y archivos copiados correctamente." << std::endl;
        }
        else
        {
            std::cerr << "Error al copiar la carpeta y archivos." << std::endl;
        }
    }

    static void compile_windows()
    {
        std::string data_path = FileManager::get_execute_path() + "projects/test/wlibsgpp";

        // copy_scene_files(FileManager::get_execute_path() + "projects/test/assets", FileManager::get_execute_path() + "projects/test/build/data/scenes/");
        // copy_shaders_files(FileManager::get_execute_path() + "projects/test/assets", FileManager::get_execute_path() + "projects/test/build/data/");
        // copy_models_files(FileManager::get_execute_path() + "projects/test/assets", FileManager::get_execute_path() + "projects/test/build/data/");
        // copy_audio_files(FileManager::get_execute_path() + "projects/test/assets", FileManager::get_execute_path() + "projects/test/build/data/");
        // copy_textures_files(FileManager::get_execute_path() + "projects/test/assets", FileManager::get_execute_path() + "projects/test/build/data/");
        // copy_and_rename_file(FileManager::get_execute_path() + "projects/test/EngineSettings.data", FileManager::get_execute_path() + "projects/test/build/", FileManager::get_execute_path() + "projects/test/build/GameSettings.data");
        copy_all_data();

        std::filesystem::current_path(data_path);

        std::string cmake_path = FileManager::get_project_path() + "wlibsgpp/Compiler-Lib/GarinEditorEngine/";

        // Formar los comandos
        std::string cmake_command = "cd /d " + cmake_path + " && cmake -G \"Visual Studio 17 2022\" .";
        std::string msbuild_command = "cd /d " + cmake_path + " && msbuild GarinEngine.sln /p:Configuration=Debug";

        // Ejecutar el comando CMake
        int result = system(cmake_command.c_str());

        if (result == 0)
        {
            UINotification::AddNotification("CMake Reloaded...", 10.0f);
            UINotification::AddNotification("Starting compilation of the libraries (Wait)...", 10.0f);

            // Ejecutar el comando MSBuild
            int result_build = system(msbuild_command.c_str());

            if (result_build == 0)
            {
                UINotification::AddNotification("Successfully compiled libraries...", 10.0f);

                std::string cmake_path_game = FileManager::get_project_path() + "wlibsgpp/build/";
                copy_file(cmake_path + "Debug/GarinGameCore.dll", cmake_path_game + "Debug/GarinGameCore.dll");

                // Formar los comandos
                std::string cmake_command_game = "cd /d " + cmake_path_game + " && cmake -G \"Visual Studio 17 2022\" .";
                std::string msbuild_command_game = "cd /d " + cmake_path_game + " && msbuild GarinGame.sln /p:Configuration=Debug";

                // Ejecutar el comando CMake
                int result = system(cmake_command_game.c_str());

                if (result == 0)
                {
                    UINotification::AddNotification("CMake Reloaded...", 10.0f);
                    UINotification::AddNotification("Starting compilation of the libraries (Wait)...", 10.0f);

                    // Ejecutar el comando MSBuild
                    int result_build_game = system(msbuild_command_game.c_str());

                    if (result_build_game == 0)
                    {
                        UINotification::AddNotification("Successfully game compiled...", 10.0f);
                    }
                    else
                    {
                        UINotification::AddNotification("Error during the compilation of the libraries...", 10.0f);
                        std::cerr << "Error during msbuild execution, result code: " << result_build << std::endl;
                    }
                }
                else
                {
                    UINotification::AddNotification("Error during CMake execution...", 10.0f);
                    std::cerr << "Error during compile game - result code: " << result << std::endl;
                }
            }
            else
            {
                UINotification::AddNotification("Error during the compilation of the libraries...", 10.0f);
                std::cerr << "Error during msbuild execution, result code: " << result_build << std::endl;
            }
        }
        else
        {
            UINotification::AddNotification("Error during CMake execution...", 10.0f);
            std::cerr << "Error during cmake execution, result code: " << result << std::endl;
        }
    }
};
